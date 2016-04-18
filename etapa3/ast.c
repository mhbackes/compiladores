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
	if(node->symbol)
		fprintf(file, "\t\"%p\" [label=\"%s\"]\n", node->symbol, node->symbol->text);
	int i;
	for(i = 0; i < node->size; i++){
		astPrintDotNodes(file, node->children[i]);
	}
}

void astPrintDotEdges(FILE* file, AST_NODE* node) {
	if(!node) return;
	if(node->symbol)
		fprintf(file, "\t\"%p\" -> \"%p\" [label=\"s\"]\n", node, node->symbol);
	int i;
	for(i = 0; i < node->size; i++){
		fprintf(file, "\t\"%p\" -> \"%p\" ", node, node->children[i]);
		fprintf(file, "[label=\"%d\"]\n", i);
		astPrintDotEdges(file, node->children[i]);
	}
}

void astPrintCode(FILE* file, AST_NODE* node) {
	if(!node) return;
	switch(node->type) {
		case AST_SYMBOL:
			fprintf(file, "%s", node->symbol->text);
			break;
		case AST_VARDEC:
			astPrintCode(file, node->children[0]);
			fprintf(file, " %s:", node->symbol->text);
			astPrintCode(file, node->children[1]);
			fprintf(file, ";");
			break;
		case AST_ARRDEC:
			astPrintCode(file, node->children[0]);
			fprintf(file, " %s[", node->symbol->text);
			astPrintCode(file, node->children[1]);
			fprintf(file, "]");
			if(node->children[2]) {
				fprintf(file, ":");
				astPrintCode(file, node->children[2]);
			}
			break;
		case AST_FUNDEC:
			//TODO
			break;
		case AST_FUNCALL:
			//TODO
			break;
		case AST_ARRACESS:
			//TODO
			break;
		case AST_LCMD:
			astPrintCode(file, node->children[0]);
			if(node->children[1]) {
				fprintf(file, "\n");
				astPrintCode(file, node->children[1]);
			}
			break;
		case AST_LPAR:
			//TODO
			break;
		case AST_LLIT:
			//TODO
			break;
		case AST_LEXP:
			astPrintCode(file, node->children[0]);
			if(node->children[1]) {
				fprintf(file, ", ");
				astPrintCode(file, node->children[1]);
			}
			break;
		case AST_LIN:
			fprintf(file, "%s", node->symbol->text);
			if(node->children[0]) {
				fprintf(file, ", ");
				astPrintCode(file, node->children[0]);
			}
			break;
		case AST_LOUT:
			//TODO
			break;
		case AST_ATTR:
			fprintf(file, "%s = ", node->symbol->text);
			astPrintCode(file, node->children[0]);
			break;
		case AST_ATTRARR:
			//TODO
			break;
		case AST_INPUT:
			fprintf(file, "input ");
			astPrintCode(file, node->children[0]);
			break;
		case AST_OUTPUT:
			//TODO
			break;
		case AST_IF:
			//TODO
			break;
		case AST_IFTE:
			//TODO
			break;
		case AST_WHILE:
			//TODO
			break;
		case AST_RETURN:
			//TODO
			break;
		case AST_EMPTY:
			//TODO
			break;
		case AST_NOT:
			fprintf(file, "!");
			astPrintCode(file, node->children[0]);
			break;
		case AST_LE:
			astPrintCode(file, node->children[0]);
			fprintf(file, " <= ");
			astPrintCode(file, node->children[1]);
			break;
		case AST_GE:
			astPrintCode(file, node->children[0]);
			fprintf(file, " >= ");
			astPrintCode(file, node->children[1]);
			break;
		case AST_EQ:
			astPrintCode(file, node->children[0]);
			fprintf(file, " == ");
			astPrintCode(file, node->children[1]);
			break;
		case AST_NE:
			astPrintCode(file, node->children[0]);
			fprintf(file, " != ");
			astPrintCode(file, node->children[1]);
			break;
		case AST_AND:
			astPrintCode(file, node->children[0]);
			fprintf(file, " && ");
			astPrintCode(file, node->children[1]);
			break;
		case AST_OR:
			astPrintCode(file, node->children[0]);
			fprintf(file, " || ");
			astPrintCode(file, node->children[1]);
			break;
		case AST_ADD:
			astPrintCode(file, node->children[0]);
			fprintf(file, " + ");
			astPrintCode(file, node->children[1]);
			break;
		case AST_SUB:
			astPrintCode(file, node->children[0]);
			fprintf(file, " - ");
			astPrintCode(file, node->children[1]);
			break;
		case AST_MUL:
			astPrintCode(file, node->children[0]);
			fprintf(file, " * ");
			astPrintCode(file, node->children[1]);
			break;
		case AST_DIV:
			astPrintCode(file, node->children[0]);
			fprintf(file, " / ");
			astPrintCode(file, node->children[1]);
			break;
		case AST_LESS:
			astPrintCode(file, node->children[0]);
			fprintf(file, " < ");
			astPrintCode(file, node->children[1]);
			break;
		case AST_GREATER:
			astPrintCode(file, node->children[0]);
			fprintf(file, " > ");
			astPrintCode(file, node->children[1]);
			break;
		case AST_PAR:
			fprintf(file, "(");
			astPrintCode(file, node->children[0]);
			fprintf(file, ")");
			break;
		case AST_BLOCK:
			fprintf(file, "{\n");
			astPrintCode(file, node->children[0]);
			fprintf(file, "\n}");
			break;
		case AST_BOOL:
			fprintf(file, "bool");
			break;
		case AST_CHAR:
			fprintf(file, "char");
			break;
		case AST_INT:
			fprintf(file, "int");
			break;
		case AST_REAL:
			fprintf(file, "real");
			break;
		default:
			fprintf(stderr, "Unknown");
	}
}
