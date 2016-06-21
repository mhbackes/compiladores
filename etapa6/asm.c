/*
 * asm.c
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#include "asm.h"


#define FMT_INT ".FMT_INT"
#define FMT_REAL ".FMT_REAL"
#define FMT_STR ".FMT_STR"

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
void asmReturn(FILE *file, TAC *node);
void asmFuncall(FILE *file, TAC *node);

void asmPrint(FILE *file, TAC *node);
void asmPrintInt(FILE *file, HASH_NODE *node);
void asmPrintChar(FILE *file, HASH_NODE *node);
void asmPrintReal(FILE *file, HASH_NODE *node);
void asmPrintStr(FILE *file, HASH_NODE *node);

void asmAttr(FILE *file, TAC *node);

/*
 * Converts the value of var "node" to int and stores the result in
 * register e"reg"x - e.g. "eax".
 *
 * Param "reg" - must be 'a' or 'b'.
 *
 * Side-effects: register 'xmm2' is changed.
 */
void asmConvertToInt(FILE *file, HASH_NODE *node, char reg);

/*
 * Converts the value of var "node" to char and stores the result in
 * register "reg"l - e.g. "al".
 *
 * Param "reg" - must be 'a' or 'b'.
 *
 * Side-effects: register 'xmm2' is changed.
 */
void asmConvertToChar(FILE *file, HASH_NODE *node, char reg);

/*
 * Converts the value of var "node" to char and stores the result in
 * register xmm"reg" - e.g. "xmm0".
 *
 * Param "reg" - must be '0' or '1'.
 *
 * Side-effects: register 'ecx' is changed.
 */
void asmConvertToReal(FILE *file, HASH_NODE *node, char reg);

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
        switch(tmp->type) {
            //case TAC_MOVE:
                //break;
            case TAC_BEGINFUN:
                asmBeginFun(file, tmp);
                break;
            case TAC_ENDFUN:
                asmEndFun(file, tmp);
                break;
            case TAC_RET:
                asmReturn(file, tmp);
                break;
            case TAC_CALL:
                asmFuncall(file, tmp);
                break;
            //case TAC_IFZ:
                //break;
            //case TAC_JUMP:
                //break;
            //case TAC_ARG:
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
            case TAC_ATTR:
				asmAttr(file, tmp);
                break;
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
    fprintf(file, "\t.size\t%s, 4\n", node->name);
    fprintf(file, "%s:\n", node->name);
    int value = astGetDeclarationValue(node->declaration);
    fprintf(file, "\t.long\t%d\n", value);
}

void asmDeclareVarByte(FILE *file, HASH_NODE *node) {
    fprintf(file, "\t.size\t%s, 1\n", node->name);
    fprintf(file, "%s:\n", node->name);
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
    fprintf(file, "\t.size\t%s, 4\n", node->name);
    fprintf(file, "%s:\n", node->name);
    int value = hashGetValue(node);
    fprintf(file, "\t.long\t%d\n", value);
}

void asmDeclareLitByte(FILE *file, HASH_NODE *node) {
    fprintf(file, "\t.size\t%s, 1\n", node->name);
    fprintf(file, "%s:\n", node->name);
    int value = hashGetValue(node);
    fprintf(file, "\t.byte\t%d\n", value);
}

void asmDeclareLitStr(FILE *file, HASH_NODE *node) {
    fprintf(file, "%s:\n", node->name);
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
    fprintf(file, "\t.size\t%s, %d\n", node->name, size * 4);
    fprintf(file, "%s:\n", node->name);

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
    fprintf(file, "\t.size\t%s, %d\n", node->name, size);
    fprintf(file, "%s:\n", node->name);

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
    char *name = node->res->name;
    fprintf(file, "/* TAC_BEGINFUN \"%s\" */\n", name);
    fprintf(file, "\t.global\t %s\n", name);
    fprintf(file, "%s:\n", name);
    fprintf(file, "\t.cfi_startproc\n");
	fprintf(file, "\tpush\t%%rbp\n");
}

void asmEndFun(FILE *file, TAC *node) {
    char *name = node->res->name;
    fprintf(file, "/* TAC_ENDFUN \"%s\" */\n", name);
    fprintf(file, "\tmovl\t$0, %%eax\n");
    fprintf(file, "\tpopq\t%%rbp\n");
	fprintf(file, "\tret\n");
    fprintf(file, "\t.cfi_endproc\n");
}

