/*
 * tac.c
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#include "tac.h"
#include <stdlib.h>
#include <stdarg.h>

/* GLOBAL DECS */

const char* _tacString[] = {
    FOREACH_TAC(GENERATE_TAC_STRING)
};

/* PROTOTYPES */

TAC *tacUnaryOp(int type, int datatype, TAC **code);

TAC *tacBinOp(int type, int datatype, TAC **code);

TAC *tacAttr(HASH_NODE* res, TAC **code);

TAC *tacAttrArr(HASH_NODE* res, TAC **code);

TAC *tacReadArr(HASH_NODE* vec, TAC **code);

TAC *tacArgs(AST_NODE *larg, AST_NODE *lexp);

TAC *tacFunCall(AST_NODE *fun);

TAC *tacIfThen(TAC **code);

TAC *tacIfThenElse(TAC **code);

TAC *tacWhile(TAC **code);

TAC *tacFunDec(HASH_NODE *res, TAC **code);

TAC *tacOutputArgs(TAC **code);

TAC *tacOutput(TAC **code);

TAC *tacInput(TAC **code);

TAC *tacInputArgs(TAC **code, HASH_NODE *id);

TAC *tacReturn(TAC **code);

/* CODE */

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

    if(node->type == AST_FUNCALL)
        return tacFunCall(node);

    for(i = 0; i < node->size; i++)
        if(!node->children[i])
            code[i] = NULL;
        else
            code[i] = generateCode(node->children[i]); 

    switch(node->type) {
        case AST_LIT:
        case AST_VAR:
            return tacCreate(TAC_SYMBOL, node->symbol, NULL, NULL);
        case AST_ARRACCESS:
            return tacReadArr(node->symbol, code);
        case AST_ATTR:
            return tacAttr(node->symbol, code);
        case AST_ATTRARR:
            return tacAttrArr(node->symbol, code);
        case AST_FUNDEC:
            return tacFunDec(node->symbol, code);
        case AST_OUTPUT:
            return tacOutput(code);
        case AST_LOUT:
            return tacOutputArgs(code);
        case AST_INPUT:
            return tacInput(code);
        case AST_LIN:
            return tacInputArgs(code, node->symbol);
        case AST_IF:
            return tacIfThen(code);
        case AST_IFTE:
            return tacIfThenElse(code);
        case AST_WHILE:
            return tacWhile(code);
        case AST_RETURN:
            return tacReturn(code);
        case AST_NOT:
            return tacUnaryOp(TAC_NOT, node->datatype, code);
        case AST_LE:
            return tacBinOp(TAC_LE, node->datatype, code);
        case AST_GE:
            return tacBinOp(TAC_GE, node->datatype, code);
        case AST_EQ:
            return tacBinOp(TAC_EQ, node->datatype, code);
        case AST_NE:
            return tacBinOp(TAC_NE, node->datatype, code);
        case AST_AND:
            return tacBinOp(TAC_AND, node->datatype, code);
        case AST_OR:
            return tacBinOp(TAC_OR, node->datatype, code);
        case AST_LESS:
            return tacBinOp(TAC_LESS, node->datatype, code);
        case AST_GREATER:
            return tacBinOp(TAC_GREATER, node->datatype, code);
        case AST_ADD:
            return tacBinOp(TAC_ADD, node->datatype, code);
        case AST_SUB:
            return tacBinOp(TAC_SUB, node->datatype, code);
        case AST_MUL:
            return tacBinOp(TAC_MUL, node->datatype, code);
        case AST_DIV:
            return tacBinOp(TAC_DIV, node->datatype, code);
        default: 
            return tacArrayJoin(node->size, code);
    }
}

TAC *tacBinOp(int type, int datatype, TAC **code) {
    TAC *newTac = tacCreate(type, makeTemp(datatype), code[0]?code[0]->res:NULL, 
		    code[1]?code[1]->res:NULL);
    return tacMultiJoin(3, code[0], code[1], newTac);
}

TAC *tacUnaryOp(int type, int datatype, TAC **code) {
    TAC *newTac = tacCreate(type, makeTemp(datatype), code[0]?code[0]->res:NULL, NULL);
    return tacMultiJoin(2, code[0], newTac);
}

TAC *tacAttr(HASH_NODE* res, TAC **code) {
    TAC *newTac = tacCreate(TAC_ATTR, res, code[0]?code[0]->res:NULL, NULL);
    return tacMultiJoin(2, code[0], newTac);
}

TAC *tacAttrArr(HASH_NODE* res, TAC **code) {
    TAC *newTac = tacCreate(TAC_ATTRARR, res, code[0]?code[0]->res:NULL,
            code[1]?code[1]->res:NULL);
    return tacMultiJoin(3, code[0], code[1], newTac);
}

TAC *tacReadArr(HASH_NODE* vec, TAC **code) {
    TAC *newTac = tacCreate(TAC_READARR, makeTemp(vec->datatype), vec,
            code[0]?code[0]->res:NULL);
    return tacMultiJoin(2, code[0], newTac);
}

