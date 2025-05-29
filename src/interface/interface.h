#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include "../stringHandlers/stringHandlers.h"

// Function to set console text color
void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Function to clear console screen
void clearScreen() {
    system("cls");
}

// Function to display the main menu
void displayMenu() {
    clearScreen();
    setConsoleColor(11); // Cyan color
    printf("=====================================\n");
    printf("       Clause Extractor 3000         \n");
    printf("=====================================\n");
    setConsoleColor(15); // White color
    printf("1. Extract Clauses : \n");
    printf("2. Test Clauses : \n");
    printf("3. Exit\n");
    setConsoleColor(14); // Yellow color
    printf("\nEnter your choice (1-3): ");
    setConsoleColor(15); // Reset to white
}

// Main interface function
int Interface() {
    int choice;
    char input[256];
    int continueProgram = 1;
    const char *defaultExtract = "A|B&B|C|D&!A|!D&P|!R|Q&Q|S";
    const char *defaultTest = "A|B&!A|C&!B|C&!C|D&!D";

    while (continueProgram) {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Clear newline from input buffer

        switch (choice) {
            case 1: // Extract Clauses
                clearScreen();
                setConsoleColor(10); // Green color
                printf("Enter a clause string (press Enter for default: %s): ", defaultExtract);
                setConsoleColor(15);
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0; // Remove trailing newline

                // Use default if input is empty
                if (strlen(input) == 0) {
                    strcpy(input, defaultExtract);
                }

                int count = getClauseCount(input);
                if (count <= 0) {
                    setConsoleColor(12); // Red color
                    printf("Error: Invalid clause string!\n");
                    setConsoleColor(15);
                    MessageBeep(MB_ICONERROR);
                    Sleep(1000);
                    break;
                }

                char **clauses = extractClauses(input);
                setConsoleColor(10);
                printf("\nFound %d clauses:\n", count);
                setConsoleColor(15);
                for (int i = 0; i < count; i++) {
                    printf("Clause %d: %s\n", i + 1, clauses[i]);
                    free(clauses[i]); // Free each clause
                }
                free(clauses); // Free the array
                MessageBeep(MB_ICONASTERISK);

                setConsoleColor(14);
                printf("\nPress Enter to continue...");
                setConsoleColor(15);
                getchar();
                break;

            case 2: // Test Clauses
                clearScreen();
                setConsoleColor(10);
                printf("Enter a clause string to test (press Enter for default: %s): ", defaultTest);
                setConsoleColor(15);
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;

                // Use default if input is empty
                if (strlen(input) == 0) {
                    strcpy(input, defaultTest);
                }

                setConsoleColor(10);
                printf("\nTesting clauses...\n");
                setConsoleColor(15);
                testClauses(input);
                MessageBeep(MB_ICONASTERISK);

                setConsoleColor(14);
                printf("\nPress Enter to continue...");
                setConsoleColor(15);
                getchar();
                break;

            case 3: // Exit
                if (MessageBox(NULL, "Do you want to continue", "Question", MB_YESNO | MB_ICONQUESTION) == IDNO) {
                    clearScreen();
                    setConsoleColor(13); // Magenta color
                    printf("Thank you for using Clause Extractor 3000!\n");
                    setConsoleColor(15);
                    continueProgram = 0;
                    Sleep(1000);
                }
                break;
        }
    }

    return 0;
}

#endif