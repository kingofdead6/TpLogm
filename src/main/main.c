#include <stdio.h>
#include <stdlib.h>
#include "../fileHandlers/fileHandlers.h"
#include "../stringHandlers/stringHandlers.h"
#include <string.h>

int main(){
    /*int count = getClauseCount("A|B&B|C|D&!A|!D&P|!R|Q&Q|S");
    //printf("clauses count %d", getClauseCount("A|B&B|C"));
    char **output = extractClauses("A|B&B|C|D&!A|!D&P|!R|Q&Q|S");
    //printf("output %p", output);
    for(int i = 0; i < count ; i++){
        printf("%s\n", output[i]);
    } */
    testClauses("A|!B");
    return 0;
}