TAC *tacWhile(TAC **code) {
    TAC *tacExp = code[0];
    TAC *tacCmd = code[1];

    // label before test expression
    HASH_NODE *labelPrev = makeLabel();
    TAC *tacPrev = tacCreate(TAC_LABEL, labelPrev, NULL, NULL);

    // label after while command
    HASH_NODE *labelNext = makeLabel();
    TAC *tacNext = tacCreate(TAC_LABEL, labelNext, NULL, NULL);
    
    // while conditional jump
    TAC *tacIfz = tacCreate(TAC_IFZ, labelNext, tacExp?tacExp->res:NULL, NULL);

    // jump back to test expression after command
    TAC *tacJmp = tacCreate(TAC_JUMP, labelPrev, NULL, NULL);

    return tacMultiJoin(6, tacPrev, tacExp, tacIfz, tacCmd, tacJmp, tacNext);
}

TAC *tacIfThen(TAC **code) {
    TAC *nIf, *target;
    HASH_NODE *label = makeLabel();
    /* ifz tac */
    nIf = tacCreate(TAC_IFZ, label, code[0]?code[0]->res:NULL, NULL);
    /* ifz false target */
    target = tacCreate(TAC_LABEL, label, NULL, NULL);

    return tacMultiJoin(4, code[0], nIf, code[1], target);
}

TAC *tacFunDec(HASH_NODE *res, TAC **code) { 
    TAC *begin, *end;

    begin = tacCreate(TAC_BEGINFUN, res, NULL, NULL);
    end = tacCreate(TAC_ENDFUN, res, NULL, NULL);
    
    return tacMultiJoin(3, begin, code[2], end);
}


TAC *tacIfThenElse(TAC **code) {
    TAC *tacExp = code[0];
    TAC *tacThen = code[1];
    TAC *tacElse = code[2];
    
    // label before else
    HASH_NODE* labelElse = makeLabel();
    TAC *tacLabelElse = tacCreate(TAC_LABEL, labelElse, NULL, NULL);

    // label after else
    HASH_NODE* labelNext = makeLabel();
    TAC *tacLabelNext = tacCreate(TAC_LABEL, labelNext, NULL, NULL);

    // ifz conditional jump
    TAC *tacIfz = tacCreate(TAC_IFZ, labelElse, tacExp?tacExp->res:NULL,
            NULL);

    // jump at end of then command
    TAC *tacJmp = tacCreate(TAC_JUMP, labelNext, NULL, NULL);
    
    return tacMultiJoin(7, tacExp, tacIfz, tacThen, tacJmp, tacLabelElse,
            tacElse, tacLabelNext);
}

TAC *tacFunCall(AST_NODE *funCall) {
    AST_NODE *funDef = funCall->symbol->declaration;
    AST_NODE *largs = funDef->children[1];
    AST_NODE *lexp = funCall->children[0];

    TAC *tacArg = tacArgs(largs, lexp);
    TAC *tacCall = tacCreate(TAC_CALL, makeTemp(funCall->datatype), funCall->symbol, NULL);
    return tacMultiJoin(2, tacArg, tacCall);
}

TAC *tacArgs(AST_NODE *larg, AST_NODE *lexp) {
    TAC *tacAcc = NULL;

    while(larg && lexp) {
        HASH_NODE *arg = larg->symbol;
        AST_NODE *exp = lexp->children[0];
        
        TAC *tacExp = generateCode(exp);
        TAC *tacArg = tacCreate(TAC_ARG, arg, tacExp?tacExp->res:NULL, NULL);

        tacAcc = tacMultiJoin(3, tacAcc, tacExp, tacArg);

        larg = larg->children[1];
        lexp = lexp->children[1];
    }

    return tacAcc;
}

// deus abencoe a america 
TAC *tacOutputArgs(TAC **code) {
    TAC *lout = code[1];
    TAC *litOrExp = code[0]; 
    if(!litOrExp)
        return NULL; // no args
    if(!lout) { // end of tree's list
        return tacJoin(litOrExp, tacCreate(TAC_OUT_ARG, litOrExp?litOrExp->res:NULL,
                                            NULL, NULL));
    }
    // there's still args
    return tacMultiJoin(3, litOrExp, tacCreate(TAC_OUT_ARG, litOrExp?litOrExp->res:NULL,
                                        NULL, NULL), lout);
}

// make america great again 
TAC *tacInputArgs(TAC **code, HASH_NODE *id) {
    TAC *lin = code[0];
    if(!id)
        return NULL; // no args
    if(!lin) { // end of tree's list
        return tacCreate(TAC_INPUT_ARG, id?id:NULL, NULL, NULL);
    }
    // there's still args
    return tacJoin(tacCreate(TAC_INPUT_ARG, id?id:NULL, NULL, NULL), lin);
}

TAC *tacOutput(TAC **code) {
    TAC *lout = code[0];
    TAC *output = tacCreate(TAC_PRINT, NULL, NULL, NULL);
    return tacJoin(lout, output);
}

TAC *tacInput(TAC **code) {
    TAC *lin = code[0];
    TAC *input = tacCreate(TAC_INPUT, NULL, NULL, NULL);
    return tacJoin(lin, input);
}

TAC *tacReturn(TAC **code) {
    TAC *retExp = code[0];
    TAC *retTac = tacCreate(TAC_RET, NULL, retExp?retExp->res:NULL, NULL);
    return tacJoin(retExp, retTac);
}

void tacPrint(TAC *tac) {
    TAC *tmp;
    for(tmp = tac; tmp; tmp = tmp->next)  
        if(tmp->type != TAC_SYMBOL) // removed symbol printing
            fprintf(stderr, "%s %s, %s, %s\n", _tacString[tmp->type], 
                    tmp->res?tmp->res->text:"_", 
                    tmp->op1?tmp->op1->text:"_",
                    tmp->op2?tmp->op2->text:"_");
    return;
}
