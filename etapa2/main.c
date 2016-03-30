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

extern FILE *yyin;

int main(int argc, char *argv[])
{
	if(argc < 2) {
		fprintf(stderr, "Error: missing argument. ./etapa2 <filename>");
		exit(1);
	}
	
	if((yyin = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Error: file could not be opened.");
		exit(1);
	}

	yyparse();
	printf("Parse successful.\n");
	hashPrint();

	return 0;
}
