#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CLAUSES 100
#define MAX_LITERALS 20
#define MAX_VAR_NAME 10
#define MAX_FORMULA_LEN 256

typedef struct {
    char literals[MAX_LITERALS][MAX_VAR_NAME];
    int num_literals;
} Clause;

typedef struct {
    Clause clauses[MAX_CLAUSES];
    int num_clauses;
} ClauseSet;

// Validate a single literal (e.g., "p", "-p")
int is_valid_literal(const char* literal) {
    int len = strlen(literal);
    if (len == 0 || len >= MAX_VAR_NAME) return 0;
    int i = 0;
    if (literal[0] == '-') i++;
    if (i >= len || !isalpha(literal[i])) return 0;
    i++;
    while (i < len) {
        if (!isalnum(literal[i])) return 0;
        i++;
    }
    return 1;
}

// Validate a single CNF clause (e.g., "p -q 0")
int validate_cnf_clause(const char* clause_str, Clause* clause) {
    char temp[256];
    strncpy(temp, clause_str, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    clause->num_literals = 0;
    char* token = strtok(temp, " \n");
    while (token && strcmp(token, "0") != 0) {
        if (!is_valid_literal(token)) return 0;
        if (clause->num_literals >= MAX_LITERALS) return 0;
        strncpy(clause->literals[clause->num_literals], token, MAX_VAR_NAME - 1);
        clause->literals[clause->num_literals][MAX_VAR_NAME - 1] = '\0';
        clause->num_literals++;
        token = strtok(NULL, " \n");
    }
    if (!token || strcmp(token, "0") != 0) return 0;
    return clause->num_literals > 0;
}

// Validate CNF file format
int validate_cnf_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Clause temp_clause;
        if (!validate_cnf_clause(line, &temp_clause)) {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

// Read clauses from a file in simplified CNF format
int read_clauses(const char* filename, ClauseSet* set) {
    if (!validate_cnf_file(filename)) {
        printf("Error: File %s is not in valid CNF format.\n", filename);
        return 0;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return 0;
    }

    set->num_clauses = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) && set->num_clauses < MAX_CLAUSES) {
        Clause* clause = &set->clauses[set->num_clauses];
        if (validate_cnf_clause(line, clause)) {
            set->num_clauses++;
        }
    }

    fclose(file);
    return set->num_clauses > 0;
}

// Check if two literals are complementary
int are_complementary(const char* lit1, const char* lit2) {
    if (lit1[0] == '-' && lit2[0] != '-') {
        return strcmp(lit1 + 1, lit2) == 0;
    }
    if (lit2[0] == '-' && lit1[0] != '-') {
        return strcmp(lit2 + 1, lit1) == 0;
    }
    return 0;
}

