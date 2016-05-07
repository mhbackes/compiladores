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
#include "y.tab.h"
#include "lex.yy.h"

extern AST_NODE *root;

int main(int argc, char *argv[]) {
    if(argc >= 2) {
        if((yyin = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr, "Error: file could not be opened.\n");
            exit(1);
        }
    }

    yyparse();

    printf("Parse successful.\n");

    FILE* prog = stdout;
    if(argc >= 3)
        prog = fopen(argv[2], "w");

    //hashPrint();
    //astPrintCode(prog, root);
    //printf("Decompiled program written in \"%s\".\n", argv[2]);
    //astPrintDot(prog, root);
    //printf("Dot written in \"%s\".\n", argv[2]);
    fclose(prog);

    return 0;
}
