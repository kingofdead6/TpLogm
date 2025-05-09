// here we shall define procs for opening and closing files, as well as assigning the file to a string
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int openFile(FILE *textFile, char *filename){
    textFile = fopen(filename, "r");
    if(textFile == NULL){
        // file not found
        return -1;
    }
    // success
    return 1;
}

int closeFile(FILE *textFile){
    if(textFile == NULL){
        return -1;
    }
    fclose(textFile);
    // success
    return 1;
}

char *extractStringOfClauses(FILE *textFile){
    char *output = NULL;
    char buffer[1024];
    // the file is supposed to have only one


    return output;
}