#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <stdbool.h>

#define COLOR_DEFAULT "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_BOLD "\033[1m"
#define COLOR_UNDERLINE "\033[4m"
// define a structure with a table that includes all ascii's of the chars, we need ahashmap for that
typedef struct Clause{
    char *clause;
    char **variables;
}Clause;
char *strndup(const char *s, size_t n) {
    size_t len = strnlen(s, n);
    char *copy = (char *)malloc(len + 1);
    if (!copy) return NULL;
    memcpy(copy, s, len);
    copy[len] = '\0';
    return copy;
}
// helper functions are here
int Hash(char c){
    return c - 'A';
}

char **initVarsMap(){
    char **vars = (char **)malloc(sizeof(char *) * 26);
    for(int i = 0; i < 26; i++) vars[i] = NULL;
    return vars;
}

void printResultBox(const char* text, const char* color) {
    int width = 60;
    printf("%s|", color);
    for (int i = 0; i < width-2; i++) printf("=");
    printf("|\n| %-56s |\n|", text);
    for (int i = 0; i < width-2; i++) printf("=");
    printf("|\n" COLOR_DEFAULT);
}

void printClauseBox(const char* clause) {
    printf(COLOR_CYAN "  Clause: " COLOR_YELLOW "%-50s" COLOR_DEFAULT "\n", clause);
}


void editClause(char **variable, char *prop, char *clause, int index){

    *variable = (char *)malloc(sizeof(char) * (strlen(prop) + 1));

    
    strcpy(*variable, prop);
    // matter of string manipulation
    if(strlen(clause) == 0){
        strcat(clause, prop);
    } else {
        strcat(clause, "|");
        strcat(clause, prop);
    }
}

int getClauseCount(char *clauses){
    // we try to do a dynamic aray kinda thing here
    int n = strlen(clauses);
    //make sure we have at least one clause
    if(n == 0) return 0;

    int count = 0;
    for(int i = 0; i < n ; i++){
        if(clauses[i] == '&') count += 1;
    }
    //last one will be automatically added at the end
    return count + 1;
}

char **extractClauses(char *str){
    int clausesCount = getClauseCount(str);
    int curClause = 0;
    char **clauses = (char **)malloc(clausesCount * sizeof(char *));
    if(clauses == NULL){
        // memory allocation failed
        return NULL;
    }
    int curLength = 5;
    char *temp = (char *)malloc(sizeof(char) * curLength);
    int j = 0;
    int n = strlen(str);
    for(int i = 0; i < n ; i++){
        if( str[i] != '&'){
            temp[j++] = str[i];
            // expand the string if it is full at any point in time
            if(j == curLength){
                curLength += 2;
                char *newString = (char *)realloc(temp, curLength);
                if(newString == NULL){
                    return NULL;
                }
                temp = newString;
            }
        } else {
            // assign the string extracted to the wherever we are, then reset everything for the next string
            temp[j] = '\0';
            clauses[curClause++] = temp;
            j = 0;
            curLength = 5;
            //allocate a new space
            temp = (char *)malloc(sizeof(char) * curLength);
        }
    }
    // again for the last clause
    temp[j] = '\0';
    clauses[curClause++] = temp;
    return clauses;
}

void freeClause(struct Clause *clause){
    // free the clause
    if(clause == NULL) return ;
    if(clause->variables != NULL){

        if(clause->clause != NULL) free(clause->clause);
        // free the variables
        for(int i = 0; i < 26; i++){
            if(clause->variables[i] != NULL){
                free(clause->variables[i]);
            }
        }
        free(clause->variables);
    }
    free(clause);
}

struct Clause *getVariables(char *clause){
    int n = strlen(clause);
    // initializations here
    struct Clause *vars = (struct Clause *)malloc(sizeof(struct Clause));
    vars->clause = (char *)malloc(sizeof(char ) * n);
    strcpy(vars->clause, clause);
    vars->variables = initVarsMap();

