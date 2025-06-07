

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>
void arrow(int position, int index){
    if(position == index){
        printf("\033[31;1;4m");
    } else {
        printf("\033[0m");
    }
}


int Interface(){
    int index = 1;
    char key = 0;
    while(key != 13){
        system("CLS");
        printf("\033[0m");
        printf("                                 WELCOME TO SETS SOLVER BY RESOLUTIONS                                        \n");
        printf("                                 MAKE SURE TO READ THE DOCUMENTATION BEFORE PROCEEDING                                   \n\n\n");
        printf("-----------------------------------------------------------------------------------------------------------------\n\n");
        arrow(0, index); printf("1 - Check if a set is satisfiable using resolution \n");
        arrow(1, index); printf("2 - Read documentation \n");
        arrow(2, index); printf("3 - How does our algorithm work? \n");
        arrow(3, index); printf("4 - QUIT \n");
        key = getch();

        if(key == 80){
            index += 1;
            if(index  > 3){
                index = 0;
            }
        } else if(key == 72){
            index -= 1;
            if(index < 0){
                index = 3;
            }
        }
    }
    printf("\033[0m");
    return index;

}

void Documentation(){
    printf("Find in the following Section the documentation of our Project, as well as the syntax we used:\n\n\n");

    printf("  To process a set of Clauses by this program, you must follow the following rules: \n");
    printf("     * Write the set without curly brackets in a plain text file (.txt)\n");
    printf("     * Clauses must be written in conjunctive normal form as (c1 and c2 and c3 ....). \n");
    printf("     * each clause is composed of literals connected by the \"or\" operator \n");
    printf("     * the allowed operators are the following: \n");
    printf("          AND : &\n");
    printf("          OR  : |\n");
    printf("          Not : !\n");
    printf("     * Each Propositional variable must be an upper case english letter. \n");
    printf("     * If P, Q are terms then we have: \n");
    printf("          - !P is also a term\n");
    printf("          - P*Q is a term where * belongs to {&, |} (conjunction and disjunction).\n");
    printf("          - No other expression is a term.\n");
    printf("      ** Note that the previous rules must be verified for the program to proceed, otherwise, it won't proceed to resolution,\n");
    printf("         we already verify before proceeding **\n\n");
    printf("                                                                                      ** THANKS FOR READING **\n\n\n");

    printf("click \'q\' when you finish reading to quit");
    int i = 1;
    char key = getch();
    while(toupper(key) != 'Q'){
       key = getch();
    }

}

void howItWorks(){
    printf(" Here is how our algorithm works: \n");
    printf("It follows an intuitive approach, where we create a set containing all clauses we have, broken down into their propositional variables\n");
    printf("each time, we compare the current clause, with all the clauses after it, and try to find a resolution, everytime, we get a resolvent clause that cancels some terms\n");
    printf("we append it to a new list, after making sure it's unique, also, if we found non zero resolutions from a clause, or the clause contains one literal, we append it to the list\n");
    printf("as we may need it later, we repeat the process on the new set, until we find an empty clause, if we don't find any resolution, we conclude that our set is satisfiable\n");
    printf("if the set is satisfiable, we may fall in an infinite loop, so we have a 4 minutes counter, that asks the user if they want to stop \n\n\n");


    printf("press \'q\' to quit");
    char key = getch();
    while(toupper(key) != 'Q'){
        key = getch();
    }

}


