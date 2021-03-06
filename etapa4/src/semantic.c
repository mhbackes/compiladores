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
int checkParameters(AST_NODE *node);

int astSymbolType(int astType) {
    switch (astType) {
	case AST_LPAR:
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

int numAndBool(int t1, int t2) {
    return (t1 != DTYPE_UNDEF && t2 != DTYPE_UNDEF &&
		    t1 != t2 && (t1 == DTYPE_BOOL || t2 == DTYPE_BOOL));
}

void checkInput(AST_NODE *node) {
    AST_NODE *in = node;
    while(in) {
	if(in->symbol->type != SYMBOL_SCALAR)
		semError(SEM_TYPE_EXPECTED_SCALAR, in->lineNumber, NULL);
	in = in->children[0];
    }
}

void checkOutput(AST_NODE *node) {
    AST_NODE *out = node;
    while(out) {
	if(out->children[0]->datatype == DTYPE_BOOL)
		semError(SEM_TYPE_OUTPUT, out->children[0]->lineNumber, NULL);
	out = out->children[1];
    }
}

int semCheck(AST_NODE* node) {
    _numErrors = 0;
    semCheckDeclaration(node);
    semCheckUndeclared(_symbolTable);
    semCheckUsage(node);
    semCheckTypes(node);
    return _numErrors;
}

void semCheckDeclaration(AST_NODE *node) {
    int i = 0;

    if(!node)
        return;

    switch(node->type) {
	case AST_LPAR:
        case AST_VARDEC:
        case AST_ARRDEC:
        case AST_FUNDEC:
            if(node->symbol->type != SYMBOL_UNDEF)
                semError(SEM_REDECLARED, node->lineNumber, node->symbol->text);
            node->symbol->type = astSymbolType(node->type);
            node->symbol->datatype = astDataType(node->children[0]->type);
	    node->symbol->declaration = node;
            node->datatype = node->symbol->datatype;
            break;

    }

    for(i = 0; i < node->size; i++)
        semCheckDeclaration(node->children[i]);
}

void semCheckUndeclared(HASH_NODE **hash) {
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
}

void semCheckUsage(AST_NODE  *node) {
    int i = 0;

    if(!node)
        return;

    switch(node->type) {
        // scalar
        case AST_VAR:   // rhs usage
        case AST_ATTR:  // lhs usage
            if(node->symbol->type != SYMBOL_SCALAR)
                semError(SEM_USAGE, node->lineNumber, node->symbol->text);
            break;

        // function
        case AST_FUNCALL:   // rhs usage
            if(node->symbol->type != SYMBOL_FUNC)
                semError(SEM_USAGE, node->lineNumber, node->symbol->text);
            break;

        // vector
        case AST_ARRACCESS:  // rhs usage
        case AST_ATTRARR:   // lhs usage
            if(node->symbol->type != SYMBOL_VECTOR)
                semError(SEM_USAGE, node->lineNumber, node->symbol->text);
    }

    for(i = 0; i < node->size; i++)
        semCheckUsage(node->children[i]);
}

int checkArithmetic(int type1, int type2, int lineNumber) {
    if(type1 == DTYPE_BOOL || type2 == DTYPE_BOOL) {
        semError(SEM_TYPE_EXPECTED_NUMBER, lineNumber, NULL);
        return DTYPE_INT;
    }
    switch(type1) {
        case DTYPE_CHAR:
            if(type2 == DTYPE_CHAR)
                return DTYPE_CHAR;
            else
                return type2;
        case DTYPE_INT:
            if(type2 == DTYPE_REAL)
                return DTYPE_REAL;
            else
                return type1;
        case DTYPE_REAL:
            return DTYPE_REAL;
    }
    return DTYPE_UNDEF;
}


void semCheckTypes(AST_NODE *node) {
    static int returnType = DTYPE_UNDEF;
    int i;

    if(!node) return;

    if(node->type == AST_FUNDEC)
        returnType = node->datatype;

    for(i = 0; i < node->size; i++)
        semCheckTypes(node->children[i]);

    switch (node->type) {
    case AST_VAR:
    case AST_LIT:
        node->datatype = node->symbol->datatype;
        break;

    case AST_NOT:
        if(node->children[0]->datatype != DTYPE_BOOL)
            semError(SEM_TYPE_EXPECTED_BOOL, node->lineNumber, NULL);
        node->datatype = DTYPE_BOOL;
        break;

    case AST_AND:
    case AST_OR:
        if(node->children[0]->datatype != DTYPE_BOOL ||
                node->children[1]->datatype != DTYPE_BOOL)
            semError(SEM_TYPE_EXPECTED_BOOL, node->lineNumber, NULL);
        node->datatype = DTYPE_BOOL;
        break;

    case AST_EQ:
    case AST_NE:
        if((node->children[0]->datatype == DTYPE_BOOL &&
                node->children[1]->datatype != DTYPE_BOOL) || 
                (node->children[0]->datatype != DTYPE_BOOL &&
                node->children[1]->datatype == DTYPE_BOOL))
            semError(SEM_TYPE_INCOMPATIBLE_OPS, node->lineNumber, NULL);
        node->datatype = DTYPE_BOOL;
        break;

    case AST_LE:
    case AST_GE:
    case AST_LESS:
    case AST_GREATER:
        if(node->children[0]->datatype == DTYPE_BOOL ||
                node->children[1]->datatype == DTYPE_BOOL)
            semError(SEM_TYPE_EXPECTED_NUMBER, node->lineNumber, NULL);

        node->datatype = DTYPE_BOOL;
        break;

    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
        node->datatype = checkArithmetic(node->children[0]->datatype,
                                         node->children[1]->datatype,
                                         node->lineNumber);
        break;

    case AST_DIV:
        if(node->children[0]->datatype == DTYPE_BOOL ||
                node->children[1]->datatype == DTYPE_BOOL)
            semError(SEM_TYPE_EXPECTED_NUMBER, node->lineNumber, NULL);
        node->datatype = DTYPE_REAL;
        break;

    case AST_PAR:
        node->datatype = node->children[0]->datatype;
        break;	

	case AST_FUNCALL:
	    node->datatype = node->symbol->declaration->datatype;
	    checkParameters(node);
	    break;
	
	case AST_ARRACCESS:
	    node->datatype = node->symbol->datatype;
	    if(node->children[0]->datatype != DTYPE_INT && 
			    node->children[0]->datatype != DTYPE_CHAR) 
                semError(SEM_TYPE_INDEX, node->lineNumber, node->symbol->text);
	    break;
	
	case AST_ATTR:
	    if(numAndBool(node->symbol->datatype, node->children[0]->datatype)) 
		semError(SEM_TYPE_ATTR_TYPE, node->lineNumber, NULL);
	    break;

	case AST_ATTRARR:
	    if(numAndBool(node->symbol->datatype, node->children[1]->datatype)) 
		semError(SEM_TYPE_ATTR_TYPE, node->lineNumber, NULL);

	    if(node->children[0]->datatype != DTYPE_INT && 
			    node->children[0]->datatype != DTYPE_CHAR) 
                semError(SEM_TYPE_INDEX, node->lineNumber, node->symbol->text);
	    break;
	
	case AST_IF:
	case AST_IFTE:
	case AST_WHILE:
	    if(node->children[0]->datatype != DTYPE_BOOL)
		semError(SEM_TYPE_EXPECTED_BOOL, node->lineNumber, NULL);
	    break;

	case AST_INPUT:
	    checkInput(node->children[0]);
	    break;

	case AST_OUTPUT:
	    checkOutput(node->children[0]);
	    break;

	case AST_RETURN:
        if(numAndBool(node->children[0]->datatype, returnType))
		    semError(SEM_TYPE_INCOMPATIBLE_RETURN, node->lineNumber, NULL);
	    break;
    }
}



// expList = list of call arguments, parList = list of declaration parameters
int checkParameters(AST_NODE *node) {
	int nArg = 0, nPar = 0; 
	AST_NODE *tArg = node->children[0], *tPar = node->symbol->declaration->children[1];

	while(tPar) {
		if(tArg != NULL) {
			if(numAndBool(tArg->children[0]->datatype, tPar->datatype))
				semError(SEM_TYPE_INCOMPATIBLE_ARGS, node->lineNumber, 
						node->symbol->text);
			nArg++;
			tArg = tArg->children[1];
		}

		nPar++;
		tPar = tPar->children[1];
	}

	while(tArg) {
	    tArg = tArg->children[1];
	    nArg++;
	}

	if(nPar != nArg)
		semError(SEM_USAGE_NOF_ARGS, node->lineNumber, node->symbol->text);

	return 0;
}

void semError(int error, int line, char *msg) {
    _numErrors++;
    fprintf(stderr, "[SEM] line %d: ", line);
    fprintf(stderr, _errorMessage[error], msg);
    fprintf(stderr, "\n");
}
