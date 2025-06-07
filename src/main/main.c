#include <stdio.h>
#include <stdlib.h>
#include "../fileHandlers/fileHandlers.h"
#include "../stringHandlers/stringHandlers.h"
#include "../interface/interface.h"
#include <string.h>
#include <windows.h>
int main(){

    do{
        int option = Interface();
        char fileName[256];
        FILE *file = NULL;
        switch (option) {

        case 1:
            system("CLS");
            Documentation();
            break;
        case 0:
            printf("Enter the file name or path with extension: ");
            fgets(fileName,256,stdin);
            fileName[strlen(fileName) - 1] = '\0';
            // now we open the file
            
            if(openFile(&file,fileName) != 1){
                printf("Something went wrong, couldn't open file \n\n");
                break;
            }

            char *output = extractStringOfClauses(file);
            printf("%s", output);
            if(verifySet(output, strlen(output)) != 1){
                printf("invalid syntax !\n\n");
                break;
            }

            testClauses(output);

            closeFile(file);
            break;
        case 2:
            system("CLS");
            howItWorks();
            break;
        default:
            goto end;
            break;
        }
        MessageBeep(0);
    } while (MessageBox(NULL, "Do you want to continue?", "Question", MB_YESNO | MB_ICONQUESTION) == IDYES);
    
   
end:    system("CLS");
    return 0;
}