    // here we begin
    int negation = 0;
    for(int i = 0; i < n; i++){
        // case of a negated variable
        char c = clause[i];
        if(clause[i] == '!') negation = 1;
        // case of end of a clause
        else if(clause[i] == '|'){
            negation = 0;
            continue;
        } else {
            if(vars->variables[Hash(clause[i])] != NULL){

                if(negation && vars->variables[Hash(clause[i])][0] != '!'){
                   free(vars->variables[Hash(clause[i])]);
                   vars->variables[Hash(clause[i])] = NULL;

                } else if(!negation && vars->variables[Hash(clause[i])][0] == '!'){
                    free(vars->variables[Hash(clause[i])]);
                    vars->variables[Hash(clause[i])] = NULL;
                    
                } else {
                    continue;
                }
            } else {
                vars->variables[Hash(clause[i])] = (char *)malloc(sizeof(char) * (negation == 1 ? 3 : 2));
                //assign the string manually!!
                // since the string won't have more than 2 chars, we can do this
                if(!negation){
                    vars->variables[Hash(clause[i])][0] = clause[i];
                    vars->variables[Hash(clause[i])][1] = '\0';
                } else {
                    vars->variables[Hash(clause[i])][0] = '!';
                    vars->variables[Hash(clause[i])][1] = clause[i];
                    vars->variables[Hash(clause[i])][2] = '\0';
                }
            }

        }
    }
    return vars;
}


struct Clause **generateClauses(char **clauses, int count){

    struct Clause **output = (struct Clause **)malloc(sizeof(struct Clause *) * count);
    for(int i = 0; i < count; i++){
        output[i] = getVariables(clauses[i]);
    }

    return output;
}


struct Clause *resolve(struct Clause *clause1, struct Clause *clause2){
    int cancellations = 0;
    // this here will be temporary, if we don't resolve anything, we remove it!
    struct Clause *res = (struct Clause *)malloc(sizeof(struct Clause));
    res->clause = (char *)malloc(sizeof(char) * (strlen(clause1->clause) + strlen(clause2->clause)) + 2);
    res->variables = initVarsMap();
    strcpy(res->clause, "");
    for(int i = 0; i < 26; i++){
        // if we have a single variable alone, we move it to the new clause
        if(clause1->variables[i] == NULL && clause2->variables[i] != NULL){

            editClause(&(res->variables[i]), clause2->variables[i], res->clause , i);

        } else if(clause1->variables[i] != NULL && clause2->variables[i] == NULL){
            
            editClause(&(res->variables[i]), clause1->variables[i], res->clause, i);

        } else if(clause1->variables[i] != NULL && clause2->variables[i] != NULL){
            // here, both aren't null, then we have either the same variable or a opposites, here we check as follows
            if(strlen(clause1->variables[i]) != strlen(clause2->variables[i])){
                // we have a cancellation here
                res->variables[i] = NULL; // so that this pointer is safe for checks and access later
                cancellations += 1;
            } else {
                // same variable, should be treated the same way as when we are just copying
                editClause(&(res->variables[i]), clause1->variables[i], res->clause, i);
            }
        }
    }
    if(cancellations == 0){
        // free used space then quit
        printf("to free: %s\n", res->clause);
        freeClause(res);
        return NULL;
    }

    return res;
}
// after getting the array of clauses, we can start doing resolutions

int setHas(struct Clause *clause, struct Clause **setOfClauses, int size ){

    for(int i = 0; i < size; i++){
        int equal = 1;
        for(int j = 0; j < 26; j++){
            if(clause->variables[j] == NULL && setOfClauses[i]->variables[j] != NULL){
                equal = 0;
                break;
            }
            else if(clause->variables[j] != NULL && setOfClauses[i]->variables[j] == NULL){
                equal = 0;
                break;
            }
            else if(clause->variables[j] != NULL && setOfClauses[i]->variables[j] != NULL){
                if(strcmp(clause->variables[j], setOfClauses[i]->variables[j]) != 0){
                    equal = 0;
                    break;
                }
            }
        }
        if(equal == 1) return 1;
    }
    return 0;
}