// Perform resolution between two clauses
int resolve(Clause* c1, Clause* c2, Clause* result) {
    result->num_literals = 0;
    int resolved = 0;

    for (int i = 0; i < c1->num_literals; i++) {
        for (int j = 0; j < c2->num_literals; j++) {
            if (are_complementary(c1->literals[i], c2->literals[j])) {
                resolved = 1;
                for (int k = 0; k < c1->num_literals; k++) {
                    if (k != i) {
                        strncpy(result->literals[result->num_literals], c1->literals[k], MAX_VAR_NAME);
                        result->num_literals++;
                    }
                }
                for (int k = 0; k < c2->num_literals; k++) {
                    if (k != j) {
                        int duplicate = 0;
                        for (int m = 0; m < result->num_literals; m++) {
                            if (strcmp(result->literals[m], c2->literals[k]) == 0) {
                                duplicate = 1;
                                break;
                            }
                        }
                        if (!duplicate) {
                            strncpy(result->literals[result->num_literals], c2->literals[k], MAX_VAR_NAME);
                            result->num_literals++;
                        }
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}

// Check if a clause is empty
int is_empty_clause(Clause* clause) {
    return clause->num_literals == 0;
}

// Resolution algorithm
int resolution(ClauseSet* set) {
    int new_clauses;
    do {
        new_clauses = 0;
        for (int i = 0; i < set->num_clauses; i++) {
            for (int j = i + 1; j < set->num_clauses; j++) {
                Clause result;
                if (resolve(&set->clauses[i], &set->clauses[j], &result)) {
                    if (is_empty_clause(&result)) {
                        return 0; // Unsatisfiable
                    }
                    int exists = 0;
                    for (int k = 0; k < set->num_clauses; k++) {
                        if (set->clauses[k].num_literals == result.num_literals) {
                            int match = 1;
                            for (int m = 0; m < result.num_literals; m++) {
                                int found = 0;
                                for (int n = 0; n < set->clauses[k].num_literals; n++) {
                                    if (strcmp(result.literals[m], set->clauses[k].literals[n]) == 0) {
                                        found = 1;
                                        break;
                                    }
                                }
                                if (!found) {
                                    match = 0;
                                    break;
                                }
                            }
                            if (match) {
                                exists = 1;
                                break;
                            }
                        }
                    }
                    if (!exists && set->num_clauses < MAX_CLAUSES) {
                        set->clauses[set->num_clauses] = result;
                        set->num_clauses++;
                        new_clauses = 1;
                    }
                }
            }
        }
    } while (new_clauses);
    return 1; // Satisfiable
}

// Print clauses
void print_clauses(ClauseSet* set) {
    if (set->num_clauses == 0) {
        printf("No clauses to display.\n");
        return;
    }
    for (int i = 0; i < set->num_clauses; i++) {
        printf("Clause %d: ", i + 1);
        for (int j = 0; j < set->clauses[i].num_literals; j++) {
            printf("%s ", set->clauses[i].literals[j]);
        }
        printf("\n");
    }
}

// Validate a propositional formula
int validate_formula(const char* formula) {
    int len = strlen(formula);
    int paren_count = 0;
    int i = 0;

    while (i < len) {
        char c = formula[i];
        if (isspace(c)) {
            i++;
            continue;
        }
        if (c == '(') {
            paren_count++;
            i++;
            continue;
        }
        if (c == ')') {
            paren_count--;
            if (paren_count < 0) return 0;
            i++;
            continue;
        }
        if (c == '&' || c == '|') {
            i++;
            continue;
        }
        if (isalpha(c) || c == '-') {
            int start = i;
            if (c == '-') i++;
            if (i >= len || !isalpha(formula[i])) return 0;
            i++;
            while (i < len && isalnum(formula[i])) i++;
            if (i - start > MAX_VAR_NAME - 1) return 0;
            continue;
        }
        return 0;
    }

    return paren_count == 0;
}

// Convert formula to CNF
int formula_to_cnf(const char* formula, ClauseSet* set) {
    set->num_clauses = 0;
    char temp[MAX_FORMULA_LEN];
    strncpy(temp, formula, MAX_FORMULA_LEN - 1);
    temp[MAX_FORMULA_LEN - 1] = '\0';

    // Normalize: keep spaces around operators and literals
    char normalized[MAX_FORMULA_LEN] = {0};
    int j = 0;
    for (int i = 0; temp[i]; i++) {
        if (isspace(temp[i])) continue;
        if (temp[i] == '(' || temp[i] == ')' || temp[i] == '|' || temp[i] == '&') {
            if (j > 0 && normalized[j-1] != ' ') normalized[j++] = ' ';
            normalized[j++] = temp[i];
            normalized[j++] = ' ';
            continue;
        }
        normalized[j++] = temp[i];
    }
    normalized[j] = '\0';

    // Trim leading/trailing spaces
    char* start = normalized;
    while (*start == ' ') start++;
    char* end = start + strlen(start) - 1;
    while (end > start && *end == ' ') end--;
    *(end + 1) = '\0';

    // Split by '&'
    char* clause_str = strtok(start, "&");
    while (clause_str && set->num_clauses < MAX_CLAUSES) {
        Clause* clause = &set->clauses[set->num_clauses];
        clause->num_literals = 0;

        // Trim clause
        while (*clause_str == ' ') clause_str++;
        char* clause_end = clause_str + strlen(clause_str) - 1;
        while (clause_end > clause_str && *clause_end == ' ') clause_end--;
        *(clause_end + 1) = '\0';

        // Remove parentheses
        char clause_temp[MAX_FORMULA_LEN];
        strncpy(clause_temp, clause_str, MAX_FORMULA_LEN - 1);
        clause_temp[MAX_FORMULA_LEN - 1] = '\0';
        char* clause_start = clause_temp;
        if (clause_temp[0] == '(') clause_start++;
        char* paren_end = strchr(clause_start, ')');
        if (paren_end) *paren_end = '\0';

        // Split by '|'
        char* literal = strtok(clause_start, "|");
        while (literal && clause->num_literals < MAX_LITERALS) {
            // Trim spaces
            while (*literal == ' ') literal++;
            char* lit_end = literal + strlen(literal) - 1;
            while (lit_end > literal && *lit_end == ' ') lit_end--;
            *(lit_end + 1) = '\0';

            if (!is_valid_literal(literal)) {
                // printf("Debug: Invalid literal '%s' in clause '%s'\n", literal, clause_str);
                set->num_clauses = 0;
                return 0;
            }
            strncpy(clause->literals[clause->num_literals], literal, MAX_VAR_NAME - 1);
            clause->literals[clause->num_literals][MAX_VAR_NAME - 1] = '\0';
            clause->num_literals++;
            literal = strtok(NULL, "|");
        }

        if (clause->num_literals > 0) {
            set->num_clauses++;
        } else {
            // printf("Debug: No valid literals in clause '%s'\n", clause_str);
            set->num_clauses = 0;
            return 0;
        }
        clause_str = strtok(NULL, "&");
    }

    return set->num_clauses > 0;
}

// Unit tests
void run_tests() {
    ClauseSet set;

    printf("\n=== Running Unit Tests ===\n");
    printf("Test 1: Unsatisfiable set\n");
    FILE* temp = fopen("test1.cnf", "w");
    fprintf(temp, "p 0\n");
    fprintf(temp, "-p 0\n");
    fclose(temp);
    if (read_clauses("test1.cnf", &set)) {
        print_clauses(&set);
        int result = resolution(&set);
        printf("Result: %s\n\n", result ? "Satisfiable" : "Unsatisfiable");
    } else {
        printf("Test 1 failed: Invalid CNF file.\n\n");
    }

    printf("Test 2: Satisfiable set\n");
    temp = fopen("test2.cnf", "w");
    fprintf(temp, "p -q 0\n");
    fprintf(temp, "-p q 0\n");
    fclose(temp);
    if (read_clauses("test2.cnf", &set)) {
        print_clauses(&set);
        int result = resolution(&set);
        printf("Result: %s\n\n", result ? "Satisfiable" : "Unsatisfiable");
    } else {
        printf("Test 2 failed: Invalid CNF file.\n\n");
    }

    printf("Test 3: Formula to CNF\n");
    const char* formula = "(p | -q) & (-p | q)";
    if (formula_to_cnf(formula, &set)) {
        printf("Formula: %s\n", formula);
        print_clauses(&set);
    } else {
        printf("Error converting formula to CNF.\n");
    }

    printf("Test 4: Formula to CNF (user case)\n");
    const char* formula2 = "(a | -b) & (-a | b)";
    if (formula_to_cnf(formula2, &set)) {
        printf("Formula: %s\n", formula2);
        print_clauses(&set);
    } else {
        printf("Error converting formula to CNF.\n");
    }
}

// User interface
void user_interface() {
    ClauseSet set;
    char input[256];
    int choice;

    while (1) {
        printf("\n====================================\n");
        printf("       Logical Solver v1.0\n");
        printf("====================================\n");
        printf("1. Check satisfiability of CNF clauses\n");
        printf("2. Convert formula to CNF\n");
        printf("3. Check satisfiability from CNF file\n");
        printf("4. Run unit tests\n");
        printf("5. Exit\n");
        printf("------------------------------------\n");
        printf("Enter choice (1-5): ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("\nError: Please enter a number between 1 and 5.\n");
            continue;
        }
        getchar();

        switch (choice) {
            case 1:
                printf("\nEnter CNF clauses, one per line (e.g., p -q 0).\n");
                printf("End with a blank line:\n> ");
                set.num_clauses = 0;
                while (1) {
                    fgets(input, sizeof(input), stdin);
                    input[strcspn(input, "\n")] = '\0';
                    if (strlen(input) == 0) break;
                    if (set.num_clauses >= MAX_CLAUSES) {
                        printf("Error: Too many clauses (max %d).\n", MAX_CLAUSES);
                        set.num_clauses = 0;
                        break;
                    }
                    Clause* clause = &set.clauses[set.num_clauses];
                    if (!validate_cnf_clause(input, clause)) {
                        printf("Error: Invalid CNF clause: %s\n", input);
                        printf("Use space-separated literals (e.g., p, -p) ending with 0.\n");
                        set.num_clauses = 0;
                        break;
                    }
                    set.num_clauses++;
                }
                if (set.num_clauses > 0) {
                    printf("\nEntered Clauses:\n");
                    print_clauses(&set);
                    int result = resolution(&set);
                    printf("\nResult: %s\n", result ? "Satisfiable" : "Unsatisfiable");
                } else if (set.num_clauses == 0 && strlen(input) != 0) {
                    printf("\nNo valid clauses entered.\n");
                }
                break;

            case 2:
                printf("\nEnter propositional formula (e.g., (p | -q) & (-p | q)):\n> ");
                fgets(input, MAX_FORMULA_LEN, stdin);
                input[strcspn(input, "\n")] = '\0';

                if (!validate_formula(input)) {
                    printf("Error: Invalid formula. Use literals (e.g., p, -p), operators (&, |), and parentheses.\n");
                    break;
                }

                if (formula_to_cnf(input, &set)) {
                    printf("\nCNF Representation:\n");
                    print_clauses(&set);
                } else {
                    printf("Error: Could not convert formula to CNF.\n");
                }
                break;

            case 3:
                printf("\nEnter CNF filename (e.g., input.cnf):\n> ");
                fgets(input, MAX_FORMULA_LEN, stdin);
                input[strcspn(input, "\n")] = '\0';
                if (read_clauses(input, &set)) {
                    printf("\nLoaded Clauses:\n");
                    print_clauses(&set);
                    int result = resolution(&set);
                    printf("\nResult: %s\n", result ? "Satisfiable" : "Unsatisfiable");
                }
                break;

            case 4:
                run_tests();
                break;

            case 5:
                printf("\nThank you for using Logical Solver!\n");
                return;

            default:
                printf("\nError: Please enter a number between 1 and 5.\n");
        }
    }
}

int main() {
    user_interface();
    return 0;
}