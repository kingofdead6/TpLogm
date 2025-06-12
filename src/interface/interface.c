#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

// Color definitions
#define COLOR_DEFAULT "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_BOLD "\033[1m"
#define COLOR_UNDERLINE "\033[4m"

// Helper function to center text with color
void centerTextColor(const char *text, const char *color) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int len = strlen(text);
    int pos = (width - len) / 2;
    if (pos < 0) pos = 0;
    printf("%s%*s%s%s\n", color, pos, "", text, COLOR_DEFAULT);
}

void centerText(const char *text) {
    centerTextColor(text, COLOR_WHITE);
}

// ASCII Art for the title
void printTitleArt() {
    printf(" ________  ________  _________  ___  ________  ___  ___  _______   ________  ___  __    _______   ________     \n", COLOR_RED);
    printf("|\\   ____\\|\\   __  \\|\\___   ___\\\\  \\|\\   ____\\|\\  \\|\\  \\|\\  ___ \\ |\\   ____\\|\\  \\|\\  \\ |\\  ___ \\ |\\   __  \\    \n", COLOR_RED);
    printf("\\ \\  \\___|\\ \\  \\|\\  \\|___ \\  \\_\\ \\  \\ \\  \\___|\\ \\  \\\\\\  \\ \\   __/|\\ \\  \\___|\\ \\  \\/  /|\\ \\   __/|\\ \\  \\|\\  \\   \n", COLOR_RED);
    printf(" \\ \\_____  \\ \\   __  \\   \\ \\  \\ \\ \\  \\ \\  \\    \\ \\   __  \\ \\  \\_|/_\\ \\  \\    \\ \\   ___  \\ \\  \\_|/_\\ \\   _  _\\  \n", COLOR_RED);
    printf("  \\|____|\\  \\ \\  \\ \\  \\   \\ \\  \\ \\ \\  \\ \\  \\____\\ \\  \\ \\  \\ \\  \\_|\\ \\ \\  \\____\\ \\  \\\\ \\  \\ \\  \\_|\\ \\ \\  \\\\  \\| \n", COLOR_RED);
    printf("    ____\\_\\  \\ \\__\\ \\__\\   \\ \\__\\ \\ \\__\\ \\_______\\ \\__\\ \\__\\ \\_______\\ \\_______\\ \\__\\\\ \\__\\ \\_______\\ \\__\\\\ _\\ \n", COLOR_RED);
    printf("   |\\_________\\|__|\\|__|    \\|__|  \\|__|\\|_______|\\|__|\\|__|\\|_______|\\|_______|\\|__| \\|__|\\|_______|\\|__|\\|__|\n", COLOR_RED);
    printf("   \\|_________|                                                                                               \n", COLOR_RED);
    printf("\n" ,COLOR_RED);
}





// Display team members splash screen with fancy design
void showTeamSplash() {
    system("CLS");
    printf("\n\n\n");
    printTitleArt();
    
    centerTextColor("============================================", COLOR_MAGENTA);
    centerTextColor("          DEVELOPED BY TEAM LOGIC           ", COLOR_YELLOW);
    centerTextColor("============================================", COLOR_MAGENTA);
    
    printf("\n");
    centerTextColor("|==========================================|", COLOR_GREEN);
    centerTextColor("|                                          |", COLOR_GREEN);
    centerTextColor("|           TEAM MEMBERS                   |", COLOR_YELLOW);
    centerTextColor("|                                          |", COLOR_GREEN);
    centerTextColor("|   1. Mekentichi NejemEddine              |", COLOR_CYAN);
    centerTextColor("|   2. Kahlouche Youcef                    |", COLOR_CYAN);
    centerTextColor("|   3. Benbada Ayoub                       |", COLOR_CYAN);
    centerTextColor("|   4. Bouderbala Heythem                  |", COLOR_CYAN);
    centerTextColor("|                                          |", COLOR_GREEN);
    centerTextColor("|==========================================|", COLOR_GREEN);
    
    printf("\n\n");
    centerTextColor("Press any key to enter the solver...", COLOR_WHITE);
    centerTextColor("> > >", COLOR_RED);
    getch();
}

void arrow(int position, int index) {
    if (position == index) {
        printf(COLOR_RED COLOR_BOLD "  " COLOR_DEFAULT);
    } else {
        printf("   ");
    }
}

void printMenuBoxTop() {
    centerTextColor("|==========================================|", COLOR_BLUE);
}

void printMenuBoxBottom() {
    centerTextColor("|==========================================|", COLOR_BLUE);
}

void printMenuLine(const char *text, int selected) {
    char line[100];
    sprintf(line, "| %-40s |", text);
    if (selected) {
        centerTextColor(line, COLOR_RED COLOR_BOLD);
    } else {
        centerTextColor(line, COLOR_WHITE);
    }
}

int Interface() {
    int index = 0;
    char key = 0;
    
    while (key != 13) {
        system("CLS");
        printf("\n");
        printTitleArt();
        centerTextColor("MAIN MENU", COLOR_YELLOW);
        printf("\n");
        printMenuBoxTop();
        printMenuLine("", 0);
        printMenuLine("1. Check set satisfiability using resolution", index == 0);
        printMenuLine("2. Convert formula to CNF", index == 1);
        printMenuLine("3. Read documentation", index == 2);
        printMenuLine("4. How our algorithm works", index == 3);
        printMenuLine("5. Quit program", index == 4);
        printMenuLine("", 0);
        printMenuBoxBottom();
        printf("\n");
        centerTextColor("Use arrows to navigate, Enter to select", COLOR_MAGENTA);
        key = getch();
        if (key == 0 || key == 0xE0) {
            key = getch();
        }
        if (key == 80) {
            index = (index + 1) % 5;
        } else if (key == 72) {
            index = (index - 1 + 5) % 5;
        }
    }
    return index;
}

