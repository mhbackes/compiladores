/*
 * ast.h
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#ifndef AST_ 
#define AST_ 

#include "hashtable.h"

/* preliminary types; doing it on the fly, add missing
 * ones or remove unecessary ones while we're doing it
 */
enum ast_types {
    AST_SYMBOL,

    // lists
    AST_LCMD,   // cmd
    AST_LPAR,   // parameters
    AST_LLIT,   // literals
    AST_LEXP,   // expressions
    AST_LIN,    // input (??)
    AST_LOUT,   // output (??)

    // commands
    AST_ATTR,
    AST_INPUT,
    AST_OUTPUT,
    AST_IF,
    AST_IFTE,
    AST_WHILE,
    AST_RETURN,

    // operators
    AST_LE,
    AST_GE,
    AST_EQ,
    AST_NE,
    AST_AND,
    AST_OR,
    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV,
    AST_LESS,
    AST_GREATER
};

typedef struct ast_node {
    int type;
    HASH_NODE *symbol;
    struct ast_node_list *sons;
} AST_NODE;

typedef struct ast_node_list {
    AST_NODE *node;
    struct ast_node_list *next;
} AST_NODE_LIST;

AST_NODE *astCreate(int type, HASH_NODE *symbol, ...);

void astPrint(AST_NODE *node, int level);

#endif
