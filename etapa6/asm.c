/*
 * asm.c
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#include "asm.h"
#include <string.h>
#include "error.h"

#define SIZEOF_RIP_ACCESS 6

#define FMT_OUT_INT ".FMT_OUT_INT"
#define FMT_OUT_CHAR ".FMT_OUT_CHAR"
#define FMT_OUT_REAL ".FMT_OUT_REAL"
#define FMT_OUT_STR ".FMT_OUT_STR"

#define FMT_IN_INT ".FMT_IN_INT"
#define FMT_IN_CHAR ".FMT_IN_CHAR"
#define FMT_IN_REAL ".FMT_IN_REAL"

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
void asmArg(FILE *file, TAC *node);

/* ===>I/O OPS */

void asmPrint(FILE *file, TAC *node);
void asmPrintInt(FILE *file, HASH_NODE *node);
void asmPrintChar(FILE *file, HASH_NODE *node);
void asmPrintReal(FILE *file, HASH_NODE *node);
void asmPrintStr(FILE *file, HASH_NODE *node);

void asmInput(FILE *file, TAC *node);


/* ===>ATTR OPS */

void asmAttr(FILE *file, TAC *node);
void asmAttrAux(FILE *file, HASH_NODE *dst, HASH_NODE *src);

void asmAttrArr(FILE *file, TAC *tac);
void asmReadArr(FILE *file, TAC *tac);
int getOffset(HASH_NODE *idx); 

/* ===>CONVERSION */

/*
 * Converts the value of var "node(%sourceReg)" to int and stores the result in
 * register e"reg"x - e.g. "eax".
 *
 * Param "node" - might be empty (""). In that case, sourceReg value is used
 * Param "reg" - must be 'a' or 'b'.
 * Param "sourceReg" - at will
 *
 * Side-effects: register 'xmm2' is changed.
 */
void asmConvertToInt(FILE *file, int datatype, char *node, char reg, char *sourceReg);

/*
 * Converts the value of var "node(%sourceReg)" to char and stores the result in
 * register "reg"l - e.g. "al".
 *
 * Param "node" - might be empty (""). In that case, sourceReg value is used
 * Param "reg" - must be 'a' or 'b'.
 * Param "sourceReg" - at will
 *
 * Side-effects: register 'xmm2' is changed.
 */
void asmConvertToChar(FILE *file, int datatype, char *node, char reg, char *sourceReg);

/*
 * Converts the value of var "node(%sourceReg)" to S. Floating Point and stores 
 * the result in register xmm"reg" - e.g. "xmm0".
 *
 * Param "node" - might be empty (""). In that case, sourceReg value is used
 * Param "reg" - must be '0' or '1'.
 * Param "sourceReg" - at will
 *
 * Side-effects: register 'ecx' is changed.
 */
void asmConvertToReal(FILE *file, int datatype, char *node, char reg, char *sourceReg);

/* ===>FLOW CONTROL */
void asmLabel(FILE *file, TAC *tac); 

void asmJump(FILE *file, TAC *tac);

void asmIfz(FILE *file, TAC *tac);

/* ===>BOOLEAN OPS */

void asmLE(FILE *file, TAC *tac);

void asmGE(FILE *file, TAC *tac);

void asmEQ(FILE *file, TAC *tac);

void asmNE(FILE *file, TAC *tac);

void asmLESS(FILE *file, TAC *tac);

void asmGREATER(FILE *file, TAC *tac);

void asmAND(FILE *file, TAC *tac);

void asmOR(FILE *file, TAC *tac);

void asmBooleanComparison(FILE *file, TAC *tac, char *op);

int comparisonTypeFetch(HASH_NODE *op1, HASH_NODE *op2);

void asmLogicalBinary(FILE *file, TAC *tac, char *op);

/* ===>ARITHMETIC OPS */

void asmAdd(FILE *file, TAC *tac);

void asmSub(FILE *file, TAC *tac);

void asmMul(FILE *file, TAC *tac);

void asmDiv(FILE *file, TAC *tac);

