/*
 * hashtable.c
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "ast.h"
#include "y.tab.h"

const char* _symboTypelString[] = {
    FOREACH_SYMBOL_TYPE(GENERATE_HASH_STRING)
};

const char* _dataTypeString[] = {
    FOREACH_DATA_TYPE(GENERATE_HASH_STRING)
};

/* PROTOTYPES */
int hashAddress(char *str);
HASH_NODE *hashNewNode(char *str, int type); 


void hashInit(void) {
    int i;

    for(i = 0; i < HASH_SIZE; i++)
        _symbolTable[i] = NULL; 
    return;
}

int hashAddress(char *str) {
    unsigned i;
    int address = 1;

    for(i = 0; i < strlen(str); i++)
        address = address * str[i] % HASH_SIZE + 1;

    return address - 1;
}

HASH_NODE *hashInsert(char *str, int type, int datatype, int lineNumber) {
    int address;
    HASH_NODE *newNode;

    // if it is a string or char, we should strip it before anything else
    if(type == SYMBOL_LIT && 
            (datatype == DTYPE_STR || datatype == DTYPE_CHAR)) {
        str++; 
        str[strlen(str) - 1] = '\0';
    }

    // "str" is already on hashtable
    if((newNode = hashFind(str, datatype)))
        return newNode;
    // new node allocation
    if(!(newNode = (HASH_NODE *) malloc(sizeof(HASH_NODE))) ||
            !(newNode->text = (char *) malloc(sizeof(char) *  strlen(str)))) {
        fprintf(stderr, "Error: out of memory\n");
        exit(1); // abort?
    }

    newNode->type = type;
    newNode->datatype = datatype;
    newNode->lineNumber = lineNumber;

    strncpy(newNode->text, str, strlen(str));

    address = hashAddress(str);
    newNode->next = _symbolTable[address];
    _symbolTable[address] = newNode;

    return newNode;
}

HASH_NODE *hashFind(char *str, int datatype) {
    int address = hashAddress(str);
    HASH_NODE *node = _symbolTable[address];

    while(node) {
        if(!strncmp(node->text, str, strlen(str)) && node->datatype == datatype)
            return node;
        node = node->next;
    }

    return NULL;
}

/* testing purposes, should be removed */
void hashPrint(void) {
    int i;
    HASH_NODE *node;
    for(i = 0; i < HASH_SIZE; i++) {
        node = _symbolTable[i];
        while(node) { 
            printf("Table[%d] -> ", i);
            printf("TYPE: %s\t", _symboTypelString[node->type]);
            printf("DATATYPE: %s\t", _dataTypeString[node->datatype]);
            printf("TEXT: %s\n", node->text);
            node = node->next;
        }
    }
    return;
}
