/*
 * tac.h
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 * >>>>>>>>> CODE SPONSORED BY THE ORDER OF THE LABLE <<<<<<<<<<
 */

#ifndef TAC_
#define TAC_

#include "hashtable.h"
#include "ast.h"

#define FOREACH_TAC(TAC)\
    TAC(TAC_SYMBOL)     \
    TAC(TAC_LABEL)      \
    TAC(TAC_MOVE)       \
    TAC(TAC_BEGINFUN)   \
    TAC(TAC_ENDFUN)     \
    TAC(TAC_IFZ)        \
    TAC(TAC_WHILE)      \
    TAC(TAC_JUMP)       \
    TAC(TAC_CALL)       \
    TAC(TAC_ARG)        \
    TAC(TAC_RET)        \
    TAC(TAC_PRINT)      \
    TAC(TAC_READ)       \
    TAC(TAC_READARR)    \
    TAC(TAC_ATTR)       \
    TAC(TAC_ATTRARR)    \
    TAC(TAC_ADD)        \
    TAC(TAC_MUL)        \
    TAC(TAC_SUB)        \
    TAC(TAC_DIV)        \
    TAC(TAC_NOT)        \
    TAC(TAC_LE)         \
    TAC(TAC_GE)         \
    TAC(TAC_EQ)			\
    TAC(TAC_NE)			\
    TAC(TAC_AND)		\
    TAC(TAC_OR)			\
    TAC(TAC_LESS)		\
    TAC(TAC_GREATER)	

#define GENERATE_TAC_ENUM(ENUM) ENUM,
#define GENERATE_TAC_STRING(STRING) #STRING,
 
#define FOREACH_TEMP(TEMP)  \
    TEMP(LABEL_CODE, "#LABEL%d")  \
    TEMP(RES_CODE, "#RES%d")        

#define GENERATE_TEMP_ENUM(ENUM, STRING) ENUM,
#define GENERATE_TEMP_STRING(ENUM, STRING) STRING,


enum tac_types {
    FOREACH_TAC(GENERATE_TAC_ENUM)
};

enum temp_labels {
    FOREACH_TEMP(GENERATE_TEMP_ENUM)
};

typedef struct tac_s {
    int type;
    HASH_NODE *res, *op1, *op2;
    struct tac_s *prev, *next;
} TAC;

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);

TAC *tacJoin(int nof, TAC **tacs); 

TAC *generateCode(AST_NODE *node);
#endif
