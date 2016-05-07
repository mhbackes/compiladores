#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

int _numErrors;

const char *_errorMessage[] = {
    FOREACH_ERROR(GENERATE_ERROR_STRING)
};

int astSymbolType(int astType);
int astDataType(int astType);

int astSymbolType(int astType) {
    switch (astType) {
        case AST_VARDEC: return SYMBOL_SCALAR;
        case AST_ARRDEC: return SYMBOL_VECTOR;
        case AST_FUNDEC: return SYMBOL_FUNC;
    }
    return SYMBOL_UNDEF;
}

int astDataType(int astType) {
    switch (astType) {
        case AST_BOOL: return DTYPE_BOOL;
        case AST_CHAR: return DTYPE_CHAR;
        case AST_INT:  return DTYPE_INT;
        case AST_REAL: return DTYPE_REAL;
    }
    return DTYPE_UNDEF;
}

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
            node->symbol->type = astSymbolType(node->type);
            node->symbol->datatype = astDataType(node->children[0]->type);
            break;
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

extern char* _symboTypelString[];

int checkUsage(AST_NODE  *node) {
    int i = 0;

    if(!node)
        return 0;

    switch(node->type) {
        case AST_VAR:
            if(node->symbol->type != SYMBOL_SCALAR)
                semError(SEM_USAGE, node->lineNumber, node->symbol->text);
            break;
        case AST_FUNCALL:
            if(node->symbol->type != SYMBOL_FUNC)
                semError(SEM_USAGE, node->lineNumber, node->symbol->text);
            break;

    }

    for(i = 0; i < node->size; i++)
        checkUsage(node->children[i]);

    return 0;
}

void semError(int error, int line, char *msg) {
    _numErrors++;
    fprintf(stderr, "[SEM] line %d: ", line);
    fprintf(stderr, _errorMessage[error], msg);
    fprintf(stderr, "\n");
}
