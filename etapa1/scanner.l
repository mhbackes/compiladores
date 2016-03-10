/*
 * scanner.l
 */

%{
#include "tokens.h"
#include "hashtable.h"

int _lineNumber = 1;
int _running;

void initMe();
int getLineNumber();
int isRunning();
%}

%x COMMENT

%%
 /* KEYWORDS */
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
 /* KEYWORDS */

 /* SPECIAL CHARACTERS */
[-,;:()\[\]{}+*/<>=!&$]	{ _running = 1; return yytext[0]; }
 /* SPECIAL CHARACTERS */

 /* OPERATORS */
\<=						{ _running = 2; return OPERATOR_LE; }
>=						{ _running = 1; return OPERATOR_GE; }
==						{ _running = 1; return OPERATOR_EQ; }
!=						{ _running = 1; return OPERATOR_NE; }
&&						{ _running = 1; return OPERATOR_AND; }
\|\|					{ _running = 1; return OPERATOR_OR; }
 /* OPERATORS */

 /* LITERALS */
FALSE					{ _running = 1; return LIT_FALSE; }
TRUE					{ _running = 1; return LIT_TRUE; }
[a-zA-Z][a-zA-Z0-9_]*	{ _running = 1; hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER; }
[0-9]+					{ _running = 1; hashInsert(yytext, LIT_INTEGER); return LIT_INTEGER; }
\'.\'					{ _running = 1; return LIT_CHAR; }
\"([^\"\\\n]|\\.)*\"	{ _running = 1; return LIT_STRING; }
 /* LITERALS */

 /* LINE COMMENT */
\/\/.*					{ _running = 1; }
 /* LINE COMMENT */

 /* COMMENT */
\/\*					{ _running = 1; BEGIN(COMMENT); }
<COMMENT>\n				{ _running = 1; _lineNumber++; }
<COMMENT>\*\/			{ _running = 1; BEGIN(INITIAL); } 
<COMMENT>.				{ _running = 1; }
 /* COMMENT */

 /* WHITE SPACE */
\n						{ _running = 1; _lineNumber++; }
[\t ]					{ _running = 1; }
 /* WHITE SPACE */

 /* ERROR */
.						{ _running = 1; return TOKEN_ERROR; }
 /* ERROR */

%%

#include "main.c"

int yywrap() {
	_running = 0;
	return 1;
}

void initMe() {
	_lineNumber = 1;
    hashInit();
}

int getLineNumber() {
	return _lineNumber;
}

int isRunning() {
	return _running;
}
