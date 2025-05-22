#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

// define a structure with a table that includes all ascii's of the chars, we need ahashmap for that
typedef struct Clause{
    char *clause;
    char **variables;
}Clause;

// helper functions are here
int Hash(char c){
    return c - 'A';
}

char **initVarsMap(){
    char **vars = (char **)malloc(sizeof(char *) * 26);
    for(int i = 0; i < 26; i++) vars[i] = NULL;
    return vars;
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
    free(clause->clause);
    // free the variables
    for(int i = 0; i < 26; i++){
        if(clause->variables[i] != NULL){
            free(clause->variables[i]);
        }
    }
    free(clause->variables);
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
    res->clause = (char *)malloc(sizeof(char) * (strlen(clause1->clause) + strlen(clause2->clause)) + 1);
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
        freeClause(res);
        return NULL;
    }

    return res;
}
// after getting the array of clauses, we can start doing resolutions






int setResolution(char **clauses, int count){
    // iteratively try to find resolutions, until we find a valid clause or run into 0 resolutions
    struct Clause **setOfClauses = generateClauses(clauses, count);
    // length of the set of clauses
    int resLen = count;
    while(1){
        int resCount = 0;
        int newSetCount = 0;
        struct Clause **newSetOfClauses = NULL;
        for(int i = 0; i < resLen; i++){
            for(int j = i + 1; j < resLen; j++){
                struct Clause *res = resolve(setOfClauses[i], setOfClauses[j]);
                if(res != NULL){
                    resCount += 1;
                    if(strcmp(res->clause, "") == 0){
                        // we arrived at a solution, now we need to free all the space we used and quit
                        freeClause(res);
                        for(int  k = 0; k < resLen; k++){
                            freeClause(setOfClauses[k]);
                        }
                        return 1;
                    }
                    // in this case, we add the clause to the new set of clauses we are creating and will be used next
                    // get rid of this clause, as we already used it
                    freeClause(setOfClauses[i]);
                    // pointers safety
                    setOfClauses[i] = NULL;
                    struct Clause **temp = realloc(newSetOfClauses, sizeof(struct Clause *) *(newSetCount + 1));
                    // verify if we failed to reallocate memory
                    if(temp == NULL){
                        // free all memory and quit with error code -1
                        freeClause(res);
                        for(int  k = 0; k < resLen; k++){
                            freeClause(setOfClauses[k]);
                        }
                        return -1;
                    }
                    // assign the new pointer to the new Set of Clauses
                    newSetOfClauses = temp;
                    newSetOfClauses[newSetCount++] = res;

                } else {
                    // meaning we didn't find a resolution for this particular pair of clauses that make any resolution
                    // I believe this part isn't necessary and we just have to continue , I will recheck this later
                }
            }
            if(setOfClauses[i] != NULL){
                // this means that this clause might be used in the next iteration
                    struct Clause **temp = realloc(newSetOfClauses, sizeof(struct Clause *) *(newSetCount + 1));
                    // verify if we failed to reallocate memory
                    if(temp == NULL){
                        // free all memory and quit with error code -1
                        for(int  k = 0; k < resLen; k++){
                            freeClause(setOfClauses[k]);
                        }
                        return -1;
                    }
                    // assign the new pointer to the new Set of Clauses
                    newSetOfClauses = temp;
                    newSetOfClauses[newSetCount++] = setOfClauses[i];
            }
        }

        if(resCount == 0) return 0; // resolution failed, hence we leave and return false
        free(setOfClauses);
        setOfClauses = newSetOfClauses;
        resLen = newSetCount;
        newSetCount = 0;
    }
    
    return 0;
}



void testClauses(char * clause){
    int size = getClauseCount(clause);
    char **stringClauses = extractClauses(clause);
    struct Clause **out = generateClauses(stringClauses, size);
    int result = setResolution(stringClauses,size);
    system("CLS");
    printf("your set of Formulae S:\n{ \n");
    for(int i = 0; i < size; i++){
        printf("%s\n", stringClauses[i]);
    }
    printf("}\n");
    if(result == 0){
        printf("resolution here, can't refute your set of formulae, please consider trying another method, like the semantic tree or truth table\n\n\n");
    } else {
        printf("Your set of formulae have ran into an empty clause, hence your set of clauses is unsatisfiable !!\n\n\n");
    }



}




