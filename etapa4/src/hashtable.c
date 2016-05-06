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

HASH_NODE *hashInsert(char *str, int type, int lineNumber) {
    int address;
    HASH_NODE *newNode;

    // if it is a string or char, we should strip it before anything else
    if(type == SYMBOL_LIT_STRING || type == SYMBOL_LIT_CHAR) {
        str++; 
        str[strlen(str) - 1] = '\0';
    }

    // "str" is already on hashtable
    if((newNode = hashFind(str, type)))
        return newNode;
    // new node allocation
    if(!(newNode = (HASH_NODE *) malloc(sizeof(HASH_NODE))) ||
            !(newNode->text = (char *) malloc(sizeof(char) *  strlen(str)))) {
        fprintf(stderr, "Error: out of memory\n");
        exit(1); // abort?
    }

    newNode->type = type;
    newNode->datatype = -1;
    newNode->lineNumber = lineNumber;

    strncpy(newNode->text, str, strlen(str));

    address = hashAddress(str);
    newNode->next = _symbolTable[address];
    _symbolTable[address] = newNode;

    return newNode;
}

HASH_NODE *hashFind(char *str, int type) {
    int address = hashAddress(str);
    HASH_NODE *node = _symbolTable[address];

    while(node) {
        if(!strncmp(node->text, str, strlen(str)) && node->type == type)
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
            switch(node->type) {
                case SYMBOL_IDENTIFIER:
                    printf("Table[%d] -> TYPE: SYMBOL_IDENTIFIER\tTEXT: %s\n", i, node->text);
                    break;
                case SYMBOL_LIT_INT:
                    printf("Table[%d] -> TYPE: SYMBOL_LIT_INT\tTEXT: %s\n", i, node->text);
                    break;
                case SYMBOL_LIT_CHAR:
                    printf("Table[%d] -> TYPE: SYMBOL_LIT_CHAR\tTEXT: %s\n", i, node->text);
                    break;
                case SYMBOL_LIT_STRING:
                    printf("Table[%d] -> TYPE: SYMBOL_LIT_STRING\tTEXT: %s\n", i, node->text);
                    break;
            }
            node = node->next;
        }
    }
    return;
}
