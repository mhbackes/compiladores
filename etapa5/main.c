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

    yyparse();

    int semErrors = semCheck(root);

    if(semErrors) {
        fprintf(stderr, "%d semantic error(s) found.\n", semErrors);   
        exit(EXIT_SEMANTIC_ERROR);
    }

    TAC *tacs = generateCode(root);
    tacs = tacReverse(tacs);
    tacPrint(stderr, tacs);

    printf("Parse successful.\n");

    FILE* prog = stdout;
    if(argc >= 3)
        prog = fopen(argv[2], "w");

    //tacPrintDot(prog, tacs);

    //hashPrint();
    //astPrintCode(prog, root);
    //printf("Decompiled program written in \"%s\".\n", argv[2]);
    //astPrintDot(prog, root);
    //printf("Dot written in \"%s\".\n", argv[2]);
    fclose(yyin);
    fclose(prog);

    return 0;
}
