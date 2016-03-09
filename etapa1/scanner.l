/*
 * scanner.l
 */

%{
#include "tokens.h"

int _lineNumber;
int _running;

void initMe();
int getLineNumber();
int isRunning();
%}

%%
int						{ _running = 1; return KW_INT; }	
real					{ _running = 1; return KW_REAL; }
bool					{ _running = 1; return KW_BOOL ; }
char					{ _running = 1; return KW_CHAR; }
if						{ _running = 1; return KW_IF; }
else					{ _running = 1; return KW_ELSE; }
while					{ _running = 1; return KW_WHILE; }
input					{ _running = 1; return KW_INPUT; }
output					{ _running = 1; return KW_OUTPUT; }
return					{ _running = 1; return KW_RETURN; }
FALSE					{ _running = 1; return LIT_FALSE; }
TRUE					{ _running = 1; return LIT_TRUE; }
[-,;:()\[\]{}+*/<>=!&$]	{ _running = 1; return yytext[0]; }
\<=						{ _running = 1; return OPERATOR_LE; }
>=						{ _running = 1; return OPERATOR_GE; }
==						{ _running = 1; return OPERATOR_EQ; }
!=						{ _running = 1; return OPERATOR_NE; }
&&						{ _running = 1; return OPERATOR_AND; }
\|\|					{ _running = 1; return OPERATOR_OR; }
[a-zA-Z][a-zA-Z0-9_]*	{ _running = 1; return TK_IDENTIFIER; }
[0-9]+					{ _running = 1; return LIT_INTEGER; } /* incluir números negativos? -?[0-9]+ */
\'.\'					{ _running = 1; return LIT_CHAR; }
\"([^\"\\]|\\.)*\"		{ _running = 1; return LIT_STRING; } /* ainda em fase de testes */
\n						{ _running = 1; _lineNumber++; }
\/\/.*					{ _running = 1; } /* LINE COMMENT */
[\t ]					{ _running = 1; } /* WHITE SPACE */
.						{ _running = 1; return TOKEN_ERROR; }
%%

#include "main.c"

int yywrap() {
	_running = 0;
	return 1;
}

void initMe() {

}

int getLineNumber() {
	return _lineNumber;
}

int isRunning() {
	return _running;
}
