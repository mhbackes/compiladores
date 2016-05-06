#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

int _numErrors;

const char *_errorMessage[] = {
    FOREACH_ERROR(GENERATE_ERROR_STRING)
};

int checkDeclaration(AST_NODE *node) {
    int i = 0;

    if(!node)
        return 0;

    switch(node->type) {
        case AST_VARDEC:
        case AST_ARRDEC:
        case AST_FUNDEC:
            if(node->symbol->type != SYMBOL_UNDEF)
                semError(SEM_REDECLARED, node->lineNumber, node->symbol->text);
            node->symbol->type = node->type;
            node->symbol->datatype = node->children[0]->type;
    }

    for(i = 0; i < node->size; i++)
        checkDeclaration(node->children[i]);

    return 0;
}

int checkUndeclared(HASH_NODE **hash) {
    int i;
    HASH_NODE *node;

    for(i = 0; i < HASH_SIZE; i++) {
        node = hash[i];
        while(node) {
            if(node->type == SYMBOL_UNDEF)
                semError(SEM_UNDECLARED, node->lineNumber, node->text);
            node = node->next;
        }
    }

    return 0;
}

int checkUsage(AST_NODE  *node) {

    return 0;
}

void semError(int error, int line, char *msg) {
    _numErrors++;
    fprintf(stderr, "[SEM] line %d: ", line);
    fprintf(stderr, _errorMessage[error], msg);
    fprintf(stderr, "\n");
}
