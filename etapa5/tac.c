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

/* nof: number of TACs to be joined */
TAC *tacJoin(int nof, TAC **tacs) {
    int i;

    for(i = nof; i > 0; i--) {
	while(tacs[i]->prev) 
	    tacs[i] = tacs[i]->prev;
	
	tacs[i]->prev = tacs[i-1];
	tacs[i-1]->next = tacs[i];
    }
    
    return tacs[i];
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
	case AST_FUNCALL:
	    return NULL;
        case AST_ARRACCESS:
	    return NULL;
	case AST_LPAR:
	    return NULL;
	case AST_ATTR:
            return NULL;
        case AST_ATTRARR:
            return NULL;
        case AST_OUTPUT:
            return NULL;
        case AST_IF:
            return NULL;
        case AST_IFTE:
            return NULL;
        case AST_WHILE:
            return NULL;
        case AST_RETURN:
            return NULL;
        case AST_NOT:
            return NULL;
        case AST_LE:
        case AST_GE:
        case AST_EQ:
        case AST_NE:
        case AST_AND:
        case AST_OR:
        case AST_LESS:
        case AST_GREATER:
        case AST_ADD:
        case AST_SUB:
        case AST_MUL:
        case AST_DIV:
            return NULL;
        default:
            fprintf(stderr, "Unknown");
    }
   return NULL;
}
