#include <stdlib.h>
#include <stdarg.h>
#include "ast.h"

AST_NODE_LIST *nodeListInsert(AST_NODE_LIST **list, AST_NODE *node);
void astPrintDotNodes(AST_NODE *node, FILE* file);
void astPrintDotEdges(AST_NODE *node, FILE* file);

/* Sentinel value must be passed as last argument (using NULL) */
AST_NODE *astCreate(int type, HASH_NODE *symbol, int size, ...) {
	va_list args;
	AST_NODE *newNode; 

	if(!(newNode = (AST_NODE *) malloc(sizeof(AST_NODE)))) {
		fprintf(stderr, "ERROR [AST]: out of memory!\n");
		exit(1); // abort
	}

	newNode->type = type;
	newNode->symbol = symbol;
	newNode->size = size;
	newNode->children = malloc(sizeof(AST_NODE *) * size);

	va_start(args, size);

	int i;
	for(i = 0; i < size; i++)
		newNode->children[i] = va_arg(args, AST_NODE *);

	va_end(args);

	return newNode;
}

/* test stub */
void astPrint(AST_NODE *node, int level) {
    //AST_NODE *tmp = node;
    //AST_NODE_LIST *tl = node->children;
    //fprintf(stderr, "TYPE %d\n", tmp->type);
    //while(tl) {
	//fprintf(stderr, "TYPE %d\n", tl->node->type);
	//tl = tl->next;
    //}
    return;
}

void astPrintDot(AST_NODE *node, FILE* file) {
	fprintf(file, "digraph program {\n");
	astPrintDotNodes(node, file);
	astPrintDotEdges(node, file);
	fprintf(file, "}\n");
}

void astPrintDotNodes(AST_NODE *node, FILE* file) {

}

void astPrintDotEdges(AST_NODE *node, FILE* file) {
	
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