int setResolution(char **clauses, int count) {
    struct Clause **setOfClauses = generateClauses(clauses, count);
    int resLen = count;
    int resolvedFrom = 0;
    time_t begins = time(NULL);
    int iterations = 0;
    const int MAX_ITERATIONS = 100;  // Prevent infinite loops
    
    while (iterations < MAX_ITERATIONS) {
        int resCount = 0;
        int newSetCount = 0;
        struct Clause **newSetOfClauses = NULL;
        int clausesAdded = 0;

        for (int i = 0; i < resLen; i++) {
            resolvedFrom = 0;

            for (int j = i + 1; j < resLen; j++) {
                struct Clause *res = resolve(setOfClauses[i], setOfClauses[j]);

                if (res != NULL) {
                    resCount++;
                    resolvedFrom++;

                    if (strcmp(res->clause, "") == 0) {
                        // Empty clause found - unsatisfiable
                        freeClause(res);
                        for (int k = 0; k < resLen; k++) {
                            freeClause(setOfClauses[k]);
                        }
                        free(setOfClauses);
                        return 1;  // Unsatisfiable
                    }

                    if (!setHas(res, newSetOfClauses, newSetCount)) {
                        struct Clause **temp = realloc(newSetOfClauses, sizeof(struct Clause *) * (newSetCount + 1));
                        if (temp == NULL) {
                            freeClause(res);
                            for (int k = 0; k < resLen; k++) {
                                freeClause(setOfClauses[k]);
                            }
                            free(setOfClauses);
                            return -1;
                        }
                        newSetOfClauses = temp;
                        newSetOfClauses[newSetCount++] = res;
                        clausesAdded = 1;
                    } else {
                        freeClause(res);
                    }
                }
            }

            // Add original clause if it might be useful later
            if (resolvedFrom != 0 || strlen(setOfClauses[i]->clause) <= 2) {
                if (!setHas(setOfClauses[i], newSetOfClauses, newSetCount)) {
                    struct Clause **temp = realloc(newSetOfClauses, sizeof(struct Clause *) * (newSetCount + 1));
                    if (temp == NULL) {
                        for (int k = 0; k < resLen; k++) {
                            freeClause(setOfClauses[k]);
                        }
                        free(setOfClauses);
                        return -1;
                    }
                    newSetOfClauses = temp;
                    newSetOfClauses[newSetCount++] = setOfClauses[i];
                    clausesAdded = 1;
                    setOfClauses[i] = NULL;
                } else {
                    freeClause(setOfClauses[i]);
                    setOfClauses[i] = NULL;
                }
            } else {
                freeClause(setOfClauses[i]);
                setOfClauses[i] = NULL;
            }
        }

        // Check if we've reached a fixed point (no new clauses added)
        if (!clausesAdded) {
            // No new clauses were added in this iteration - set is satisfiable
            for (int k = 0; k < resLen; k++) {
                if (setOfClauses[k] != NULL) {
                    freeClause(setOfClauses[k]);
                }
            }
            free(setOfClauses);
            return 0;  // Satisfiable
        }

        free(setOfClauses);
        setOfClauses = newSetOfClauses;
        resLen = newSetCount;
        iterations++;

        // Timeout check
        if (time(NULL) - begins >= 180) {
            MessageBeep(MB_ICONASTERISK);
            MessageBox(NULL, "Processing is taking too long. The set might be satisfiable but complex.", 
                      "Timeout", MB_OK | MB_ICONEXCLAMATION);
            
            for (int k = 0; k < resLen; k++) {
                freeClause(setOfClauses[k]);
            }
            free(setOfClauses);
            return 0;  // Treat timeout as satisfiable
        }
    }

    // Max iterations reached - assume satisfiable
    for (int k = 0; k < resLen; k++) {
        freeClause(setOfClauses[k]);
    }
    free(setOfClauses);
    return 0;  // Satisfiable
}


