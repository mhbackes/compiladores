/*
 * hashtable.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "tokens.h"

/* PROTOTYPES */
HASH_NODE *hashFind(char *str, int type);
int hashAddress(char *str);
HASH_NODE *hashNewNode(char *str, int type); 

/* GLOBAL VARIABLES */
HASH_NODE *_symbolTable[HASH_SIZE];

void hashInit(void) {
	int i;

	for(i = 0; i < HASH_SIZE; i++)
		_symbolTable[i] = NULL; 
	return;
}

int hashAddress(char *str) {
	int i;
	int address = 1;

	for(i = 0; i < strlen(str); i++)
		address = address * str[i] % HASH_SIZE + 1;

	return address - 1;
}

HASH_NODE *hashInsert(char *str, int type) {
	int address;
	HASH_NODE *newNode;

	// if it is a string, we should strip it before anything else
	if(type == LIT_STRING) {
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
		printf("Table[%d] -> TYPE: %d STR: %s\n", i, node->type, node->text);
		node = node->next;
	}
    }
    return;
}

