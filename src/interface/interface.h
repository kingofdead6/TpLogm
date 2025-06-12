#include "./interface.c"
#include <stdio.h>
#include <stdlib.h>
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

// Function prototypes
void centerTextColor(const char *text, const char *color);
void centerText(const char *text);
void printTitleArt();
void showTeamSplash();
void arrow(int position, int index);
void printMenuBoxTop();
void printMenuBoxBottom();
void printMenuLine(const char *text, int selected);
int Interface();
void printSectionHeader(const char *title);
void Documentation();
void howItWorks();

