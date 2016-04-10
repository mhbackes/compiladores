#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ast.h"

AST_NODE_LIST *nodeListInsert(AST_NODE_LIST *list, AST_NODE *node);

/* Creates a AST_NODE * with any number of sons (after parameter symbol) */
AST_NODE *astCreate(int type, HASH_NODE *symbol, ...) {
	va_list args;
	AST_NODE *newNode, *tmp; 

	if(!(newNode = (AST_NODE *) malloc(sizeof(AST_NODE)))) {
		fprintf(stderr, "ERROR [AST]: out of memory!\n");
		exit(1); // abort
	}

	newNode->type = type;
	newNode->symbol = symbol;
	newNode->sons = NULL;

	va_start(args, symbol);

	while(!(tmp = va_arg(args, AST_NODE *)))
		newNode->sons = nodeListInsert(newNode->sons, tmp);

	va_end(args);

	return newNode;
}

void astPrint(AST_NODE *node) {
	return;
}

/* Inserts a AST_NODE at the beginning of a AST_NODE_LIST */
AST_NODE_LIST *nodeListInsert(AST_NODE_LIST *list, AST_NODE *node) {
	AST_NODE_LIST *tmp;

	if(!(tmp = (AST_NODE_LIST *) malloc(sizeof(AST_NODE_LIST)))) {
			fprintf(stderr, "ERROR [AST]: out of memory!\n");
			exit(1); // abort
		}

	tmp->node = node;
	tmp->next = list;
	list = tmp;

	return list;
}

