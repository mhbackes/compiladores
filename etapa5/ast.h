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

#define FOREACH_AST(AST)\
    AST(AST_PROGRAM)    \
    AST(AST_VARDEC)     \
    AST(AST_ARRDEC)     \
    AST(AST_FUNDEC)     \
    AST(AST_LIT)		\
    AST(AST_VAR)        \
    AST(AST_FUNCALL)    \
    AST(AST_ARRACCESS)   \
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
    AST(AST_PAR)		\
    AST(AST_BLOCK)		\
    AST(AST_BOOL)		\
    AST(AST_CHAR)		\
    AST(AST_INT)		\
    AST(AST_REAL)

#define GENERATE_AST_ENUM(ENUM) ENUM,
#define GENERATE_AST_STRING(STRING) #STRING,

enum ast_types {
    FOREACH_AST(GENERATE_AST_ENUM)
};

typedef struct ast_node {
    int type;
    int datatype;
    int lineNumber;
    HASH_NODE *symbol;
    int size;
    struct ast_node **children;
} AST_NODE;

AST_NODE *astCreate(int type, int lineNumber, HASH_NODE *symbol, int size, ...);

void astPrintDot(FILE *file, AST_NODE *node);

void astPrintCode(FILE* file, AST_NODE* node);

#endif
