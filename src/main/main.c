#include <stdio.h>
#include <stdlib.h>
#include "../interface/interface.h"
#include <string.h>
#include <windows.h>
int main(){
    /*int count = getClauseCount("A|B&B|C|D&!A|!D&P|!R|Q&Q|S");
    //printf("clauses count %d", getClauseCount("A|B&B|C"));
    char **output = extractClauses("A|B&B|C|D&!A|!D&P|!R|Q&Q|S");
    //printf("output %p", output);
    for(int i =  0; i < count ; i++){
        printf("%s\n", output[i]);
    } */

    //testClauses("A|B&!A|C&!B|C&!C|D&!D");
    //MessageBeep(MB_ICONASTERISK);



    //MessageBox(NULL,"Do you want to continue", "Question", MB_YESNO | MB_ICONQUESTION);
   // WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
   int option = Interface();

    return 0;
}