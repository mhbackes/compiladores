/*
 * hashtable.h
 */

#ifndef HASHTABLE_
#define HASHTABLE_

#define HASH_SIZE 997


typedef struct hash_node_struct {
    int type;
    char *text;
    struct hash_node_struct *next;
} HASH_NODE;

void hashInit(void);
int hashAddress(char *str);
HASH_NODE *hashNewNode(char *str, int type);
HASH_NODE *hashInsert(char *str, int type);
void hashPrint(void);

#endif