void asmArithmeticBinary(FILE *file, TAC *tac, char *intOp, char *floatOp);

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
            case TAC_ARG:
                asmArg(file, tmp);
                break;
            case TAC_IFZ:
                asmIfz(file, tmp);
                break;
            case TAC_JUMP:
                asmJump(file, tmp);
                break;
            case TAC_PRINT:
                asmPrint(file, tmp);
                break;
            case TAC_INPUT:
                asmInput(file, tmp);
                break;
            case TAC_READARR:
                asmReadArr(file, tmp);
                break;
            case TAC_ATTR:
                asmAttr(file, tmp);
                break;
            case TAC_ATTRARR:
                asmAttrArr(file, tmp);
                break;
            case TAC_ADD:
                asmAdd(file, tmp);
                break;
            case TAC_MUL:
                asmMul(file, tmp);
                break;
            case TAC_SUB:
                asmSub(file, tmp);
                break;
            case TAC_DIV:
                asmDiv(file, tmp);
                break;
            //case TAC_NOT:
            //    break;
            case TAC_LE:
                asmLE(file, tmp);
                break;
            case TAC_GE:
                asmGE(file, tmp);
                break;
            case TAC_EQ:
                asmEQ(file, tmp);
                break;
            case TAC_NE:
                asmNE(file, tmp);
                break;
            case TAC_LESS:
                asmLESS(file, tmp);
                break;
            case TAC_GREATER:
                asmGREATER(file, tmp);
                break;
            case TAC_AND:
                asmAND(file, tmp);
                break;
            case TAC_OR:
                asmOR(file, tmp);
                break;
            case TAC_LABEL:
                asmLabel(file, tmp);
                break;
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
            asmConvertToChar(file, retValue->datatype, retValue->name, 'a', "rip");
            break;
        case DTYPE_INT:
            asmConvertToInt(file, retValue->datatype, retValue->name, 'a', "rip");
            break;
        case DTYPE_REAL:
            asmConvertToReal(file, retValue->datatype, retValue->name, '0', "rip");
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

void asmArg(FILE *file, TAC *node) {
    HASH_NODE* dst = node->res;
    HASH_NODE* src = node->op1;
    fprintf(file, "/* TAC_ARG %s %s */\n", dst->text, src->text);
    asmAttrAux(file, dst, src);
}

void asmFormat(FILE *file) {
    fprintf(file, "/* Formatters for printf and scanf */\n");
    fprintf(file, FMT_OUT_INT ":\n");
    fprintf(file, "\t.string\t\"%%d\"\n");
    fprintf(file, FMT_OUT_CHAR ":\n");
    fprintf(file, "\t.string\t\"%%c\"\n");
    fprintf(file, FMT_OUT_REAL ":\n");
    fprintf(file, "\t.string\t\"%%f\"\n");
    fprintf(file, FMT_OUT_STR ":\n");
    fprintf(file, "\t.string\t\"%%s\"\n");
    fprintf(file, FMT_IN_INT ":\n");
    fprintf(file, "\t.string\t\" %%d\"\n");
    fprintf(file, FMT_IN_CHAR ":\n");
    fprintf(file, "\t.string\t\" %%c\"\n");
    fprintf(file, FMT_IN_REAL ":\n");
    fprintf(file, "\t.string\t\" %%f\"\n");
}

/* ===>I/O OPS */
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
    fprintf(file, "\tmovl\t$" FMT_OUT_INT ", %%edi\n"); 
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
    fprintf(file, "\tmovl\t$" FMT_OUT_REAL ", %%edi\n"); 
    fprintf(file, "\tmovl\t$1, %%eax\n"); 
    fprintf(file, "\tcall\tprintf\n"); 

}

void asmPrintStr(FILE *file, HASH_NODE *node) {
    fprintf(file, "\tmovl\t$%s, %%esi\n", node->name); 
    fprintf(file, "\tmovl\t$" FMT_OUT_STR ", %%edi\n"); 
    fprintf(file, "\tmovl\t$0, %%eax\n"); 
    fprintf(file, "\tcall\tprintf\n"); 
}

void asmInput(FILE *file, TAC *node) {
    HASH_NODE *hnode = node->op1;
    fprintf(file, "/* TAC_INPUT \"%s\" */\n", hnode->text);
    fprintf(file, "\tmovl\t$%s, %%esi\n", hnode->name); 
    switch(hnode->datatype) {
        case DTYPE_INT:
            fprintf(file, "\tmovl\t$" FMT_IN_INT ", %%edi\n"); 
            break;
        case DTYPE_CHAR:
            fprintf(file, "\tmovl\t$" FMT_IN_CHAR ", %%edi\n"); 
            break;
        case DTYPE_REAL:
            fprintf(file, "\tmovl\t$" FMT_IN_REAL ", %%edi\n"); 
    }
    fprintf(file, "\tmovl\t$0, %%eax\n"); 
    fprintf(file, "\tcall\tscanf\n"); 
}