void asmReturn(FILE *file, TAC *node) {
    HASH_NODE *retValue = node->res;
    HASH_NODE *funDec = node->op1;
    fprintf(file, "/* TAC_RET \"%s\" \"%s\" */\n",
                retValue->text, funDec->text);
    switch(funDec->datatype) {
	case DTYPE_CHAR:
        asmConvertToChar(file, retValue, 'a');
        break;
	case DTYPE_INT:
        asmConvertToInt(file, retValue, 'a');
        break;
	case DTYPE_REAL:
        asmConvertToReal(file, retValue, '0');
        break;
    case DTYPE_BOOL:
        fprintf(file, "\tmov\t%s(%%rip), %%al\n", retValue->name);
    }
    fprintf(file, "\tpopq\t%%rbp\n");
    fprintf(file, "\tret\n");
}

void asmFuncall(FILE *file, TAC *node) {
    HASH_NODE *res = node->res;
    HASH_NODE *funDec = node->op1;
    fprintf(file, "/* TAC_CALL \"%s\" \"%s\" */\n", res->text, funDec->text);
    fprintf(file, "\tcall\t%s\n", funDec->name);
    switch(res->datatype) {
	case DTYPE_CHAR:
        if(funDec->datatype == DTYPE_REAL)
		    fprintf(file, "\tcvttss2si\t%%eax, %%eax\n");
        fprintf(file, "\tmov\t\t%%al, %s(%%rip)\n", res->name);
        break;
	case DTYPE_INT:
        if(funDec->datatype == DTYPE_REAL)
		    fprintf(file, "\tcvttss2si\t%%eax, %%eax\n");
        fprintf(file, "\tmovl\t%%eax, %s(%%rip)\n", res->name);
        break;
	case DTYPE_REAL:
        if(funDec->datatype != DTYPE_REAL)
		    fprintf(file, "\tcvtsi2ss\t%%eax, %%xmm0\n");
        fprintf(file, "\tmovss\t%%xmm0, %s(%%rip)\n", res->name);
        break;
    case DTYPE_BOOL:
        fprintf(file, "\tmov\t%%al, %s(%%rip)\n", res->name);
    }
}

void asmFormat(FILE *file) {
    fprintf(file, "/* Formatters for printf */\n");
    fprintf(file, FMT_INT ":\n");
    fprintf(file, "\t.string\t\"%%d\"\n");
    fprintf(file, FMT_REAL ":\n");
    fprintf(file, "\t.string\t\"%%f\"\n");
    fprintf(file, FMT_STR ":\n");
    fprintf(file, "\t.string\t\"%%s\"\n");
}

void asmPrint(FILE *file, TAC *node) {
    HASH_NODE *hnode = node->op1;
    fprintf(file, "/* TAC_PRINT \"%s\" */\n", hnode->text);
    switch(hnode->datatype) {
        case DTYPE_INT:
            asmPrintInt(file, hnode);
            break;
        case DTYPE_CHAR:
            asmPrintChar(file, hnode);
            break;
        case DTYPE_REAL:
            asmPrintReal(file, hnode);
            break;
        case DTYPE_STR:
            asmPrintStr(file, hnode);
    }
}

void asmPrintInt(FILE *file, HASH_NODE *node) {
    fprintf(file, "\tmovl\t%s(%%rip), %%eax\n", node->name); 
    fprintf(file, "\tmovl\t%%eax, %%esi\n"); 
    fprintf(file, "\tmovl\t$" FMT_INT ", %%edi\n"); 
    fprintf(file, "\tmovl\t$0, %%eax\n"); 
    fprintf(file, "\tcall\tprintf\n"); 
}

void asmPrintChar(FILE *file, HASH_NODE *node) {
    fprintf(file, "\tmovzbl\t%s(%%rip), %%eax\n", node->name); 
    fprintf(file, "\tmovsbl\t%%al, %%eax\n"); 
    fprintf(file, "\tmovl\t%%eax, %%edi\n"); 
    fprintf(file, "\tcall\tputchar\n"); 
}

