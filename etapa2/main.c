/*
 * main.c
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#include <stdio.h>
#include "hashtable.h"
#include "y.tab.h"

int main(int argc, char *argv[])
{
	yyparse();
	printf("Parse successful.\n");
	hashPrint();
	return 0;
}
