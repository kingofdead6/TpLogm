#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h> // Added for _getch
#include <string.h>
#include "../stringHandlers/stringHandlers.h"
#include "interface.h"

// Console width for centering (adjust if needed)
#define CONSOLE_WIDTH 80
#define MAX_INPUT 256

// Set console text color
void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Center text in console
void printCentered(const char *text, int color) {
    int len = strlen(text);
    int padding = (CONSOLE_WIDTH - len) / 2;
    setConsoleColor(color);
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", text);
    setConsoleColor(15); // Reset to white
}

// Clear console screen
void clearScreen() {
    system("cls");
}

// Display loading animation
void showLoadingAnimation() {
    const char *spinner = "|/-\\";
    for (int i = 0; i < 10; i++) {
        clearScreen();
        char anim[50];
        sprintf(anim, "Processing %c", spinner[i % 4]);
        printCentered("=====================================", 9);
        printCentered("       Clause Extractor 3000         ", 9);
        printCentered("=====================================", 9);
        printCentered(anim, 13);
        Sleep(100);
    }
}

// Display menu with highlighted selection
void displayMenu(int selected) {
    clearScreen();
    printCentered("=====================================", 9); // Blue
    printCentered("       Clause Extractor 3000         ", 9);
    printCentered("=====================================", 9);
    printCentered(" ", 15);
    char option1[50] = "1. Extract Clauses";
    char option2[50] = "2. Test Clauses";
    char option3[50] = "3. Exit";
    if (selected == 0) strcpy(option1, "> 1. Extract Clauses <");
    if (selected == 1) strcpy(option2, "> 2. Test Clauses <");
    if (selected == 2) strcpy(option3, "> 3. Exit <");
    printCentered(option1, selected == 0 ? 13 : 15); // Magenta if selected
    printCentered(option2, selected == 1 ? 13 : 15);
    printCentered(option3, selected == 2 ? 13 : 15);
    printCentered(" ", 15);
    printCentered("Use UP/DOWN arrows, ENTER to select, ESC to exit", 14); // Yellow
}

// Get keyboard input for navigation
int getMenuChoice() {
    int selected = 0;
    int key;
    while (1) {
        displayMenu(selected);
        key = _getch(); // Non-blocking key input
        if (key == 224) { // Arrow key prefix
            key = _getch();
            if (key == 72 && selected > 0) selected--; // Up arrow
            if (key == 80 && selected < 2) selected++; // Down arrow
        } else if (key == 13) { // Enter
            return selected + 1;
        } else if (key == 27) { // Esc
            return 3; // Exit
        }
    }
}

// Display boxed input prompt
void displayInputPrompt(char *input) {
    clearScreen();
    printCentered("=====================================", 9);
    printCentered("       Enter Clause String           ", 9);
    printCentered("=====================================", 9);
    printCentered(" ", 15);
    printCentered("╔════════════════════════════════╗", 10); // Green box
    char buffer[50];
    sprintf(buffer, "║ %-30s ║", input[0] ? input : "e.g., A|B&B|C");
    printCentered(buffer, 10);
    printCentered("╚════════════════════════════════╝", 10);
    printCentered(" ", 15);
    printCentered("Press ENTER to submit, ESC to cancel", 14);
}

// Main interface function
int Interface() {
    int choice;
    char input[MAX_INPUT];
    int continueProgram = 1;

    while (continueProgram) {
        choice = getMenuChoice();
        showLoadingAnimation();

        switch (choice) {
            case 1: // Extract Clauses
                input[0] = '\0';
                while (1) {
                    displayInputPrompt(input);
                    int ch, i = 0;
                    while ((ch = _getch()) != 13 && ch != 27 && i < MAX_INPUT - 1) {
                        if (ch == 8 && i > 0) { // Backspace
                            input[--i] = '\0';
                            displayInputPrompt(input);
                        } else if (ch >= 32 && ch <= 126) { // Printable chars
                            input[i++] = ch;
                            input[i] = '\0';
                            displayInputPrompt(input);
                        }
                    }
                    if (ch == 13) break; // Enter
                    if (ch == 27) goto cancel; // Esc

                    if (strlen(input) == 0) {
                        printCentered("Error: Empty input!", 12); // Red
                        MessageBeep(MB_ICONERROR);
                        Sleep(1000);
                        continue;
                    }

                    int count = getClauseCount(input);
                    if (count <= 0) {
                        printCentered("Error: Invalid clause string!", 12);
                        MessageBeep(MB_ICONERROR);
                        Sleep(1000);
                        continue;
                    }

                    showLoadingAnimation();
                    char **clauses = extractClauses(input);
                    clearScreen();
                    printCentered("=====================================", 9);
                    printCentered("       Clause Extraction Results     ", 9);
                    printCentered("=====================================", 9);
                    printCentered(" ", 15);
                    char result[50];
                    sprintf(result, "Found %d clauses:", count);
                    printCentered(result, 10);
                    for (int i = 0; i < count; i++) {
                        sprintf(result, "Clause %d: %s", i + 1, clauses[i]);
                        printCentered(result, 15);
                        free(clauses[i]);
                    }
                    free(clauses);
                    MessageBeep(MB_ICONASTERISK);
                    printCentered(" ", 15);
                    printCentered("Press ENTER to continue...", 14);
                    while (_getch() != 13);
                    break;
                }
                break;

            case 2: // Test Clauses
                input[0] = '\0';
                while (1) {
                    displayInputPrompt(input);
                    int ch, i = 0;
                    while ((ch = _getch()) != 13 && ch != 27 && i < MAX_INPUT - 1) {
                        if (ch == 8 && i > 0) {
                            input[--i] = '\0';
                            displayInputPrompt(input);
                        } else if (ch >= 32 && ch <= 126) {
                            input[i++] = ch;
                            input[i] = '\0';
                            displayInputPrompt(input);
                        }
                    }
                    if (ch == 13) break; // Enter
                    if (ch == 27) goto cancel; // Esc

                    if (strlen(input) == 0) {
                        printCentered("Error: Empty input!", 12);
                        MessageBeep(MB_ICONERROR);
                        Sleep(1000);
                        continue;
                    }

                    showLoadingAnimation();
                    clearScreen();
                    printCentered("=====================================", 9);
                    printCentered("       Clause Testing Results        ", 9);
                    printCentered("=====================================", 9);
                    printCentered(" ", 15);
                    printCentered("Testing clauses...", 10);
                    testClauses(input);
                    MessageBeep(MB_ICONASTERISK);
                    printCentered(" ", 15);
                    printCentered("Press ENTER to continue...", 14);
                    while (_getch() != 13);
                    break;
                }
                break;

            case 3: // Exit
                if (MessageBox(NULL, "Do you want to exit?", "Question", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                    clearScreen();
                    printCentered("=====================================", 9);
                    printCentered("  Thank you for using Clause Extractor 3000!  ", 9);
                    printCentered("=====================================", 9);
                    setConsoleColor(13);
                    Sleep(1000);
                    continueProgram = 0;
                }
                break;
        }
cancel:
        continue;
    }

    return 0;
}