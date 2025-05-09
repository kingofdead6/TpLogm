#include "./stringHandlers.c"


int Hash(char c);

int getClauseCount(char *clauses);

char **extractClauses(char *str);

struct Clause *getVariables(char *Clause);

void testClauses(char * clause);