void asmPrintReal(FILE *file, HASH_NODE *node) {
    fprintf(file, "\tmovss\t%s(%%rip), %%xmm0\n", node->name); 
    fprintf(file, "\tcvtss2sd\t%%xmm0, %%xmm0\n"); 
    fprintf(file, "\tmovl\t$" FMT_REAL ", %%edi\n"); 
    fprintf(file, "\tmovl\t$1, %%eax\n"); 
    fprintf(file, "\tcall\tprintf\n"); 

}

void asmPrintStr(FILE *file, HASH_NODE *node) {
    fprintf(file, "\tmovl\t$%s, %%esi\n", node->name); 
    fprintf(file, "\tmovl\t$" FMT_STR ", %%edi\n"); 
    fprintf(file, "\tmovl\t$0, %%eax\n"); 
    fprintf(file, "\tcall\tprintf\n"); 
}

void asmConvertToInt(FILE *file, HASH_NODE *node, char reg) {
	switch(node->datatype) {
	case DTYPE_CHAR:
		fprintf(file, "\tmovzbl\t%s(%%rip), %%e%cx\n", node->name, reg);
		fprintf(file, "\tmovsbl\t%%%cl, %%e%cx\n", reg, reg);
		break;
	case DTYPE_INT:
		fprintf(file, "\tmovl\t%s(%%rip), %%e%cx\n", node->name, reg);
		break;
	case DTYPE_REAL:
		fprintf(file, "\tmovss\t%s(%%rip), %%xmm2\n", node->name);
		fprintf(file, "\tcvttss2si\t%%xmm2, %%e%cx\n", reg);
	}
}

void asmConvertToChar(FILE *file, HASH_NODE *node, char reg) {
	switch(node->datatype) {
	case DTYPE_CHAR:
		fprintf(file, "\tmovzbl\t%s(%%rip), %%e%cx\n", node->name, reg);
        break;
	case DTYPE_INT:
		fprintf(file, "\tmovl\t%s(%%rip), %%e%cx\n", node->name, reg);
		break;
	case DTYPE_REAL:
		fprintf(file, "\tmovss\t%s(%%rip), %%xmm2\n", node->name);
		fprintf(file, "\tcvttss2si\t%%xmm2, %%e%cx\n", reg);
	}
}

void asmConvertToReal(FILE *file, HASH_NODE *node, char reg) {
	switch(node->datatype) {
	case DTYPE_CHAR:
		fprintf(file, "\tmovzbl\t%s(%%rip), %%ecx\n", node->name);
		fprintf(file, "\tmovsbl\t%%cl, %%ecx\n");
		fprintf(file, "\tpxor\t%%xmm%c, %%xmm%c\n", reg, reg);
		fprintf(file, "\tcvtsi2ss\t%%ecx, %%xmm%c\n", reg);
		break;
	case DTYPE_INT:
		fprintf(file, "\tmovl\t%s(%%rip), %%ecx\n", node->name);
		fprintf(file, "\tpxor\t%%xmm%c, %%xmm%c\n", reg, reg);
		fprintf(file, "\tcvtsi2ss\t%%ecx, %%xmm%c\n", reg);
		break;
	case DTYPE_REAL:
		fprintf(file, "\tmovss\t%s(%%rip), %%xmm%c\n", node->name, reg);
	}
}

void asmAttr(FILE *file, TAC *node) {
    HASH_NODE* dst = node->res;
    HASH_NODE* src = node->op1;
    fprintf(file, "/* TAC_ATTR %s %s */\n", dst->text, src->text);
    switch(dst->datatype) {
	case DTYPE_CHAR:
        asmConvertToChar(file, src, 'a');
        fprintf(file, "\tmov\t\t%%al, %s(%%rip)\n", dst->name);
        break;
	case DTYPE_INT:
        asmConvertToInt(file, src, 'a');
        fprintf(file, "\tmovl\t%%eax, %s(%%rip)\n", dst->name);
        break;
	case DTYPE_REAL:
        asmConvertToReal(file, src, '0');
        fprintf(file, "\tmovss\t%%xmm0, %s(%%rip)\n", dst->name);
        break;
    case DTYPE_BOOL:
        fprintf(file, "\tmov\t%s(%%rip), %%al\n", src->name);
        fprintf(file, "\tmov\t%%al, %s(%%rip)\n", dst->name);
    }

}
