/*
 * hashtable.h
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#ifndef HASHTABLE_
#define HASHTABLE_

#define HASH_SIZE 997

enum symbol_types {
    SYMBOL_LIT_INT,
    SYMBOL_LIT_CHAR,
    SYMBOL_LIT_STRING,
    SYMBOL_IDENTIFIER,
    SYMBOL_LIT_TRUE,
    SYMBOL_LIT_FALSE,
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
HASH_NODE *hashInsert(char *str, int type, int lineNumber);
HASH_NODE *hashFind(char *str, int type);
void hashPrint(void);

#endif
