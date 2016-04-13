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

int main(int argc, char *argv[])
{
	if(argc >= 2) {
		if((yyin = fopen(argv[1], "r")) == NULL) {
			fprintf(stderr, "Error: file could not be opened.\n");
			exit(1);
		}
	}

	AST_NODE *rhs1 = astCreate(AST_SYMBOL, NULL, 0);
	AST_NODE *rhs2 = astCreate(AST_SYMBOL, NULL, 0);
	AST_NODE *lhs = astCreate(AST_SYMBOL, NULL, 0);
	AST_NODE *exp = astCreate(AST_ADD, NULL, 2, rhs1, rhs2);
	AST_NODE *ass = astCreate(AST_ATTR, NULL, 2, lhs, exp);
	AST_NODE *tst = astCreate(AST_SYMBOL, NULL, 0);
	AST_NODE *ift = astCreate(AST_IF, NULL, 2, tst, ass);

	FILE* dot = fopen("program.dot", "w");
	astPrintDot(dot, ift);
	fclose(dot);

	yyparse();

	printf("Parse successful.\n");
	hashPrint();

	return 0;
}