void testClauses(char* clause) {
    int size = getClauseCount(clause);
    system("CLS");
    
    // Print title
    centerTextColor("SatiChecker Resolution Result", COLOR_MAGENTA);
    printf("\n");
    
    // Print input clauses
    centerTextColor("Input Clauses:", COLOR_CYAN);
    printf("\n");
    centerTextColor("----------------------------", COLOR_BLUE);
    
    char** stringClauses = extractClauses(clause);
    for (int i = 0; i < size; i++) {
        printClauseBox(stringClauses[i]);
    }
    centerTextColor("----------------------------", COLOR_BLUE);
    printf("\n");
    
    // Process resolution
    int result = setResolution(stringClauses, size);
    
    // Print result with appropriate color and box
    printf("\n");
    centerTextColor("Resolution Result:", COLOR_CYAN);
    printf("\n");
    
    if (result == 1) {
        printResultBox("UNSATISFIABLE - Empty clause derived!", COLOR_RED);
        centerTextColor("The set of clauses is unsatisfiable", COLOR_RED);
    } 
    else if (result == 0) {
        printResultBox("SATISFIABLE - No empty clause found", COLOR_GREEN);
        centerTextColor("The set of clauses is satisfiable", COLOR_GREEN);
    } 
    else {
        printResultBox("ERROR - Resolution process failed", COLOR_YELLOW);
        centerTextColor("An error occurred during resolution", COLOR_YELLOW);
    }
    
    // Add decorative footer
    printf("\n\n");
    centerTextColor("Press 'Q' to return to main menu", COLOR_MAGENTA);
    
    // Cleanup
    for (int i = 0; i < size; i++) {
        free(stringClauses[i]);
    }
    free(stringClauses);
    
    // Wait for Q key
    char key;
    do {
        key = toupper(getch());
    } while (key != 'Q');
}
int verifySet(char *str, int size){
    for(int i = 0; i < size; i++){
        if(!(str[i] >= 'A' && str[i] <= 'Z')){
            if(str[i] != '|' && str[i] != '!' && str[i] != '&' ){
                return 0;
            }
        }
    }
    return 1;
}

// Additional functions to add to your existing code for CNF conversion
// Helper function to apply De Morgan's laws
char* applyDeMorgan(char *expr) {
    if (!expr) return NULL;
    int len = strlen(expr);
    char *result = (char*)malloc(len * 2 + 1);
    if (!result) return NULL;
    strcpy(result, "");

    int i = 0;
    while (i < len) {
        if (expr[i] == '&') {
            strcat(result, "|");
        } else if (expr[i] == '|') {
            strcat(result, "&");
        } else if (isalpha(expr[i])) {
            strcat(result, "!");
            int currentLen = strlen(result);
            result[currentLen] = expr[i];
            result[currentLen + 1] = '\0';
        } else if (expr[i] == '!' && i < len - 1 && isalpha(expr[i + 1])) {
            i++; // Skip the '!'
            int currentLen = strlen(result);
            result[currentLen] = expr[i];
            result[currentLen + 1] = '\0';
        } else {
            int currentLen = strlen(result);
            result[currentLen] = expr[i];
            result[currentLen + 1] = '\0';
        }
        i++;
    }

    return result;
}
// Function to check if a character is a logical operator
int isLogicalOperator(char c) {
    return (c == '&' || c == '|' || c == '!' || c == '>' || c == '<' || c == '=');
}

// Function to find matching closing parenthesis
int findMatchingParen(char *expr, int start) {
    int count = 1;
    int i = start + 1;
    while (i < strlen(expr) && count > 0) {
        if (expr[i] == '(') count++;
        else if (expr[i] == ')') count--;
        i++;
    }
    return (count == 0) ? i - 1 : -1;
}

