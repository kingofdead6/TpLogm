
// Updated main.c with proper error handling for CNF conversion
#include <stdio.h>
#include <stdlib.h>
#include "../fileHandlers/fileHandlers.h"
#include "../stringHandlers/stringHandlers.h"
#include "../interface/interface.h"
#include <string.h>
#include <windows.h>

int main() {
    system("");
    showTeamSplash();
    
    do {
        int option = Interface();
        char fileName[256];
        FILE *file = NULL;
        
        switch (option) {
            case 0: {
                system("CLS");
                printf("\n");
                printTitleArt();
                printSectionHeader("FILE INPUT - RESOLUTION");
                
                centerTextColor(" " COLOR_BOLD "Enter file path:" COLOR_DEFAULT, COLOR_CYAN);
                printf("\n   ");
                fgets(fileName, 256, stdin);
                fileName[strlen(fileName) - 1] = '\0';
                
                if (openFile(&file, fileName) != 1) {
                    printf("\n");
                    centerTextColor(" Error: Couldn't open file!", COLOR_RED);
                    Sleep(2000);
                    break;
                }

                char *output = extractStringOfClauses(file);
                
                // Check if it needs CNF conversion first
                if (containsImplicationOrEquivalence(output)) {
                    printf("\n");
                    centerTextColor(" Formula contains implications/equivalences.", COLOR_YELLOW);
                    centerTextColor(" Converting to CNF first...", COLOR_YELLOW);
                    Sleep(1500);
                    
                    char *cnfOutput = convertToCNF(output);
                    
                    if (verifySet(cnfOutput, strlen(cnfOutput)) != 1) {
                        printf("\n");
                        centerTextColor(" Error: Invalid syntax after CNF conversion!", COLOR_RED);
                        Sleep(2000);
                        free(output);
                        free(cnfOutput);
                        closeFile(file);
                        break;
                    }
                    
                    testClauses(cnfOutput);
                    free(cnfOutput);
                } else {
                    // Standard verification for already CNF formulas
                    if (verifySet(output, strlen(output)) != 1) {
                        printf("\n");
                        centerTextColor(" Error: Invalid syntax in input file!", COLOR_RED);
                        Sleep(2000);
                        free(output);
                        closeFile(file);
                        break;
                    }
                    
                    testClauses(output);
                }
                
                free(output);
                closeFile(file);
                break;
            }
            case 1: {
                system("CLS");
                printf("\n");
                printTitleArt();
                printSectionHeader("CNF CONVERSION");
                
                centerTextColor(" " COLOR_BOLD "Enter file path:" COLOR_DEFAULT, COLOR_CYAN);
                printf("\n   ");
                fgets(fileName, 256, stdin);
                fileName[strlen(fileName) - 1] = '\0';
                
                if (openFile(&file, fileName) != 1) {
                    printf("\n");
                    centerTextColor(" Error: Couldn't open file!", COLOR_RED);
                    Sleep(2000);
                    break;
                }

                char *output = extractStringOfClauses(file);
                
                // Verify with extended syntax for CNF conversion
                if (verifySetWithConversion(output, strlen(output)) != 1) {
                    printf("\n");
                    centerTextColor(" Error: Invalid syntax in input file!", COLOR_RED);
                    centerTextColor(" Check documentation for allowed operators.", COLOR_YELLOW);
                    Sleep(3000);
                    free(output);
                    closeFile(file);
                    break;
                }
                
                displayCNF(output);
                free(output);
                closeFile(file);
                break;
            }
            case 2:
                Documentation();
                break;
            case 3:
                howItWorks();
                break;
            default:
                goto end;
        }
    } while (MessageBox(NULL, "Do you want to continue?", "Question", MB_YESNO | MB_ICONQUESTION) == IDYES);
    
end:
    system("CLS");
    printf("\n\n");
    printTitleArt();
    centerTextColor("Thank you for using our Sets Solver!", COLOR_YELLOW);
    centerTextColor("Goodbye!", COLOR_CYAN);
    Sleep(2000);
    return 0;
}
