/*
 * tac.c
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 * >>>>>>>>><3 CODE SPONSORED BY THE ORDER OF THE LABLE <3<<<<<<<<<<
 */

#include "tac.h"
#include <stdlib.h>
#include <stdarg.h>

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

TAC *tacJoin(int nof, ...) {
    va_list args;
    TAC *tacs[nof];
    int i;

    va_start(args, nof);
    for(i = 0; i < nof; i++)
        tacs[i] = va_arg(args, TAC*);
    va_end(args);
    

    return NULL;
}