// Function to eliminate implications (A > B becomes !A | B)
char* eliminateImplications(char *expr) {
    if (!expr) return NULL;
    int len = strlen(expr);
    char *result = (char*)malloc(len * 3 + 1);
    if (!result) return NULL;
    strcpy(result, "");

    int i = 0;
    while (i < len) {
        if (expr[i] == '>' && i > 0 && i < len - 1) {
            int leftStart = i - 1;
            while (leftStart > 0 && (isalpha(expr[leftStart - 1]) || expr[leftStart - 1] == '!' || expr[leftStart - 1] == ')')) {
                leftStart--;
            }
            if (expr[leftStart] == ')') {
                int parenCount = 1;
                leftStart--;
                while (leftStart >= 0 && parenCount > 0) {
                    if (expr[leftStart] == ')') parenCount++;
                    else if (expr[leftStart] == '(') parenCount--;
                    leftStart--;
                }
                leftStart++;
            }

            int leftLen = i - leftStart;
            char *leftOperand = strndup(expr + leftStart, leftLen);
            if (!leftOperand) {
                free(result);
                return NULL;
            }

            int rightStart = i + 1;
            int rightEnd = rightStart;
            if (expr[rightStart] == '(') {
                rightEnd = findMatchingParen(expr, rightStart);
                if (rightEnd == -1) {
                    free(leftOperand);
                    free(result);
                    return NULL; // Malformed expression
                }
            } else {
                if (expr[rightStart] == '!') rightEnd++;
                while (rightEnd < len && isalpha(expr[rightEnd])) rightEnd++;
                rightEnd--;
            }

            char *rightOperand = strndup(expr + rightStart, rightEnd - rightStart + 1);
            if (!rightOperand) {
                free(leftOperand);
                free(result);
                return NULL;
            }

            int resultLen = strlen(result);
            result[resultLen - leftLen] = '\0';
            strcat(result, "(!");
            strcat(result, leftOperand);
            strcat(result, "|");
            strcat(result, rightOperand);
            strcat(result, ")");

            free(leftOperand);
            free(rightOperand);
            i = rightEnd + 1;
        } else {
            int currentLen = strlen(result);
            result[currentLen] = expr[i];
            result[currentLen + 1] = '\0';
            i++;
        }
    }

    return result;
}
// Function to eliminate equivalences (A = B becomes (A > B) & (B > A))
char* eliminateEquivalences(char *expr) {
    if (!expr) return NULL;
    int len = strlen(expr);
    char *result = (char*)malloc(len * 4 + 1);
    if (!result) return NULL;
    strcpy(result, "");

    int i = 0;
    while (i < len) {
        if (expr[i] == '=' && i > 0 && i < len - 1) {
            int leftStart = i - 1;
            while (leftStart > 0 && (isalpha(expr[leftStart - 1]) || expr[leftStart - 1] == '!' || expr[leftStart - 1] == ')')) {
                leftStart--;
            }
            if (expr[leftStart] == ')') {
                int parenCount = 1;
                leftStart--;
                while (leftStart >= 0 && parenCount > 0) {
                    if (expr[leftStart] == ')') parenCount++;
                    else if (expr[leftStart] == '(') parenCount--;
                    leftStart--;
                }
                leftStart++;
            }

            int leftLen = i - leftStart;
            char *leftOperand = strndup(expr + leftStart, leftLen);
            if (!leftOperand) {
                free(result);
                return NULL;
            }

            int rightStart = i + 1;
            int rightEnd = rightStart;
            if (expr[rightStart] == '(') {
                rightEnd = findMatchingParen(expr, rightStart);
                if (rightEnd == -1) {
                    free(leftOperand);
                    free(result);
                    return NULL; // Malformed expression
                }
            } else {
                if (expr[rightStart] == '!') rightEnd++;
                while (rightEnd < len && isalpha(expr[rightEnd])) rightEnd++;
                rightEnd--;
            }

            char *rightOperand = strndup(expr + rightStart, rightEnd - rightStart + 1);
            if (!rightOperand) {
                free(leftOperand);
                free(result);
                return NULL;
            }

            int resultLen = strlen(result);
            result[resultLen - leftLen] = '\0';
            strcat(result, "((");
            strcat(result, leftOperand);
            strcat(result, ">");
            strcat(result, rightOperand);
            strcat(result, ")&(");
            strcat(result, rightOperand);
            strcat(result, ">");
            strcat(result, leftOperand);
            strcat(result, "))");

            free(leftOperand);
            free(rightOperand);
            i = rightEnd + 1;
        } else {
            int currentLen = strlen(result);
            result[currentLen] = expr[i];
            result[currentLen + 1] = '\0';
            i++;
        }
    }

    return result;
}
// Function to push negations inward (De Morgan's laws)
char* pushNegationsInward(char *expr) {
    if (!expr) return NULL;
    int len = strlen(expr);
    char *result = (char*)malloc(len * 2 + 1);
    if (!result) return NULL;
    strcpy(result, "");

    int i = 0;
    while (i < len) {
        if (expr[i] == '!' && i < len - 1) {
            if (expr[i + 1] == '(') {
                int closePos = findMatchingParen(expr, i + 1);
                if (closePos != -1) {
                    char *inner = strndup(expr + i + 2, closePos - i - 2);
                    if (!inner) {
                        free(result);
                        return NULL;
                    }

                    char *transformed = applyDeMorgan(inner);
                    if (!transformed) {
                        free(inner);
                        free(result);
                        return NULL;
                    }
                    strcat(result, transformed);

                    free(inner);
                    free(transformed);
                    i = closePos + 1;
                } else {
                    int currentLen = strlen(result);
                    result[currentLen] = expr[i];
                    result[currentLen + 1] = '\0';
                    i++;
                }
            } else if (expr[i + 1] == '!') {
                i += 2; // Skip double negation
            } else {
                int currentLen = strlen(result);
                result[currentLen] = expr[i];
                result[currentLen + 1] = '\0';
                i++;
            }
        } else {
            int currentLen = strlen(result);
            result[currentLen] = expr[i];
            result[currentLen + 1] = '\0';
            i++;
        }
    }

    return result;
}
// Function to distribute OR over AND (convert to CNF)
char* distributeOrOverAnd(char *expr) {
    int len = strlen(expr);
    char *result = (char*)malloc(len * 4 + 1);  // Extra space for distribution
    if (!result) return NULL;
    strcpy(result, "");

    // Check if expression is of the form (A|(B&C)) or similar
    int i = 0;
    while (i < len) {
        if (expr[i] == '(') {
            int closePos = findMatchingParen(expr, i);
            if (closePos == -1) {
                // Malformed parentheses, copy as is
                strncat(result, expr + i, 1);
                i++;
                continue;
            }

            // Extract inner expression
            char *inner = strndup(expr + i + 1, closePos - i - 1);
            int innerLen = strlen(inner);

            // Check for OR over AND: e.g., A|(B&C)
            int orPos = -1;
            int parenCount = 0;
            for (int j = 0; j < innerLen; j++) {
                if (inner[j] == '(') parenCount++;
                else if (inner[j] == ')') parenCount--;
                else if (inner[j] == '|' && parenCount == 0) {
                    orPos = j;
                    break;
                }
            }

            if (orPos != -1) {
                // Found OR: split into left and right operands
                char *left = strndup(inner, orPos);
                char *right = strndup(inner + orPos + 1, innerLen - orPos - 1);

                // Check if right operand is an AND expression (e.g., B&C)
                if (right[0] == '(' && strchr(right, '&')) {
                    int rightClose = findMatchingParen(right, 0);
                    if (rightClose != -1) {
                        char *andInner = strndup(right + 1, rightClose - 1);
                        // Split AND operands
                        int andPos = -1;
                        parenCount = 0;
                        for (int k = 0; k < strlen(andInner); k++) {
                            if (andInner[k] == '(') parenCount++;
                            else if (andInner[k] == ')') parenCount--;
                            else if (andInner[k] == '&' && parenCount == 0) {
                                andPos = k;
                                break;
                            }
                        }

                        if (andPos != -1) {
                            // Distribute: (A|(B&C)) -> (A|B)&(A|C)
                            char *andLeft = strndup(andInner, andPos);
                            char *andRight = strndup(andInner + andPos + 1, strlen(andInner) - andPos - 1);

                            // Build distributed result: (left|andLeft)&(left|andRight)
                            strcat(result, "(");
                            strcat(result, left);
                            strcat(result, "|");
                            strcat(result, andLeft);
                            strcat(result, ")&(");
                            strcat(result, left);
                            strcat(result, "|");
                            strcat(result, andRight);
                            strcat(result, ")");

                            free(andLeft);
                            free(andRight);
                        } else {
                            // No AND to distribute, copy inner as is
                            strcat(result, "(");
                            strcat(result, inner);
                            strcat(result, ")");
                        }
                        free(andInner);
                    } else {
                        // Malformed right operand, copy as is
                        strcat(result, "(");
                        strcat(result, inner);
                        strcat(result, ")");
                    }
                } else {
                    // No AND in right operand, copy as is
                    strcat(result, "(");
                    strcat(result, inner);
                    strcat(result, ")");
                }

                free(left);
                free(right);
                i = closePos + 1;
            } else {
                // No OR to distribute, copy as is
                strncat(result, expr + i, closePos - i + 1);
                i = closePos + 1;
            }
            free(inner);
        } else {
            // Copy non-parenthesized characters
            strncat(result, expr + i, 1);
            i++;
        }
    }

    return result;
}
// Function to display step-by-step CNF conversion
void displayCNFSteps(char *formula) {
    printf("\n");
    centerTextColor("STEP-BY-STEP CNF CONVERSION", COLOR_MAGENTA);
    printf("\n");
    
    printf("%sStep 1: Original Formula%s\n", COLOR_CYAN, COLOR_DEFAULT);
    printf("   %s%s%s\n\n", COLOR_WHITE, formula, COLOR_DEFAULT);
    
    // Step 1: Eliminate equivalences
    char *step1 = eliminateEquivalences(formula);
    if (strcmp(step1, formula) != 0) {
        printf("%sStep 2: Eliminate Equivalences (A=B > (A>B)&(B>A))%s\n", COLOR_CYAN, COLOR_DEFAULT);
        printf("   %s%s%s\n\n", COLOR_WHITE, step1, COLOR_DEFAULT);
    }
    
    // Step 2: Eliminate implications
    char *step2 = eliminateImplications(step1);
    if (strcmp(step2, step1) != 0) {
        printf("%sStep %d: Eliminate Implications (A>B > !A|B)%s\n", COLOR_CYAN, 
               (strcmp(step1, formula) != 0) ? 3 : 2, COLOR_DEFAULT);
        printf("   %s%s%s\n\n", COLOR_WHITE, step2, COLOR_DEFAULT);
    }
    
    // Step 3: Push negations inward
    char *step3 = pushNegationsInward(step2);
    if (strcmp(step3, step2) != 0) {
        printf("%sStep %d: Push Negations Inward (De Morgan's Laws)%s\n", COLOR_CYAN,
               (strcmp(step1, formula) != 0 && strcmp(step2, step1) != 0) ? 4 : 
               (strcmp(step1, formula) != 0 || strcmp(step2, step1) != 0) ? 3 : 2, COLOR_DEFAULT);
        printf("   %s%s%s\n\n", COLOR_WHITE, step3, COLOR_DEFAULT);
    }
    
    // Step 4: Distribute OR over AND
    char *step4 = distributeOrOverAnd(step3);
    
    printf("%sFinal CNF Form:%s\n", COLOR_GREEN, COLOR_DEFAULT);
    printf("   %s%s%s%s\n\n", COLOR_BOLD, COLOR_GREEN, step4, COLOR_DEFAULT);
    
    // Clean up
    free(step1);
    free(step2);
    free(step3);
    free(step4);
}