/* Builds an ASM var access string if node is not blank or an ASM regster 
 * access string otherwise.
 * 
 * The returned string/pointer MUST BE FREED.
 */
char *srcStrBuild(char *node, char *sourceReg) {
    char *srcStr = (char *) malloc(sizeof(char) * (strlen(node) + strlen(sourceReg) + 4));

    if(!strcmp(node, ""))
        sprintf(srcStr, "%%%s", sourceReg);
    else
        sprintf(srcStr, "%s(%%%s)", node, sourceReg);

    return srcStr;
                                        
}
/* ===>CONVERSION */
void asmConvertToInt(FILE *file, int datatype, char *node, char reg, char *sourceReg) {
    char *srcStr = srcStrBuild(node, sourceReg);

    switch(datatype) {
        case DTYPE_CHAR:
            fprintf(file, "\tmovzx\t%s, %%e%cx\n", srcStr, reg);
            //fprintf(file, "\tmovsbl\t%%%cl, %%e%cx\n", reg, reg);
            break;
        case DTYPE_INT:
            fprintf(file, "\tmovl\t%s, %%e%cx\n", srcStr, reg);
            break;
        case DTYPE_REAL:
            fprintf(file, "\tmovss\t%s, %%xmm2\n", srcStr); 
            fprintf(file, "\tcvttss2si\t%%xmm2, %%e%cx\n", reg);
    }

    free(srcStr);
}

void asmConvertToChar(FILE *file, int datatype, char *node, char reg, char *sourceReg) {
    char *srcStr = srcStrBuild(node, sourceReg);

    switch(datatype) {
        case DTYPE_CHAR:
            fprintf(file, "\tmovzx\t%s, %%e%cx\n", srcStr, reg);
            break;
        case DTYPE_INT:
            fprintf(file, "\tmovl\t%s, %%e%cx\n", srcStr, reg);
            break;
        case DTYPE_REAL:
            fprintf(file, "\tmovss\t%s, %%xmm2\n", srcStr);
            fprintf(file, "\tcvttss2si\t%%xmm2, %%e%cx\n", reg);
    }

    free(srcStr);
}

void asmConvertToReal(FILE *file, int datatype, char *node, char reg, char *sourceReg) {
    char *srcStr = srcStrBuild(node, sourceReg);

    switch(datatype) {
        case DTYPE_CHAR:
            fprintf(file, "\tmovzx\t%s, %%ecx\n", srcStr);
            //fprintf(file, "\tmovsbl\t%%cl, %%ecx\n");
            fprintf(file, "\tpxor\t%%xmm%c, %%xmm%c\n", reg, reg);
            fprintf(file, "\tcvtsi2ss\t%%ecx, %%xmm%c\n", reg);
            break;
        case DTYPE_INT:
            fprintf(file, "\tmovl\t%s, %%ecx\n", srcStr);
            fprintf(file, "\tpxor\t%%xmm%c, %%xmm%c\n", reg, reg);
            fprintf(file, "\tcvtsi2ss\t%%ecx, %%xmm%c\n", reg);
            break;
        case DTYPE_REAL:
            fprintf(file, "\tmovss\t%s, %%xmm%c\n", srcStr, reg);
    }

    free(srcStr);
}

/* ===>ATTR OPS */
void asmAttr(FILE *file, TAC *node) {
    HASH_NODE* dst = node->res;
    HASH_NODE* src = node->op1;
    fprintf(file, "/* TAC_ATTR %s %s */\n", dst->text, src->text);
    asmAttrAux(file, dst, src);
}

void asmAttrAux(FILE *file, HASH_NODE *dst, HASH_NODE *src) {
    switch(dst->datatype) {
        case DTYPE_CHAR:
            asmConvertToChar(file, src->datatype, src->name, 'a', "rip");
            fprintf(file, "\tmov\t\t%%al, %s(%%rip)\n", dst->name);
            break;
        case DTYPE_INT:
            asmConvertToInt(file, src->datatype, src->name, 'a', "rip");
            fprintf(file, "\tmovl\t%%eax, %s(%%rip)\n", dst->name);
            break;
        case DTYPE_REAL:
            asmConvertToReal(file, src->datatype, src->name, '0', "rip");
            fprintf(file, "\tmovss\t%%xmm0, %s(%%rip)\n", dst->name);
            break;
        case DTYPE_BOOL:
            fprintf(file, "\tmov\t%s(%%rip), %%al\n", src->name);
            fprintf(file, "\tmov\t%%al, %s(%%rip)\n", dst->name);
    }
}

