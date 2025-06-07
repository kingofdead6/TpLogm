#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
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



int setResolution(char **clauses, int count){
    // iteratively try to find resolutions, until we find a valid clause or run into 0 resolutions
    struct Clause **setOfClauses = generateClauses(clauses, count);
    // length of the set of clauses
    int resLen = count;
    int resolvedFrom = 0;
    time_t begins = time(NULL);
    while(1){
        int resCount = 0;
        int newSetCount = 0;
        struct Clause **newSetOfClauses = NULL;

        for(int i = 0; i < resLen; i++){
            resolvedFrom = 0;

            for(int j = i + 1; j < resLen; j++){

                struct Clause *res = resolve(setOfClauses[i], setOfClauses[j]);

                if(res != NULL){
                    resCount += 1;
                    resolvedFrom += 1;

                    if(strcmp(res->clause, "") == 0){
                        // we arrived at a solution, now we need to free all the space we used and quit

                        freeClause(res);
                        res = NULL;
                        for(int  k = 0; k < resLen; k++){
                            if(setOfClauses[k] != NULL){

                                freeClause(setOfClauses[k]);
                                setOfClauses[k] = NULL;    
                            }
                        }
                        return 1;
                    }
                    if(setHas(res, newSetOfClauses, newSetCount) == 0){
                        printf("not present\n");
                        // we assign the new clause to the new set, we only remove the ith clause after testing it against all the following clauses
                        struct Clause **temp = realloc(newSetOfClauses, sizeof(struct Clause *) *(newSetCount + 1));
                        // verify if we failed to reallocate memory
                        if(temp == NULL){
                            // free all memory and quit with error code -1
                            if(res != NULL){

                                res = NULL;
                            }

                            for(int  k = 0; k < resLen; k++){

                                freeClause(setOfClauses[k]);
                                setOfClauses[k] = NULL;
                            }
                            return -1;
                        }
                        // assign the new pointer to the new Set of Clauses
                        newSetOfClauses = temp;
                        newSetOfClauses[newSetCount++] = res;                        
                        } else {
                            freeClause(res);
                            res = NULL;
                        }

                } 
                // if we didn't find a resolution for this particular pair of clauses that make any resolution
                // I believe this part isn't necessary and we just have to continue , I will recheck this later
    
            }
            if(resolvedFrom != 0 || strlen(setOfClauses[i]->clause) <= 2){
                // this means that this clause might be used in the next iteration
                if(setHas(setOfClauses[i], newSetOfClauses, newSetCount) == 0){

                    struct Clause **temp = realloc(newSetOfClauses, sizeof(struct Clause *) *(newSetCount + 1));
                    // verify if we failed to reallocate memory
                    if(temp == NULL){
                        // free all memory and quit with error code -1
                        for(int  k = 0; k < resLen; k++){
                            if(setOfClauses[k] != NULL){
                                printf("clause to free: %s\n", setOfClauses[k]->clause);
                                Sleep(1000);
                                freeClause(setOfClauses[k]);
                            }
                        }
                        for(int k = 0; k < newSetCount; k++){
                            if(newSetOfClauses[k] != NULL){

                                freeClause(newSetOfClauses[k]);
                            }
                        }
                        return -1;
                    }
                    // assign the new pointer to the new Set of Clauses
                    newSetOfClauses = temp;
                    newSetOfClauses[newSetCount++] = setOfClauses[i];
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

        if(resCount == 0) return 0; // resolution failed, hence we leave and return false
        free(setOfClauses);
        setOfClauses = newSetOfClauses;
        resLen = newSetCount;
        
        newSetCount = 0;
        time_t now = time(NULL);
        if(now - begins >= 180){
            MessageBeep(MB_ICONASTERISK);
            MessageBox(NULL, "It looks like processing this set is taking too long, it might be due to the size, or just because we ran into an infinite loop","DISCLAIMER", MB_OK | MB_ICONEXCLAMATION);
            
            MessageBeep(MB_ICONASTERISK);
            if(MessageBox(NULL, "Do you want to continue","Question", MB_YESNO) == IDNO){
                return -1;
            } else {
                now = time(NULL);
            }
        }
    }
    
    return 0;
}



void testClauses(char * clause){
    int size = getClauseCount(clause);
    system("CLS");
    char **stringClauses = extractClauses(clause);
    struct Clause **out = generateClauses(stringClauses, size);
    int result = setResolution(stringClauses,size);
    if(result == -1){
        printf("Inner issue, try again later!\n");
        return;
    }
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


    printf("press \'q\' to quit\n");
    char key = getch();
    while(toupper(key) != 'Q'){
        key = getch();
    }
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