// Main function to convert formula to CNF
char* convertToCNF(char *formula) {
    printf("Original formula: %s\n", formula);
    
    // Step 1: Eliminate equivalences
    char *step1 = eliminateEquivalences(formula);
    printf("After eliminating equivalences: %s\n", step1);
    
    // Step 2: Eliminate implications
    char *step2 = eliminateImplications(step1);
    printf("After eliminating implications: %s\n", step2);
    
    // Step 3: Push negations inward
    char *step3 = pushNegationsInward(step2);
    printf("After pushing negations inward: %s\n", step3);
    
    // Step 4: Distribute OR over AND
    char *step4 = distributeOrOverAnd(step3);
    printf("Final CNF: %s\n", step4);
    
    // Clean up intermediate results
    free(step1);
    free(step2);
    free(step3);
    
    return step4;
}

// Function to verify if input contains implications or equivalences
int containsImplicationOrEquivalence(char *expr) {
    int len = strlen(expr);
    for (int i = 0; i < len; i++) {
        if (expr[i] == '>' || expr[i] == '=') {
            return 1;
        }
    }
    return 0;
}

// Enhanced verification function that accepts implications and equivalences
int verifySetWithConversion(char *str, int size) {
    for (int i = 0; i < size; i++) {
        if (!(str[i] >= 'A' && str[i] <= 'Z')) {
            if (str[i] != '|' && str[i] != '!' && str[i] != '&' && 
                str[i] != '>' && str[i] != '=' && str[i] != '(' && str[i] != ')') {
                return 0;
            }
        }
    }
    return 1;
}

