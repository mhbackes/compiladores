#include <stdlib.h>
#include <stdarg.h>
#include "ast.h"

const char* _astString[] = {
    FOREACH_AST(GENERATE_AST_STRING)
};

void astPrintDotNodes(FILE *file, AST_NODE *node);
void astPrintDotEdges(FILE* file, AST_NODE* node);

AST_NODE *astCreate(int type, int lineNumber, HASH_NODE *symbol, int size, ...) {
    va_list args;
    AST_NODE *newNode; 

    if(!(newNode = (AST_NODE *) malloc(sizeof(AST_NODE)))) {
        fprintf(stderr, "ERROR [AST]: out of memory!\n"); exit(1); // abort
    }

    newNode->type = type;
    newNode->lineNumber = lineNumber;
    newNode->symbol = symbol;
    newNode->size = size;
    newNode->datatype = DTYPE_UNDEF;
    newNode->children = malloc(sizeof(AST_NODE *) * size);

    va_start(args, size);

    int i;
    for(i = 0; i < size; i++)
        newNode->children[i] = va_arg(args, AST_NODE *);

    va_end(args);

    return newNode;
}

void astPrintDot(FILE *file, AST_NODE *node) {
    fprintf(file, "digraph program {\n");
    astPrintDotNodes(file, node);
    astPrintDotEdges(file, node);
    fprintf(file, "}\n");
}

extern const char* _dataTypeString[];
void astPrintDotNodes(FILE *file, AST_NODE *node) {
    if(!node) return;
    fprintf(file, "\t\"%p\" [label=\"%s\\nDTYPE=%s\\nLINE=%d\"]\n", node, 
            _astString[node->type], _dataTypeString[node->datatype],
            node->lineNumber);
    if(node->symbol) {
        hashPrintDotNode(file, node->symbol);
    }
    int i;
    for(i = 0; i < node->size; i++){
        astPrintDotNodes(file, node->children[i]);
    }
}

void astPrintDotEdges(FILE* file, AST_NODE* node) {
    if(!node) return;
    if(node->symbol)
        fprintf(file, "\t\"%p\" -> \"%p\" [label=\"s\"]\n", node, node->symbol);
    int i;
    for(i = 0; i < node->size; i++){
        fprintf(file, "\t\"%p\" -> \"%p\" ", node, node->children[i]);
        fprintf(file, "[label=\"%d\"]\n", i);
        astPrintDotEdges(file, node->children[i]);
    }
}

