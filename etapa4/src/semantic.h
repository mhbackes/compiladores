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

#define FOREACH_ERROR(ERROR)					\
    ERROR(SEM_REDECLARED, "ID %s REDECLARED")	\
    ERROR(SEM_UNDECLARED, "ID %s UNDECLARED")	\
    ERROR(SEM_USAGE, "ID %s WRONG USAGE")		\

#define GENERATE_ERROR_ENUM(ENUM, STRING) ENUM,
#define GENERATE_ERROR_STRING(ENUM, STRING) STRING,

enum sem_error {
    FOREACH_ERROR(GENERATE_ERROR_ENUM)
};

int checkDeclaration(AST_NODE  *node);
int checkUndeclared(HASH_NODE **hash);
int checkUsage(AST_NODE *node);

void semError(int error, int line, char *msg);

#endif
