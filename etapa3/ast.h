/*
 * ast.h
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#ifndef AST_ 
#define AST_ 

#include "hashtable.h"

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