/* not thread safe */
void astPrintCode(FILE* file, AST_NODE* node) {
    static int indent = 0;
    int i;
    if(!node) return;
    switch(node->type) {
        case AST_PROGRAM:
            for(i = 0; i < indent; i++)
                fprintf(file, "\t");
            astPrintCode(file, node->children[0]);
            if(node->children[1])
                astPrintCode(file, node->children[1]);
        case AST_LIT:
        case AST_VAR:
            if(node->symbol) {
                if(node->symbol->type == SYMBOL_LIT && 
                        node->symbol->datatype == DTYPE_STR) {
                    fprintf(file, "\"");
                    fprintf(file, "%s", node->symbol->text);
                    fprintf(file, "\"");
                    break;
                }
                if(node->symbol->type == SYMBOL_LIT &&
                        node->symbol->datatype == DTYPE_CHAR) {
                    fprintf(file, "\'");
                    fprintf(file, "%s", node->symbol->text);
                    fprintf(file, "\'");
                    break;
                }
                fprintf(file, "%s", node->symbol->text);
            }
            break;
        case AST_VARDEC:
            astPrintCode(file, node->children[0]);
            fprintf(file, " %s:", node->symbol->text);
            astPrintCode(file, node->children[1]);
            fprintf(file, ";\n");
            break;
        case AST_ARRDEC:
            astPrintCode(file, node->children[0]);
            fprintf(file, " %s[", node->symbol->text);
            astPrintCode(file, node->children[1]);
            fprintf(file, "]");
            if(node->children[2]) {
                fprintf(file, ": ");
                astPrintCode(file, node->children[2]);
            }
            fprintf(file, ";\n");
            break;
        case AST_FUNDEC:
            astPrintCode(file, node->children[0]);
            fprintf(file, " %s (", node->symbol->text);
            astPrintCode(file, node->children[1]);
            fprintf(file, ")");
            astPrintCode(file, node->children[2]);
            fprintf(file, ";\n");
            break;
        case AST_FUNCALL:
            fprintf(file, " %s(", node->symbol->text);
            if(node->children[0])
                astPrintCode(file, node->children[0]);
            fprintf(file, ")");
            break;
        case AST_ARRACCESS:
            fprintf(file, " %s[", node->symbol->text);
            astPrintCode(file, node->children[0]);
            fprintf(file, "]");
            break;
        case AST_LCMD:
            for(i = 0; i < indent; i++)
                fprintf(file, "\t");
            astPrintCode(file, node->children[0]);
            fprintf(file, "\n");
            if(node->children[1]) {
                astPrintCode(file, node->children[1]);
            }
            break;
        case AST_LPAR:
            astPrintCode(file, node->children[0]);
            fprintf(file, " %s", node->symbol->text);
            if(node->children[1]) {
                fprintf(file, ", ");
                astPrintCode(file, node->children[1]);
            }
            break;
        case AST_LLIT:
            astPrintCode(file, node->children[0]);
            if(node->children[1]) {
                fprintf(file, " ");
                astPrintCode(file, node->children[1]);
            }
            break;
        case AST_LEXP:
            astPrintCode(file, node->children[0]);
            if(node->children[1]) {
                fprintf(file, ", ");
                astPrintCode(file, node->children[1]);
            }
            break;
        case AST_LIN:
            fprintf(file, "%s", node->symbol->text);
            if(node->children[0]) {
                fprintf(file, ", ");
                astPrintCode(file, node->children[0]);
            }
            break;
        case AST_LOUT:
            astPrintCode(file, node->children[0]);
            if(node->children[1]) {
                fprintf(file, ", ");
                astPrintCode(file, node->children[1]);
            }
            break;
        case AST_ATTR:
            fprintf(file, "%s = ", node->symbol->text);
            astPrintCode(file, node->children[0]);
            break;
        case AST_ATTRARR:
            fprintf(file, "%s [", node->symbol->text);
            astPrintCode(file, node->children[0]);
            fprintf(file, "] = ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_INPUT:
            fprintf(file, "input ");
            astPrintCode(file, node->children[0]);
            break;
        case AST_OUTPUT:
            fprintf(file, "output ");
            astPrintCode(file, node->children[0]);
            break;
        case AST_IF:
            fprintf(file, "if (");
            astPrintCode(file, node->children[0]);	
            fprintf(file, ") ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_IFTE:
            fprintf(file, "if (");
            astPrintCode(file, node->children[0]);	
            fprintf(file, ") ");
            astPrintCode(file, node->children[1]);
            fprintf(file, " else ");
            astPrintCode(file, node->children[2]);
            break;
        case AST_WHILE:
            fprintf(file, "while (");
            astPrintCode(file, node->children[0]);
            fprintf(file, ") ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_RETURN:
            fprintf(file, "return ");
            astPrintCode(file, node->children[0]);
            break;
        case AST_EMPTY:
            fprintf(file, ";");
            break;
        case AST_NOT:
            fprintf(file, "!");
            astPrintCode(file, node->children[0]);
            break;
        case AST_LE:
            astPrintCode(file, node->children[0]);
            fprintf(file, " <= ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_GE:
            astPrintCode(file, node->children[0]);
            fprintf(file, " >= ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_EQ:
            astPrintCode(file, node->children[0]);
            fprintf(file, " == ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_NE:
            astPrintCode(file, node->children[0]);
            fprintf(file, " != ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_AND:
            astPrintCode(file, node->children[0]);
            fprintf(file, " && ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_OR:
            astPrintCode(file, node->children[0]);
            fprintf(file, " || ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_ADD:
            astPrintCode(file, node->children[0]);
            fprintf(file, " + ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_SUB:
            astPrintCode(file, node->children[0]);
            fprintf(file, " - ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_MUL:
            astPrintCode(file, node->children[0]);
            fprintf(file, " * ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_DIV:
            astPrintCode(file, node->children[0]);
            fprintf(file, " / ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_LESS:
            astPrintCode(file, node->children[0]);
            fprintf(file, " < ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_GREATER:
            astPrintCode(file, node->children[0]);
            fprintf(file, " > ");
            astPrintCode(file, node->children[1]);
            break;
        case AST_PAR:
            fprintf(file, "(");
            astPrintCode(file, node->children[0]);
            fprintf(file, ")");
            break;
        case AST_BLOCK:
            indent++;
            fprintf(file, "{\n");
            astPrintCode(file, node->children[0]);
            indent--;
            for(i = 0; i < indent; i++)
                fprintf(file, "\t");
            fprintf(file, "}");
            break;
        case AST_BOOL:
            fprintf(file, "bool");
            break;
        case AST_CHAR:
            fprintf(file, "char");
            break;
        case AST_INT:
            fprintf(file, "int");
            break;
        case AST_REAL:
            fprintf(file, "real");
            break;
        default:
            fprintf(stderr, "Unknown");
    }
}