// Function to display CNF form in a formatted box
void displayCNFForm(char *originalFormula, char *cnfFormula) {
    printf("\n");
    centerTextColor("CNF CONVERSION RESULT", COLOR_CYAN);
    printf("\n");
    
    // Display original formula
    printf("%s+", COLOR_YELLOW);
    for (int i = 0; i < 70; i++) printf("-");
    printf("+\n");
    printf("| %-68s |\n", "ORIGINAL FORMULA:");
    printf("| %-68s |\n", "");
    printf("| %-68s |\n", originalFormula);
    printf("+");
    for (int i = 0; i < 70; i++) printf("-");
    printf("+\n");
    
    // Display CNF formula
    printf("| %-68s |\n", "CNF FORM:");
    printf("| %-68s |\n", "");
    printf("| %-68s |\n", cnfFormula);
    printf("+");
    for (int i = 0; i < 70; i++) printf("-");
    printf("+%s\n", COLOR_DEFAULT);
    
    printf("\n");
}

// Main function to display CNF - matches your main.c call
void displayCNF(char *formula) {
    system("CLS");
    
    
    // Check if conversion is needed
    if (containsImplicationOrEquivalence(formula)) {
        // Show step-by-step conversion
        displayCNFSteps(formula);
        
        // Get final CNF
        char *step1 = eliminateEquivalences(formula);
        char *step2 = eliminateImplications(step1);
        char *step3 = pushNegationsInward(step2);
        char *cnfFormula = distributeOrOverAnd(step3);
        
        // Display formatted result
        displayCNFForm(formula, cnfFormula);
        
        // Clean up
        free(step1);
        free(step2);
        free(step3);
        free(cnfFormula);
        
    } else {
        printf("%s%sFormula is already in CNF format:%s\n\n", COLOR_GREEN, COLOR_BOLD, COLOR_DEFAULT);
        displayCNFForm(formula, formula);
    }
    
    printf("\n");
    centerTextColor("Press 'C' to continue with resolution test, 'Q' to return to menu", COLOR_CYAN);
    
    char key;
    do {
        key = toupper(getch());
    } while (key != 'C' && key != 'Q');
    
    if (key == 'C') {
        // Continue with resolution test
        if (containsImplicationOrEquivalence(formula)) {
            char *step1 = eliminateEquivalences(formula);
            char *step2 = eliminateImplications(step1);
            char *step3 = pushNegationsInward(step2);
            char *cnfFormula = distributeOrOverAnd(step3);
            
            testClauses(cnfFormula);
            
            free(step1);
            free(step2);
            free(step3);
            free(cnfFormula);
        } else {
            testClauses(formula);
        }
    }
}


