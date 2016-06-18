/*
 * asm.c
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#include "asm.h"

#define LITERAL ".LIT"

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

void asmDeclareArr(FILE *file, HASH_NODE *node);
void asmDeclareArrLong(FILE *file, HASH_NODE *node);
void asmDeclareArrByte(FILE *file, HASH_NODE *node);

void asmDeclareVariables(FILE* file) {
    int i;
    HASH_NODE* node;

    fprintf(file, "\t.data\n");
    for(i = 0; i < HASH_SIZE; i++) {
        node = _symbolTable[i];
        while(node) { 
            switch(node->type) {
                case SYMBOL_LIT:
                    fprintf(file, "/* Literal \"%s\" */\n", node->text);
                    asmDeclareLit(file, node);
                    break;
                case SYMBOL_SCALAR:
                    fprintf(file, "/* Variable \"%s\" */\n", node->text);
                    asmDeclareVar(file, node);
                    break;
                case SYMBOL_VECTOR:
                    fprintf(file, "/* Vector \"%s\" */\n", node->text);
                    asmDeclareArr(file, node);
                    break;
            }
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
            fprintf(stderr, "Unknown lit type in hash\n");
    }

}

void asmDeclareLitLong(FILE *file, HASH_NODE *node) {
    fprintf(file, "\t.align\t4\n");
    fprintf(file, "\t.size\t" LITERAL "%d, 4\n", node->id);
    fprintf(file, LITERAL "%d:\n", node->id);
    int value = hashGetValue(node);
    fprintf(file, "\t.long\t%d\n", value);
}

void asmDeclareLitByte(FILE *file, HASH_NODE *node) {
    fprintf(file, "\t.size\t" LITERAL "%d, 1\n", node->id);
    fprintf(file, LITERAL "%d:\n", node->id);
    int value = hashGetValue(node);
    fprintf(file, "\t.byte\t%d\n", value);
}

void asmDeclareLitStr(FILE *file, HASH_NODE *node) {
    fprintf(file, LITERAL "%d:\n", node->id);
    fprintf(file, "\t.string\t\"%s\"\n", node->text);
}

void asmDeclareArr(FILE *file, HASH_NODE *node) {
    switch(node->datatype) {
        case DTYPE_INT:
        case DTYPE_REAL:
            asmDeclareArrLong(file, node);
            break;
        case DTYPE_CHAR:
        case DTYPE_BOOL:
            asmDeclareArrByte(file, node);
            break;

        default:
            fprintf(stderr, "Unknown array type in hash\n");
    }

}

void asmDeclareArrLong(FILE *file, HASH_NODE *node) {
    AST_NODE *decl = node->declaration;
    if(decl->type != AST_ARRDEC)
        return;

    int size = astGetLiteralValue(decl->children[1]);
    fprintf(file, "\t.align\t4\n");
    fprintf(file, "\t.size\t%s, %d\n", node->text, size * 4);
    fprintf(file, "%s:\n", node->text);

    int i;
    AST_NODE* llit = decl->children[2];
    for(i = 0; i < size; i++) {
        if(llit) {
            int value = astGetLiteralValue(llit->children[0]);
            if(node->datatype == DTYPE_REAL) {
                union { int i; float f; } uValue;
                uValue.f = value;
                value = uValue.i;
            }
            fprintf(file, "\t.long\t%d\n", value);
            llit = llit->children[1];
        } else 
            fprintf(file, "\t.long\t0\n");
    }
}

void asmDeclareArrByte(FILE *file, HASH_NODE *node) {
    AST_NODE *decl = node->declaration;
    if(decl->type != AST_ARRDEC)
        return;

    int size = astGetLiteralValue(decl->children[1]);
    fprintf(file, "\t.align\t4\n");
    fprintf(file, "\t.size\t%s, %d\n", node->text, size);
    fprintf(file, "%s:\n", node->text);

    int i;
    AST_NODE* llit = decl->children[2];
    for(i = 0; i < size; i++) {
        if(llit) {
            int value = astGetLiteralValue(llit->children[0]);
            fprintf(file, "\t.byte\t%d\n", value);
            llit = llit->children[1];
        } else 
            fprintf(file, "\t.byte\t0\n");
    }
}

