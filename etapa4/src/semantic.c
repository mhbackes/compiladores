#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

int checkDeclaration(AST_NODE *node) {
    int i = 0;
    
    if(!node)
	    return 0;

    switch(node->type) {
	    case AST_VARDEC:
	    case AST_ARRDEC:
	    case AST_FUNHD:
		    if(node->symbol->type != SYMBOL_IDENTIFIER)
			    fprintf(stderr, "[SEM] VAR WAS REDECLARED\n");
		    node->symbol->type = node->type;
		    switch(node->children[0]->type) {
			    case AST_INT:
			    case AST_REAL:
			    case AST_CHAR:
				    node->symbol->datatype = SCALAR;
				    break;
			    case AST_BOOL:
				    node->symbol->datatype = BOOLEAN;
				    break;
		    }
		    break;

    }

    for(i = 0; i < node->size; i++)
	checkDeclaration(node->children[i]);

    return 0;
}

int checkUndeclared(HASH_NODE *hash) {

    return 0;
}

int checkUsage(AST_NODE  *node) {

    return 0;
}
