/*
 * main.c
 */

#include <stdio.h>
#include "tokens.h"
#include "lex.yy.h"
#include "hashtable.h"

int main(int argc, char *argv[])
{
	int token = yylex();
	while (isRunning()) {
		switch (token) {
			case KW_INT:
				printf("Line: %d - KW_INT\n", getLineNumber());
				break;
			case KW_REAL:
				printf("Line: %d - KW_REAL\n", getLineNumber());
				break;
			case KW_BOOL:
				printf("Line: %d - KW_BOOL\n", getLineNumber());
				break;
			case KW_CHAR:
				printf("Line: %d - KW_CHAR\n", getLineNumber());
				break;
			case KW_IF:
				printf("Line: %d - KW_IF\n", getLineNumber());
				break;
			case KW_ELSE:
				printf("Line: %d - KW_ELSE\n", getLineNumber());
				break;
			case KW_WHILE:
				printf("Line: %d - KW_WHILE\n", getLineNumber());
				break;
			case KW_INPUT:
				printf("Line: %d - KW_INPUT\n", getLineNumber());
				break;
			case KW_RETURN:
				printf("Line: %d - KW_RETURN\n", getLineNumber());
				break;
			case KW_OUTPUT:
				printf("Line: %d - KW_OUTPUT\n", getLineNumber());
				break;
			case OPERATOR_LE:
				printf("Line: %d - OPERATOR_LE\n", getLineNumber());
				break;
			case OPERATOR_GE:
				printf("Line: %d - OPERATOR_GE\n", getLineNumber());
				break;
			case OPERATOR_EQ:
				printf("Line: %d - OPERATOR_EQ\n", getLineNumber());
				break;
			case OPERATOR_NE:
				printf("Line: %d - OPERATOR_NE\n", getLineNumber());
				break;
			case OPERATOR_AND:
				printf("Line: %d - OPERATOR_AND\n", getLineNumber());
				break;
			case OPERATOR_OR:
				printf("Line: %d - OPERATOR_OR\n", getLineNumber());
				break;
			case TK_IDENTIFIER:
				printf("Line: %d - TK_IDENTIFIER\n", getLineNumber());
				break;
			case LIT_INTEGER:
				printf("Line: %d - LIT_INTEGER\n", getLineNumber());
				break;
			case LIT_FALSE:
				printf("Line: %d - LIT_FALSE\n", getLineNumber());
				break;
			case LIT_TRUE:
				printf("Line: %d - LIT_TRUE\n", getLineNumber());
				break;
			case LIT_CHAR:
				printf("Line: %d - LIT_CHAR\n", getLineNumber());
				break;
			case LIT_STRING:
				printf("Line: %d - LIT_STRING\n", getLineNumber());
				break;
			case TOKEN_ERROR:
				printf("Line: %d - TOKEN_ERROR\n", getLineNumber());
				break;
			default:
				printf("Line: %d - %c\n", getLineNumber(), token);
				break;
		}
		token = yylex();
	}
	hashPrint();
	return 0;
}
