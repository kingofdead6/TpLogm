#include "./stringHandlers.c"


int Hash(char c);

int getClauseCount(char *clauses);

char **extractClauses(char *str);

struct Clause *getVariables(char *Clause);

void testClauses(char * clause);

void freeClause(struct Clause *clause);

void editClause(char **variable, char *prop, char *clause, int index);

struct Clause **generateClauses(char **clauses, int count);

int setResolution(char **clauses, int count);

struct Clause *resolve(struct Clause *clause1, struct Clause *clause2);

int verifySet(char *str, int size);

char* convertToCNF(char *formula);

int verifySetWithConversion(char *str, int size) ;

void displayCNF(char *formula) ;
char* applyDeMorgan(char *expr);
void displayCNFSteps(char *formula);