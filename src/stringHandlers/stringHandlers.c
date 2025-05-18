#include <stdlib.h>
#include <stdio.h>
#include <string.h>


    // define a structure with a table that includes all ascii's of the chars, we need ahashmap for that
    typedef struct Clause{
        char *clause;
        char **variables;
    }Clause;

int Hash(char c){
    return c - 'A';
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

struct Clause *getVariables(char *clause){
    int n = strlen(clause);
    // initializations here
    struct Clause *vars = (struct Clause *)malloc(sizeof(struct Clause));
    vars->clause = (char *)malloc(sizeof(char ) * n);
    strcpy(vars->clause, clause);
    vars->variables = (char **)malloc(sizeof(char *) * 26);
    // init all strings to null
    for(int i = 0; i < 26; i++) vars->variables[i] = NULL;

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
            printf("char %c, %d\n", clause[i], vars->variables[Hash(clause[i])] != NULL) ;
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

// after getting the array of clauses, we can start doing resolutions



void testClauses(char * clause){
    struct Clause *out = getVariables(clause);
    printf("clause: %s\n", out->clause);
    for(int i = 0; i < 26; i++){
        if(out->variables[i] != NULL){
            printf("%d %s\n",i, out->variables[i]);
        }
    }
}


int setResolution(struct Clause **clauses, int count){
    // iteratively try to find resoltions, until we find a valid clause or run into 0 resolutions
    int resCount = 0;


    
    return 0;
}
