/*
 * hashtable.h
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#ifndef HASHTABLE_
#define HASHTABLE_

#define HASH_SIZE 997

#define GENERATE_HASH_ENUM(ENUM) ENUM,
#define GENERATE_HASH_STRING(STRING) #STRING,

#define FOREACH_SYMBOL_TYPE(SYMBOL) \
    SYMBOL(SYMBOL_UNDEF)            \
    SYMBOL(SYMBOL_LIT)              \
    SYMBOL(SYMBOL_SCALAR)           \
    SYMBOL(SYMBOL_VECTOR)           \
    SYMBOL(SYMBOL_FUNC)             \

enum hash_symbol_type {
    FOREACH_SYMBOL_TYPE(GENERATE_HASH_ENUM)
};

#define FOREACH_DATA_TYPE(SYMBOL)   \
    SYMBOL(DTYPE_UNDEF)             \
    SYMBOL(DTYPE_INT)               \
    SYMBOL(DTYPE_CHAR)              \
    SYMBOL(DTYPE_STR)               \
    SYMBOL(DTYPE_BOOL)              \

enum hash_data_type {
    FOREACH_DATA_TYPE(GENERATE_HASH_ENUM)
};

typedef struct hash_node_struct {
    int type;
    int datatype;
    int lineNumber;
    char *text;
    struct hash_node_struct *next;
} HASH_NODE;

/* GLOBAL VARIABLES */
HASH_NODE *_symbolTable[HASH_SIZE];

void hashInit(void);
HASH_NODE *hashInsert(char *str, int type, int datatype, int lineNumber);
HASH_NODE *hashFind(char *str, int datatype);
void hashPrint(void);

#endif
