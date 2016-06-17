/*
 * asm.c
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#include "asm.h"

#define LITERAL "lit"

void asmWriteCode(FILE* file, TAC* tac) {
    asmDeclareVariables(file);
}

void asmDeclareVar(FILE *file, HASH_NODE *node);
void asmDeclareVarLong(FILE *file, HASH_NODE *node);
void asmDeclareVarByte(FILE *file, HASH_NODE *node);

void asmDeclareLit(FILE *file, HASH_NODE *node);
void asmDeclareLitLong(FILE *file, HASH_NODE *node);
void asmDeclareLitByte(FILE *file, HASH_NODE *node);
void asmDeclareLitStr(FILE *file, HASH_NODE *node);

void asmDeclareVariables(FILE* file) {
    int i;
    HASH_NODE* node;

    fprintf(file, "\t.data\n");
    for(i = 0; i < HASH_SIZE; i++) {
        node = _symbolTable[i];
        while(node) { 
            switch(node->type) {
                case SYMBOL_LIT:
                    asmDeclareLit(file, node);
                    break;
                case SYMBOL_SCALAR:
                    asmDeclareVar(file, node);
                    break;
            }
            fprintf(file, "\n");
            node = node->next;
        }
    }

}

void asmDeclareVar(FILE *file, HASH_NODE *node) {
    switch(node->datatype) {
        case DTYPE_INT:
        case DTYPE_REAL:
            asmDeclareVarLong(file, node);
            break;
        case DTYPE_CHAR:
        case DTYPE_BOOL:
            asmDeclareVarByte(file, node);
            break;

        default:
            fprintf(stderr, "Unknown var type in hash\n");
    }
}

void asmDeclareVarLong(FILE *file, HASH_NODE *node) {
    fprintf(file, "\t.align\t4\n");
    fprintf(file, "\t.size\t%s, 4\n", node->text);
    fprintf(file, "%s:\n", node->text);
    int value = astGetDeclarationValue(node->declaration);
    fprintf(file, "\t.long\t%d\n", value);
}

void asmDeclareVarByte(FILE *file, HASH_NODE *node) {
    fprintf(file, "\t.size\t%s, 1\n", node->text);
    fprintf(file, "%s:\n", node->text);
    int value = astGetDeclarationValue(node->declaration);
    fprintf(file, "\t.byte\t%d\n", value);
}

void asmDeclareLit(FILE *file, HASH_NODE *node) {
    switch(node->datatype) {
        case DTYPE_INT:
        case DTYPE_REAL:
            asmDeclareLitLong(file, node);
            break;
        case DTYPE_CHAR:
        case DTYPE_BOOL:
            asmDeclareLitByte(file, node);
            break;
        case DTYPE_STR:
            asmDeclareLitStr(file, node);
            break;

        default:
            fprintf(stderr, "Unknown var type in hash\n");
    }

}

void asmDeclareLitLong(FILE *file, HASH_NODE *node) {
    fprintf(file, "\t.align\t4\n");
    fprintf(file, "\t.size\t" LITERAL "%d, 4\n", node->id);
    fprintf(file, LITERAL "%d:\n", node->id);
    fprintf(file, "\t.long\t0\n");
}

void asmDeclareLitByte(FILE *file, HASH_NODE *node) {
    fprintf(file, "\t.size\t" LITERAL "%d, 1\n", node->id);
    fprintf(file, LITERAL "%d:\n", node->id);
    fprintf(file, "\t.byte\t0\n");
}

void asmDeclareLitStr(FILE *file, HASH_NODE *node) {
    fprintf(file, LITERAL "%d:\n", node->id);
    fprintf(file, "\t.string\t\"%s\"\n", node->text);
}
