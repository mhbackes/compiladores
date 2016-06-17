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
#include "y.tab.h"

#define TEMP_P  "#TEMP"
#define LABEL_P "#LABEL"

const char* _symbolTypeString[] = {
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
            !(newNode->text = (char *) malloc(sizeof(char) *  (strlen(str) + 1)))) {
        fprintf(stderr, "Error: out of memory\n");
        exit(-1); // abort
    }

    newNode->type = type;
    newNode->datatype = datatype;
    newNode->lineNumber = lineNumber;
    newNode->declaration = NULL;

    strncpy(newNode->text, str, strlen(str) + 1);

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

void hashClean(void) {
    int i;
    HASH_NODE *node, *tmp;
    for(i = 0; i < HASH_SIZE; i++) {
        node = _symbolTable[i];
        while(node) { 
            tmp = node->next;
            free(node->text);
            free(node);
            node = tmp;
        }
        _symbolTable[i] = NULL;
    }
}

/* testing purposes, should be removed */
void hashPrint(void) {
    int i;
    HASH_NODE *node;
    for(i = 0; i < HASH_SIZE; i++) {
        node = _symbolTable[i];
        while(node) { 
            printf("Table[%d] -> ", i);
            printf("TYPE: %s\t", _symbolTypeString[node->type]);
            printf("DATATYPE: %s\t", _dataTypeString[node->datatype]);
            printf("TEXT: %s\n", node->text);
            node = node->next;
        }
    }
    return;
}

void hashPrintDotNode(FILE* file, HASH_NODE *node) {
        fprintf(file, "\t\"%p\" [label=\"TEXT=\\\"%s\\\"\\n", node,
                node->text);
        fprintf(file, "STYPE=%s\\nDTYPE=%s\"", _symbolTypeString[node->type], 
                _dataTypeString[node->datatype]);
        fprintf(file, "shape=box]\n");
}

HASH_NODE *makeTemp(int dataType) {
    static int tmpIdx = 0;

    char *str = (char *) malloc(sizeof(TEMP_P) + 20 * sizeof(char)); 
    sprintf(str, "%s%d", TEMP_P, tmpIdx);
    tmpIdx++;

    HASH_NODE* temp =  hashInsert(str, SYMBOL_SCALAR, dataType, 0);
    free(str);
    return temp;

}

HASH_NODE *makeLabel() {
    static int labelIdx = 0;

    char *str = (char *) malloc(sizeof(LABEL_P) + 20 * sizeof(char)); 
    sprintf(str, "%s%d", LABEL_P, labelIdx);
    labelIdx++;

    HASH_NODE* label = hashInsert(str, SYMBOL_LABEL, DTYPE_UNDEF, 0);
    free(str);
    return label;
}
