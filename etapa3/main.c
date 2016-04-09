/*
 * main.c
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
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

	yyparse();
	printf("Parse successful.\n");
	hashPrint();

	return 0;
}
