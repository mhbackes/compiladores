/*
 * main.c
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "ast.h"
#include "tac.h"
#include "y.tab.h"
#include "lex.yy.h"
#include "semantic.h"
#include "error.h"

extern AST_NODE *root;

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Usage: %s input.txt [output.txt]\n", argv[0]);
        exit(EXIT_PARAMETER_ERROR);
    }

    if((yyin = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Error: file could not be opened.\n");
        exit(EXIT_FILE_ERROR);
    }

    /* Parsing */
    yyparse();

    // free input and lex buffers
    fclose(yyin);
    yylex_destroy();

    printf("Parse successful.\n");
    /* Parsing */

    /* Semantic checking */
    int semErrors = semCheck(root);

    if(semErrors) {
        fprintf(stderr, "%d semantic error(s) found.\n", semErrors);   
        exit(EXIT_SEMANTIC_ERROR);
    }

    printf("Semantic check successful.\n");
    /* Semantic checking */

    /* TAC generating */
    TAC *tacs = generateCode(root);
    tacs = tacReverse(tacs);
    tacs = tacRemoveSymbols(tacs);

    // syntax tree not needed anymore
    astDeleteTree(root);

    printf("TACs created.");
    /* TAC generating */

    FILE* prog = stdout;
    if(argc >= 3)
        prog = fopen(argv[2], "w");

    tacPrint(prog, tacs);
    fclose(prog);

    // free tacs and symbol table
    tacDeleteList(tacs);
    hashClean();


    return 0;
}
