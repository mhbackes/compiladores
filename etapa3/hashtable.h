/*
 * hashtable.h
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#ifndef HASHTABLE_
#define HASHTABLE_

#define HASH_SIZE 997

#define SYMBOL_LIT_INT		1
#define SYMBOL_LIT_CHAR		2
#define SYMBOL_LIT_STRING	3
#define SYMBOL_IDENTIFIER	4
#define SYMBOL_LIT_TRUE 5
#define SYMBOL_LIT_FALSE 6

typedef struct hash_node_struct {
    int type;
    char *text;
    struct hash_node_struct *next;
} HASH_NODE;

void hashInit(void);
HASH_NODE *hashInsert(char *str, int type);
HASH_NODE *hashFind(char *str, int type);
void hashPrint(void);

#endif
