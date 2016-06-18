/*
 * asm.c
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#include "asm.h"

#define LITERAL ".LIT"

#define FMT_INT ".FMT_INT"
#define FMT_CHAR ".FMT_CHAR"
#define FMT_BOOL ".FMT_BOOL"
#define FMT_REAL ".FMT_REAL"

/* PROTOTYPES */

/* ===>GENERAL */
void asmWriteCodeAux(FILE *file, TAC *tac);

/* ===>DECLARATION */

void asmDeclareVariables(FILE* file);

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

void asmFormat(FILE *file);

void asmBeginFun(FILE *file, TAC *node);
void asmEndFun(FILE *file, TAC *node);

void asmPrint(FILE *file, TAC *node);

/* ===>OP ARGS ASSEMBLING */
char *asmSourceVar(HASH_NODE *s) {
    return NULL;
}

char *asmDestinationVar(HASH_NODE *d) {
    return NULL;
}

/* ===>BOOLEAN OPS */

/* ===>ARITHMETIC OPS */

/* ===>I/O OPS */

/* ===>ATTR OPS */


/* PUBLIC CODE */

void asmWriteCode(FILE *file, TAC *tac) {
    asmDeclareVariables(file);
    asmFormat(file);
    asmWriteCodeAux(file, tac);
}

/* PRIVATE CODE */

void asmWriteCodeAux(FILE* file, TAC* tac) {
    TAC *tmp;

    fprintf(file, "\t.text\n");
    for(tmp = tac; tmp; tmp = tmp->next) {
        switch(tac->type) {
            //case TAC_MOVE:
                //break;
            case TAC_BEGINFUN:
                asmBeginFun(file, tac);
                break;
            case TAC_ENDFUN:
                asmEndFun(file, tac);
                break;
            //case TAC_IFZ:
                //break;
            //case TAC_JUMP:
                //break;
            //case TAC_CALL:
                //break;
            //case TAC_ARG:
                //break;
            //case TAC_RET:
                //break;
            case TAC_PRINT:
				asmPrint(file, tmp);
                break;
            //case TAC_INPUT:
                //break;
            //case TAC_READ:
                //break;
            //case TAC_READARR:
                //break;
            //case TAC_ATTR:
                //break;
            //case TAC_ATTRARR:
                //break;
            //case TAC_ADD:
                //break;
            //case TAC_MUL:
                //break;
            //case TAC_SUB:
                //break;
            //case TAC_DIV:
                //break;
            //case TAC_NOT:
                //break;
            //case TAC_LE:
                //break;
            //case TAC_GE:
                //break;
            //case TAC_EQ:
                //break;
            //case TAC_NE:
                //break;
            //case TAC_AND:
                //break;
            //case TAC_OR:
                //break;
            //case TAC_LESS:
                //break;
            //case TAC_GREATER:
                //break;
        }
    }
}

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

void asmBeginFun(FILE *file, TAC *node) {
    char *text = node->res->text;
    fprintf(file, "/* Begin Function \"%s\" */\n", text);
    fprintf(file, "\t.global\t %s\n", text);
    fprintf(file, "%s:\n", text);
    fprintf(file, "\t.cfi_startproc\n");
	fprintf(file, "\tpush\t%%rbp\n");
}

void asmEndFun(FILE *file, TAC *node) {
    char *text = node->res->text;
    fprintf(file, "/* End Function \"%s\" */\n", text);
    fprintf(file, "\tpopq\t%%rbp\n");
	fprintf(file, "\tret\n");
    fprintf(file, "\t.cfi_endproc\n");
}

void asmFormat(FILE *file) {
    fprintf(file, "/* Formatters for printf */\n");
    fprintf(file, FMT_INT ":\n");
    fprintf(file, "\t.string\t\"%%d\"\n");
    fprintf(file, FMT_CHAR ":\n");
    fprintf(file, "\t.string\t\"%%c\"\n");
    fprintf(file, FMT_BOOL ":\n");
    fprintf(file, "\t.string\t\"%%i\"\n");
    fprintf(file, FMT_REAL ":\n");
    fprintf(file, "\t.string\t\"%%f\"\n");
}

/* not ready yet */
void asmPrint(FILE *file, TAC *node) {
    HASH_NODE *hnode = node->op1;
    fprintf(file, "/* TAC_PRINT \"%s\" */\n", hnode->text);
    fprintf(file, "\tmovl\t%s(%%rip), %%eax\n", hnode->text); 
    fprintf(file, "\tmovl\t%%eax, %%esi\n"); 
    fprintf(file, "\tmovl\t$" FMT_INT ", %%edi\n"); 
    fprintf(file, "\tmovl\t$0, %%eax\n"); 
    fprintf(file, "\tcall\tprintf\n"); 
}
