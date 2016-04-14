#include <stdlib.h>
#include <stdarg.h>
#include "ast.h"

const char* _astString[] = {
	FOREACH_AST(GENERATE_STRING)
};

void astPrintDotNodes(FILE *file, AST_NODE *node);
void astPrintDotEdges(FILE* file, AST_NODE* node);

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

void astPrintDot(FILE *file, AST_NODE *node) {
	fprintf(file, "digraph program {\n");
	astPrintDotNodes(file, node);
	astPrintDotEdges(file, node);
	fprintf(file, "}\n");
}

void astPrintDotNodes(FILE *file, AST_NODE *node) {
	if(!node) return;
	fprintf(file, "\t\"%p\" [label=\"%s\"]\n", node, _astString[node->type]);
	if(node->type == AST_SYMBOL)
		fprintf(file, "\t\"%p\" [label=\"%s\"]\n", node->symbol, node->symbol->text);
	int i;
	for(i = 0; i < node->size; i++){
		astPrintDotNodes(file, node->children[i]);
	}
}

void astPrintDotEdges(FILE* file, AST_NODE* node) {
	if(!node) return;
	if(node->type == AST_SYMBOL)
		fprintf(file, "\t\"%p\" -> \"%p\"\n", node, node->symbol);
	int i;
	for(i = 0; i < node->size; i++){
		fprintf(file, "\t\"%p\" -> \"%p\" ", node, node->children[i]);
		fprintf(file, "[label=\"%d\"]\n", i);
		astPrintDotEdges(file, node->children[i]);
	}
}
