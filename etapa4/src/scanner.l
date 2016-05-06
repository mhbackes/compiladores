/*
 * scanner.l
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

%{
#include "ast.h"
#include "hashtable.h"
#include "y.tab.h"

int _lineNumber;
int _lastTokenLineNumber;
int _running;

void initMe();
int getLineNumber();
int getLastTokenLineNumber();
int isRunning();
int token(int tk);
%}

%option nounput
%option noinput

%x COMMENT

%%
         /* Initialization procedure (indentation is needed) */
         static int initialized = 0;

         if (!initialized){
            initMe();
            initialized = 1;
         }

 /* KEYWORDS */
int                         { return token(KW_INT); }
real                        { return token(KW_REAL); }
bool                        { return token(KW_BOOL); }
char                        { return token(KW_CHAR); }
if                          { return token(KW_IF); }
else                        { return token(KW_ELSE); }
while                       { return token(KW_WHILE); }
input                       { return token(KW_INPUT); }
output                      { return token(KW_OUTPUT); }
return                      { return token(KW_RETURN); }
 /* KEYWORDS */

 /* SPECIAL CHARACTERS */
[-,;:()\[\]{}+*/<>=!&$]     { return token(yytext[0]); }
 /* SPECIAL CHARACTERS */

 /* OPERATORS */
\<=                         { return token(OPERATOR_LE); }
>=                          { return token(OPERATOR_GE); }
==                          { return token(OPERATOR_EQ); }
!=                          { return token(OPERATOR_NE); }
&&                          { return token(OPERATOR_AND); }
\|\|                        { return token(OPERATOR_OR); }
 /* OPERATORS */

 /* LITERALS */
FALSE                       {
                                yylval.symbol = hashInsert(yytext, SYMBOL_LIT, 
                                                    DTYPE_BOOL, _lineNumber);
                                return token(LIT_FALSE); 
                            }
TRUE                        {
                                yylval.symbol = hashInsert(yytext, SYMBOL_LIT,
                                                     DTYPE_BOOL, _lineNumber);
                                return token(LIT_TRUE);
                            }
[a-zA-Z][a-zA-Z0-9_]*       { 
                                yylval.symbol = hashInsert(yytext, SYMBOL_UNDEF,
                                                    DTYPE_UNDEF, _lineNumber);
                                return token(TK_IDENTIFIER); }
[0-9]+                      { 
                                yylval.symbol = hashInsert(yytext, SYMBOL_LIT,
                                                    DTYPE_INT, _lineNumber);
                                return token(LIT_INTEGER); 
                            }
\'.\'                       { 
                                yylval.symbol = hashInsert(yytext, SYMBOL_LIT,
                                                    DTYPE_CHAR, _lineNumber); 
                                return token(LIT_CHAR);
                            }
\"([^\"\\\n]|\\.)*\"        { 
                                yylval.symbol = hashInsert(yytext, SYMBOL_LIT,
                                                    DTYPE_CHAR, _lineNumber); 
                                return token(LIT_STRING);
                            }
 /* LITERALS */

 /* LINE COMMENT */
\/\/.*                      { }
 /* LINE COMMENT */

 /* COMMENT */
\/\*                        { BEGIN(COMMENT); }
<COMMENT>\n                 { _lineNumber++; }
<COMMENT>\*\/               { BEGIN(INITIAL); } 
<COMMENT>.                  { }
 /* COMMENT */

 /* WHITE SPACE */
\n                          { _lineNumber++; }
[\t ]                       { }
 /* WHITE SPACE */

 /* ERROR */
.                           { return token(TOKEN_ERROR); }
 /* ERROR */

%%

int yywrap() {
    _running = 0;
    return 1;
}

void initMe() {
    _lineNumber = 1;
    _lastTokenLineNumber = 1;
    _running = 1;
    hashInit();
}

int getLineNumber() {
    return _lineNumber;
}

int getLastTokenLineNumber() {
    return _lastTokenLineNumber;
}

int token(int tk) {
    _lastTokenLineNumber = _lineNumber;
    return tk;
}

int isRunning() {
    return _running;
}
