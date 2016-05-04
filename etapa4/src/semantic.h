/*
 * hashtable.h
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#ifndef SEMANTIC
#define SEMANTIC

#include "hashtable.h"
#include "ast.h"

int checkDeclaration(AST_NODE  *node);
int checkUndeclared(HASH_NODE *hash);
int checkUsage(AST_NODE *node);

#endif
