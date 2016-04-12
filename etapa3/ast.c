#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ast.h"

AST_NODE_LIST *nodeListInsert(AST_NODE_LIST **list, AST_NODE *node);

/* Sentinel value must be passed as last argument (using NULL) */
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

	while((tmp = va_arg(args, AST_NODE *)) != NULL) 
		nodeListInsert(&newNode->sons, tmp);

	va_end(args);

	return newNode;
}

/* test stub */
void astPrint(AST_NODE *node, int level) {
    AST_NODE *tmp = node;
    AST_NODE_LIST *tl = node->sons;
    fprintf(stderr, "TYPE %d\n", tmp->type);
    while(tl) {
	fprintf(stderr, "TYPE %d\n", tl->node->type);
	tl = tl->next;
    }
    return;
}

AST_NODE_LIST *nodeListInsert(AST_NODE_LIST **list, AST_NODE *node) {
	AST_NODE_LIST *new;
	AST_NODE_LIST *tmp = *list;

	if(!(new = (AST_NODE_LIST *) malloc(sizeof(AST_NODE_LIST)))) {
		fprintf(stderr, "ERROR [AST]: out of memory!\n");
		exit(1); // abort
	}
	new->node = node;
	new->next = NULL;

	// list is uninitialized
	if(!tmp) { 
		*list = new;
		return *list;
	}

	// list was initialized, insert new son at 1st pos
	while(tmp->next)
		tmp = tmp->next;
	
	tmp->next = new;

	return *list;
}
