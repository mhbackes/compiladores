/*
 * ast.h
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#ifndef AST_ 
#define AST_ 

#include "hashtable.h"
#include <stdio.h>

/* preliminary types; doing it on the fly, add missing
 * ones or remove unecessary ones while we're doing it
 */
#define FOREACH_AST(AST)\
    AST(AST_SYMBOL)		\
    AST(AST_VARDEC)     \
    AST(AST_ARRDEC)     \
    AST(AST_FUNDEC)     \
    AST(AST_FUNCALL)    \
    AST(AST_ARRACESS)   \
    AST(AST_LCMD)		\
    AST(AST_LPAR)		\
    AST(AST_LLIT)		\
    AST(AST_LEXP)		\
    AST(AST_LIN)		\
    AST(AST_LOUT)		\
    AST(AST_ATTR)		\
    AST(AST_ATTRARR)    \
    AST(AST_INPUT)		\
    AST(AST_OUTPUT)		\
    AST(AST_IF)			\
    AST(AST_IFTE)		\
    AST(AST_WHILE)		\
    AST(AST_RETURN)		\
	AST(AST_EMPTY)		\
	AST(AST_NOT)		\
    AST(AST_LE)			\
    AST(AST_GE)			\
    AST(AST_EQ)			\
    AST(AST_NE)			\
    AST(AST_AND)		\
    AST(AST_OR)			\
    AST(AST_ADD)		\
    AST(AST_SUB)		\
    AST(AST_MUL)		\
    AST(AST_DIV)		\
    AST(AST_LESS)		\
    AST(AST_GREATER)	\
	AST(AST_PAR)        

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum ast_types {
	FOREACH_AST(GENERATE_ENUM)
};

typedef struct ast_node {
    int type;
    HASH_NODE *symbol;
	int size;
    struct ast_node **children;
} AST_NODE;

AST_NODE *astCreate(int type, HASH_NODE *symbol, int size, ...);

void astPrintDot(FILE *file, AST_NODE *node);

#endif
