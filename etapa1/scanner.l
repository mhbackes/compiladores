/*
 * scanner.l
 */

%{
#include "tokens.h"
#include "hashtable.h"

int _lineNumber;
int _running;

void initMe();
int getLineNumber();
int isRunning();
%}

%x COMMENT

%%
         /* Initialization procedure (identation is needed) */
         static int initialized = 0;
     
         if (!initialized){
            initMe();
            initialized = 1;
         }

 /* KEYWORDS */
int						{ return KW_INT; }
real					{ return KW_REAL; }
bool					{ return KW_BOOL ; }
char					{ return KW_CHAR; }
if						{ return KW_IF; }
else					{ return KW_ELSE; }
while					{ return KW_WHILE; }
input					{ return KW_INPUT; }
output					{ return KW_OUTPUT; }
return					{ return KW_RETURN; }
 /* KEYWORDS */

 /* SPECIAL CHARACTERS */
[-,;:()\[\]{}+*/<>=!&$]	{ return yytext[0]; }
 /* SPECIAL CHARACTERS */

 /* OPERATORS */
\<=						{ return OPERATOR_LE; }
>=						{ return OPERATOR_GE; }
==						{ return OPERATOR_EQ; }
!=						{ return OPERATOR_NE; }
&&						{ return OPERATOR_AND; }
\|\|					{ return OPERATOR_OR; }
 /* OPERATORS */

 /* LITERALS */
FALSE					{ return LIT_FALSE; }
TRUE					{ return LIT_TRUE; }
[a-zA-Z][a-zA-Z0-9_]*	{ hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER; }
[0-9]+					{ hashInsert(yytext, LIT_INTEGER); return LIT_INTEGER; }
\'.\'					{ hashInsert(yytext, LIT_CHAR); return LIT_CHAR; }
\"([^\"\\\n]|\\.)*\"	{ hashInsert(yytext, LIT_STRING); return LIT_STRING; }
 /* LITERALS */

 /* LINE COMMENT */
\/\/.*					{ }
 /* LINE COMMENT */

 /* COMMENT */
\/\*					{ BEGIN(COMMENT); }
<COMMENT>\n				{ _lineNumber++; }
<COMMENT>\*\/			{ BEGIN(INITIAL); } 
<COMMENT>.				{ }
 /* COMMENT */

 /* WHITE SPACE */
\n						{ _lineNumber++; }
[\g ]					{ }
 /* WHITE SPACE */

 /* ERROR */
.						{ return TOKEN_ERROR; }
 /* ERROR */

%%

int yywrap() {
	_running = 0;
	return 1;
}

void initMe() {
	_lineNumber = 1;
	_running = 1;
    hashInit();
}

int getLineNumber() {
	return _lineNumber;
}

int isRunning() {
	return _running;
}