void asmReadArr(FILE *file, TAC *tac) {
    HASH_NODE *dst = tac->res;
    HASH_NODE *src= tac->op1;
    HASH_NODE *idx= tac->op2;
    fprintf(file, "/* TAC_READARR %s[%s] -> %s */\n", src->text, idx->text, 
            dst->text);

    switch(dst->datatype) {
        case DTYPE_CHAR:
            fprintf(file, "\tmovl\t%s(%%rip), %%eax\n", idx->name);
            fprintf(file, "\tcltq\n");
            fprintf(file, "\tmov\t%s(,%%rax,4), %%bl\n", src->name);
            asmConvertToChar(file, src->datatype, "", 'a', "bl");
            fprintf(file, "\tmov\t%%al, %s(%%rip)\n", dst->name);
            break;
        case DTYPE_INT:
            fprintf(file, "\tmovl\t%s(%%rip), %%eax\n", idx->name);
            fprintf(file, "\tcltq\n");
            fprintf(file, "\tmovl\t%s(,%%rax,4), %%eax\n", src->name);
            asmConvertToInt(file, src->datatype, "", 'a', "eax");
            fprintf(file, "\tmovl\t%%eax, %s(%%rip)\n", dst->name);
            break;
        case DTYPE_REAL:
            fprintf(file, "\tmovl\t%s(%%rip), %%eax\n", idx->name);
            fprintf(file, "\tcltq\n");
            fprintf(file, "\tmovss\t%s(,%%rax,4), %%xmm0\n", src->name);
            asmConvertToReal(file, src->datatype, "", '1', "xmm0");
            fprintf(file, "\tmovss\t%%xmm1, %s(%%rip)\n", dst->name);
            break;
        case DTYPE_BOOL:
            fprintf(file, "\tmovl\t%s(%%rip), %%eax\n", idx->name);
            fprintf(file, "\tcltq\n");
            fprintf(file, "\tmov\t%s(,%%rax,4), %%al\n", src->name);
            fprintf(file, "\tmov\t%%al, %s(%%rip)\n", dst->name);
    }
}

void asmAttrArr(FILE *file, TAC *tac) {
    HASH_NODE *dst = tac->res;
    HASH_NODE *idx = tac->op1;
    HASH_NODE *val = tac->op2;

    fprintf(file, "/* TAC_ATTRARR %s[%s] = %s */\n", dst->text, idx->text, val->text);

    switch(dst->datatype) {
        case DTYPE_CHAR:
            asmConvertToChar(file, val->datatype, val->name, 'a', "rip");
            fprintf(file, "\tmovl\t%s(%%rip), %%eax\n", idx->name);
            fprintf(file, "\tcltq\n");
            fprintf(file, "\tmov\t%%al, %s(,%%rax,4)\n", dst->name);
            break;
        case DTYPE_INT:
            asmConvertToInt(file, val->datatype, val->name, 'b', "rip");
            fprintf(file, "\tmovl\t%s(%%rip), %%eax\n", idx->name);
            fprintf(file, "\tcltq\n");
            fprintf(file, "\tmovl\t%%ebx, %s(,%%rax,4)\n", dst->name);
            break;
        case DTYPE_REAL:
            asmConvertToReal(file, val->datatype, val->name, '0', "rip");
            fprintf(file, "\tmovl\t%s(%%rip), %%eax\n", idx->name);
            fprintf(file, "\tcltq\n");
            fprintf(file, "\tmovss\t%%xmm0, %s(,%%rax,4)\n", dst->name);
            break;
        case DTYPE_BOOL:
            fprintf(file, "\tmov\t%s(%%rip), %%al\n", val->name);
            fprintf(file, "\tmovl\t%s(%%rip), %%eax\n", idx->name);
            fprintf(file, "\tcltq\n");
            fprintf(file, "\tmov\t%%al, %s(,%%rax,4)\n", dst->name);
    }
}


/* ===>ARITHMETIC OPS */