void printSectionHeader(const char *title) {
    printf("\n");
    centerTextColor("==========================================", COLOR_BLUE);
    centerTextColor(title, COLOR_YELLOW COLOR_BOLD);
    centerTextColor("==========================================", COLOR_BLUE);
    printf("\n");
}

void Documentation() {
    system("CLS");
    printf("\n");
    printTitleArt();
    printSectionHeader("DOCUMENTATION");
    centerTextColor("To process a set of clauses or convert a formula to CNF:", COLOR_WHITE);
    printf("\n");
    centerTextColor(" " COLOR_BOLD "File Format Requirements:" COLOR_DEFAULT, COLOR_CYAN);
    centerTextColor("- Write the formula in a .txt file without curly brackets", COLOR_WHITE);
    centerTextColor("- For resolution: Use clauses in conjunctive normal form (e.g., c1 & c2 & c3)", COLOR_WHITE);
    centerTextColor("- For CNF conversion: Use propositional variables and operators, with parentheses for grouping", COLOR_WHITE);
    printf("\n");
    centerTextColor(" " COLOR_BOLD "Allowed Operators:" COLOR_DEFAULT, COLOR_CYAN);
    centerTextColor("AND: &     OR: |     NOT: !     IMPLIES: >     EQUIVALENCE: =", COLOR_GREEN);
    printf("\n");
    centerTextColor(" " COLOR_BOLD "Propositional Variables:" COLOR_DEFAULT, COLOR_CYAN);
    centerTextColor("- Must be uppercase English letters (A-Z)", COLOR_WHITE);
    centerTextColor("- Examples: P, !P, P|Q, P&Q, (A>B), (A=B)", COLOR_WHITE);
    printf("\n");
    centerTextColor(" " COLOR_BOLD "Examples of Valid Inputs:" COLOR_DEFAULT, COLOR_CYAN);
    centerTextColor("Resolution: A|B|!C & D|!E & F", COLOR_GREEN);
    centerTextColor("CNF Conversion: (A>B)&(B>C)", COLOR_GREEN);
    centerTextColor("CNF Conversion: !(A&B)|(C=D)", COLOR_GREEN);
    printf("\n");
    centerTextColor(" " COLOR_BOLD "Input Notes:" COLOR_DEFAULT, COLOR_CYAN);
    centerTextColor("- Use parentheses to group expressions for CNF conversion", COLOR_WHITE);
    centerTextColor("- Invalid characters or unbalanced parentheses will be rejected", COLOR_WHITE);
    printf("\n\n");
    centerTextColor(COLOR_BOLD "Press " COLOR_RED "'Q'" COLOR_BOLD " to return to main menu" COLOR_DEFAULT, COLOR_YELLOW);
    char key;
    do {
        key = toupper(getch());
    } while (key != 'Q');
}

void howItWorks() {
    system("CLS");
    printf("\n");
    printTitleArt();
    printSectionHeader("ALGORITHM WORKFLOW");
    centerTextColor(" " COLOR_BOLD "Resolution Process:" COLOR_DEFAULT, COLOR_CYAN);
    centerTextColor("1. Parse input clauses into propositional variables", COLOR_WHITE);
    centerTextColor("2. Identify complementary literals between clauses (e.g., A and !A)", COLOR_WHITE);
    centerTextColor("3. Generate resolvents by removing complementary pairs", COLOR_WHITE);
    centerTextColor("4. Add unique resolvents to the working set", COLOR_WHITE);
    printf("\n");
    centerTextColor(" " COLOR_BOLD "CNF Conversion Process:" COLOR_DEFAULT, COLOR_CYAN);
    centerTextColor("1. Eliminate equivalences (A=B > (A>B)&(B>A))", COLOR_WHITE);
    centerTextColor("2. Eliminate implications (A>B > !A|B)", COLOR_WHITE);
    centerTextColor("3. Push negations inward using De Morgan's laws (e.g., !(A&B) → !A|!B)", COLOR_WHITE);
    centerTextColor("4. Distribute OR over AND (e.g., (A|(B&C)) > (A|B)&(A|C))", COLOR_WHITE);
    printf("\n");
    centerTextColor(" " COLOR_BOLD "Termination Conditions:" COLOR_DEFAULT, COLOR_CYAN);
    centerTextColor("- Empty clause derived: Set is UNSATISFIABLE", COLOR_RED);
    centerTextColor("- No new resolvents generated: Set is SATISFIABLE", COLOR_GREEN);
    centerTextColor("- Timeout (3 minutes): Likely SATISFIABLE", COLOR_YELLOW);
    printf("\n");
 
    centerTextColor(COLOR_BOLD "Press " COLOR_RED "'Q'" COLOR_BOLD " to return to main menu" COLOR_DEFAULT, COLOR_YELLOW);
    char key;
    do {
        key = toupper(getch());
    } while (key != 'Q');
}