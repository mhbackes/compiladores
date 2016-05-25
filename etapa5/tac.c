/*
 * tac.c
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 * >>>>>>>>> CODE SPONSORED BY THE ORDER OF THE LABLE <<<<<<<<<<
 */

#include "tac.h"
#include <stdlib.h>
#include <stdarg.h>


const char* _tacString[] = {
    FOREACH_TAC(GENERATE_TAC_STRING)
};

TAC *tacUnaryOp(int type, TAC **code);

TAC *tacBinOp(int type, TAC **code);

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2) {
    TAC *newTac; 

    if(!(newTac = (TAC *) malloc(sizeof(TAC)))) {
        fprintf(stderr, "ERROR [TAC]: out of memory!\n"); 
	    exit(-1); // abort
    }

    newTac->type = type;
    newTac->res = res;
    newTac->op1 = op1;
    newTac->op2 = op2;
    newTac->prev = newTac->next = NULL;

    return newTac;
}

TAC *tacJoin(TAC *t, TAC *s) {
    if(!t)
        return s;
    if(!s)
        return t;

    TAC *tmp = s;
    while(tmp->prev)
        tmp = tmp->prev;

    // double linking list
    tmp->prev = t;
    t->next = tmp;

    return s;
}

TAC *tacMultiJoin(int numTacs, ...) {
    int i;
    va_list args;
    TAC *acc = NULL, *t;
    va_start(args, numTacs);
    for(i = 0; i < numTacs; i++) {
        t = va_arg(args, TAC*);
        acc = tacJoin(acc, t);
    }
    return acc;
}

TAC *tacArrayJoin(int numTacs, TAC **tacs) {
    int i;
    TAC *acc = NULL;
    for(i = 0; i < numTacs; i++)
        acc = tacJoin(acc, tacs[i]);
    return acc;
}

TAC *tacReverse(TAC *tac) {
    TAC *tmp = tac;

    if(!tac)
	return NULL;

    while(tmp->prev) 
	tmp = tmp->prev;

    return tmp;
}

TAC *generateCode(AST_NODE *node) {
    TAC *code[node->size];    
    int i;

    for(i = 0; i < node->size; i++)
        if(!node->children[i])
            code[i] = NULL;
        else
            code[i] = generateCode(node->children[i]); 

    switch(node->type) {
        case AST_LIT:
        case AST_VAR:
            return tacCreate(TAC_SYMBOL, node->symbol, NULL, NULL);
        //case AST_FUNCALL:
            //return NULL;
        //case AST_ARRACCESS:
            //return NULL;
        //case AST_LPAR:
            //return NULL;
        //case AST_ATTR:
            //return NULL;
        //case AST_ATTRARR:
            //return NULL;
        //case AST_OUTPUT:
            //return NULL;
        //case AST_INPUT:
            //return NULL;
        //case AST_IF:
            //return NULL;
        //case AST_IFTE:
            //return NULL;
        //case AST_WHILE:
            //return NULL;
        //case AST_RETURN:
            //return NULL;
            // UNARY
        case AST_NOT:
            return tacUnaryOp(TAC_NOT, code);
            // BINARY
        case AST_LE:
            return tacBinOp(TAC_LE, code);
        case AST_GE:
            return tacBinOp(TAC_GE, code);
        case AST_EQ:
            return tacBinOp(TAC_EQ, code);
        case AST_NE:
            return tacBinOp(TAC_NE, code);
        case AST_AND:
            return tacBinOp(TAC_AND, code);
        case AST_OR:
            return tacBinOp(TAC_OR, code);
        case AST_LESS:
            return tacBinOp(TAC_LESS, code);
        case AST_GREATER:
            return tacBinOp(TAC_GREATER, code);
        case AST_ADD:
            return tacBinOp(TAC_ADD, code);
        case AST_SUB:
            return tacBinOp(TAC_SUB, code);
        case AST_MUL:
            return tacBinOp(TAC_MUL, code);
        case AST_DIV:
            return tacBinOp(TAC_DIV, code);
        default: 
            return tacArrayJoin(node->size, code);
    }
}

TAC *tacBinOp(int type, TAC **code) {
    TAC *new = tacCreate(type, makeTemp(), code[0]?code[0]->res:NULL, 
		    code[1]?code[1]->res:NULL);
    return tacMultiJoin(3, code[0], code[1], new);
}

TAC *tacUnaryOp(int type, TAC **code) {
    TAC *new = tacCreate(type, makeTemp(), code[0]?code[0]->res:NULL, NULL);
    return tacMultiJoin(2, code[0], new);
}

void tacPrint(TAC *tac) {
    TAC *tmp;
    for(tmp = tac; tmp; tmp = tmp->next)  // johann tm
	fprintf(stderr, "%s %s, %s, %s\n", _tacString[tmp->type], 
			tmp->res?tmp->res->text:"_", 
			tmp->op1?tmp->op1->text:"_",
			tmp->op2?tmp->op2->text:"_");
    return;
}