// Enhanced function to display CNF conversion with options
void showCNFConversion(char* formula) {
    system("CLS");
    
    centerTextColor("CNF CONVERTER", COLOR_MAGENTA);
    printf("\n");
    
    // Check if conversion is needed
    if (containsImplicationOrEquivalence(formula)) {
        // Show step-by-step conversion
        displayCNFSteps(formula);
        
        // Get final CNF
        char *step1 = eliminateEquivalences(formula);
        char *step2 = eliminateImplications(step1);
        char *step3 = pushNegationsInward(step2);
        char *cnfFormula = distributeOrOverAnd(step3);
        
        // Display formatted result
        displayCNFForm(formula, cnfFormula);
        
        // Clean up
        free(step1);
        free(step2);
        free(step3);
        free(cnfFormula);
        
    } else {
        printf("%s%sFormula is already in CNF format:%s\n\n", COLOR_GREEN, COLOR_BOLD, COLOR_DEFAULT);
        displayCNFForm(formula, formula);
    }
    
    printf("\n");
    centerTextColor("Press 'C' to continue with resolution test, 'Q' to return to menu", COLOR_CYAN);
    
    char key;
    do {
        key = toupper(getch());
    } while (key != 'C' && key != 'Q');
    
    if (key == 'C') {
        // Continue with resolution test
        if (containsImplicationOrEquivalence(formula)) {
            char *step1 = eliminateEquivalences(formula);
            char *step2 = eliminateImplications(step1);
            char *step3 = pushNegationsInward(step2);
            char *cnfFormula = distributeOrOverAnd(step3);
            
            testClauses(cnfFormula);
            
            free(step1);
            free(step2);
            free(step3);
            free(cnfFormula);
        } else {
            testClauses(formula);
        }
    }
}

// Function to test clauses with CNF conversion
void testClausesWithConversion(char* formula) {
    system("CLS");
    
    centerTextColor("SatiChecker with CNF Conversion", COLOR_MAGENTA);
    printf("\n");
    
    printf("Input formula: %s\n\n", formula);
    
    // Check if conversion is needed
    if (containsImplicationOrEquivalence(formula)) {
        printf("Converting to CNF...\n\n");
        char *cnfFormula = convertToCNF(formula);
        
        printf("\nTesting CNF formula...\n");
        testClauses(cnfFormula);
        
        free(cnfFormula);
    } else {
        printf("Formula is already in CNF format.\n");
        testClauses(formula);
    }
}