void asmAdd(FILE *file, TAC *tac) {
    fprintf(file, "/* TAC_SUB %s = %s - %s */\n", tac->res->text, 
            tac->op1->text, tac->op2->text);
    asmArithmeticBinary(file, tac, "addl", "addss");
}

void asmSub(FILE *file, TAC *tac) {
    fprintf(file, "/* TAC_SUB %s = %s - %s */\n", tac->res->text, 
            tac->op1->text, tac->op2->text);
    asmArithmeticBinary(file, tac, "subl", "subss");
}

void asmMul(FILE *file, TAC *tac) {
    fprintf(file, "/* TAC_MUL %s = %s * %s */\n", tac->res->text, 
            tac->op1->text, tac->op2->text);
    asmArithmeticBinary(file, tac, "imul", "mulss");
}

void asmDiv(FILE *file, TAC *tac) {
    fprintf(file, "/* TAC_DIV %s = %s / %s */\n", tac->res->text, 
            tac->op1->text, tac->op2->text);
    asmArithmeticBinary(file, tac, "idiv", "divss");
}

void asmArithmeticBinary(FILE *file, TAC *tac, char *intOp, char *floatOp) {
    HASH_NODE *dst = tac->res;
    HASH_NODE *op1 = tac->op1;
    HASH_NODE *op2 = tac->op2;

    switch(dst->datatype) {
        case DTYPE_CHAR:
            asmConvertToChar(file, op2->datatype, op2->name, 'a', "rip");
            fprintf(file, "\tmovzx\t\t%%al, %%eax\n");
            asmConvertToChar(file, op1->datatype, op1->name, 'b', "rip");
            fprintf(file, "\tmovzx\t\t%%bl, %%ebx\n");
            fprintf(file, "\t%s\t%%eax, %%ebx\n", intOp);
            fprintf(file, "\tmovl\t%%ebx, %s(%%rip)\n", dst->name);
            break;
        case DTYPE_INT:
            asmConvertToInt(file, op2->datatype, op2->name, 'a', "rip");
            asmConvertToInt(file, op1->datatype, op1->name, 'b', "rip");
            fprintf(file, "\t%s\t%%eax, %%ebx\n", intOp);
            fprintf(file, "\tmovl\t%%ebx, %s(%%rip)\n", dst->name);
            break;
        case DTYPE_REAL:
            asmConvertToReal(file, op2->datatype, op2->name, '0', "rip");
            asmConvertToReal(file, op1->datatype, op1->name, '1', "rip");
            fprintf(file, "\t%s\t%%xmm0, %%xmm1\n", floatOp);
            fprintf(file, "\tmovss\t%%xmm1, %s(%%rip)\n", dst->name);
            break;
        case DTYPE_BOOL:
            break;
    }
}

/* ===>FLOW CONTROL */
void asmLabel(FILE *file, TAC *tac) {
    fprintf(file, "%s:\n", tac->res->name);
}

void asmJump(FILE *file, TAC *tac) {
    HASH_NODE *target = tac->res;

    fprintf(file, "/* TAC_JUMP %s */\n", target->name);
    fprintf(file, "\tjmp %s\n", target->name);
}

void asmIfz(FILE *file, TAC *tac) {
    HASH_NODE *target = tac->res;
    HASH_NODE *exp = tac->op1;

    fprintf(file, "/* TAC_IF (%s) THEN (%s) */\n", exp->name, target->name);
    fprintf(file, "\tmov\t\t%s(%%rip), %%al\n", exp->name);
    fprintf(file, "\ttest\t%%al, %%al\n");
    fprintf(file, "\tje\t\t%s\n", target->name);
}

/* ===>BOOLEAN OPS */

void asmLE(FILE *file, TAC *tac) {
    fprintf(file, "/* TAC_LE %s = %s <= %s */\n", tac->res->text, 
            tac->op1->text, tac->op2->text);
    asmBooleanComparison(file, tac, "setle");
}

void asmGE(FILE *file, TAC *tac) {
    fprintf(file, "/* TAC_GE %s = %s >= %s */\n", tac->res->text, 
            tac->op1->text, tac->op2->text);
    asmBooleanComparison(file, tac, "setnb");

}

void asmEQ(FILE *file, TAC *tac) {
    fprintf(file, "/* TAC_LE %s = %s == %s */\n", tac->res->text, 
            tac->op1->text, tac->op2->text);
    asmBooleanComparison(file, tac, "sete");

}

void asmNE(FILE *file, TAC *tac) {
    fprintf(file, "/* TAC_LE %s = %s != %s */\n", tac->res->text, 
            tac->op1->text, tac->op2->text);
    asmBooleanComparison(file, tac, "setne");

}

void asmLESS(FILE *file, TAC *tac) {
    fprintf(file, "/* TAC_LE %s = %s < %s */\n", tac->res->text, 
            tac->op1->text, tac->op2->text);
    asmBooleanComparison(file, tac, "setb");

}

void asmGREATER(FILE *file, TAC *tac) {
    fprintf(file, "/* TAC_LE %s = %s > %s */\n", tac->res->text, 
            tac->op1->text, tac->op2->text);
    asmBooleanComparison(file, tac, "setnle");
}

// op: the ASM operation that sets the comparison flag
void asmBooleanComparison(FILE *file, TAC *tac, char *op) {
    HASH_NODE *dst = tac->res;
    HASH_NODE *op1 = tac->op1;
    HASH_NODE *op2 = tac->op2;
    int datatype = comparisonTypeFetch(op1, op2);

    switch(datatype) {
        case DTYPE_CHAR:
            asmConvertToChar(file, op2->datatype, op2->name, 'a', "rip");
            asmConvertToChar(file, op1->datatype, op1->name, 'b', "rip");
            fprintf(file, "\tcmp\t%%al, %%bl\n");
            fprintf(file, "\t%s\t%%al\n", op);    // op -> comparison flag
            fprintf(file, "\tmov\t\t%%al, %s(%%rip)\n", dst->name);
            break;
        case DTYPE_INT:
            asmConvertToInt(file, op2->datatype, op2->name, 'a', "rip");
            asmConvertToInt(file, op1->datatype, op1->name, 'b', "rip");
            fprintf(file, "\tcmp\t%%eax, %%ebx\n");
            fprintf(file, "\t%s\t%%al\n", op);    // op -> comparison flag
            fprintf(file, "\tmov\t\t%%al, %s(%%rip)\n", dst->name);
            break;
        case DTYPE_REAL:
            asmConvertToReal(file, op2->datatype, op2->name, '0', "rip");
            asmConvertToReal(file, op1->datatype, op1->name, '1', "rip");
            fprintf(file, "\tucomiss\t%%xmm0, %%xmm1\n");
            fprintf(file, "\t%s\t%%al\n", op);    // op -> comparison flag
            fprintf(file, "\tmov\t\t%%al, %s(%%rip)\n", dst->name);
            break;
        case DTYPE_UNDEF:
            break; // furou a embarcaçao
    }
}

int comparisonTypeFetch(HASH_NODE *op1, HASH_NODE *op2) {
    int type1 = op1->datatype;
    int type2 = op2->datatype;

    switch(type1) {
        case DTYPE_CHAR:
            if(type2 == DTYPE_CHAR)
                return DTYPE_CHAR;
            else
                return type2;
        case DTYPE_INT:
            if(type2 == DTYPE_REAL)
                return DTYPE_REAL;
            else
                return type1;
        case DTYPE_REAL:
            return DTYPE_REAL;
    }
    
    return(DTYPE_UNDEF);
}

void asmAND(FILE *file, TAC *tac) {
    fprintf(file, "/* TAC_AND %s = %s AND %s */\n", tac->res->text, 
            tac->op1->text, tac->op2->text);
    asmLogicalBinary(file, tac, "and");
}

void asmOR(FILE *file, TAC *tac) {
    fprintf(file, "/* TAC_AND %s = %s AND %s */\n", tac->res->text, 
            tac->op1->text, tac->op2->text);
    asmLogicalBinary(file, tac, "or");
}

void asmLogicalBinary(FILE *file, TAC *tac, char *op) {
    HASH_NODE *dst = tac->res;
    HASH_NODE *op1 = tac->op1;
    HASH_NODE *op2 = tac->op2;

    fprintf(file, "\tmov\t%s(%%rip), %%al\n", op1->name);
    fprintf(file, "\tmov\t%s(%%rip), %%bl\n", op2->name);
    fprintf(file, "\t%s\t%%al, %%bl\n", op);
    fprintf(file, "\tmov\t%%bl, %s(%%rip)\n", dst->name);
}

