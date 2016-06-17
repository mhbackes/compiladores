/*
 * parser.y
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN */ 

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "hashtable.h"
#include "lex.yy.h"
#include "error.h"

AST_NODE* root;

int yylex();
void yyerror(const char *s);
int getLineNumber();
int getLastTokenLineNumber();

%}

%union {
    HASH_NODE *symbol;
    AST_NODE *ast;
    int ln;
}

%token KW_INT       
%token KW_REAL      
%token KW_BOOL      
%token KW_CHAR      
%token KW_IF        
%token KW_ELSE      
%token KW_WHILE     
%token KW_INPUT     
%token KW_RETURN    
%token KW_OUTPUT    

%token OPERATOR_LE  
%token OPERATOR_GE  
%token OPERATOR_EQ  
%token OPERATOR_NE  
%token OPERATOR_AND 
%token OPERATOR_OR  

%token <symbol> TK_IDENTIFIER
%token <symbol> LIT_INTEGER  
%token <symbol> LIT_FALSE    
%token <symbol> LIT_TRUE	 
%token <symbol> LIT_CHAR     
%token <symbol> LIT_STRING   
%token <symbol> LIT_REAL /* gotta see about this */
%token TOKEN_ERROR  

%nonassoc OP_NOT
%nonassoc KW_IFX
%nonassoc KW_ELSE
%left OPERATOR_OR
%left OPERATOR_AND
%left OPERATOR_EQ OPERATOR_NE
%left '<' '>' OPERATOR_LE OPERATOR_GE %left '+' '-' %left '*' '/' %left '!'

%type<ast> program declaration global variable array listOfLiteral
%type<ast> function listOfParameters listOfExp exp
%type<ast> cmd attr input listOfInput output listOfOutput stringOrExp
%type<ast> if while block listOfCmd return literal type literalInteger literalBool
%type<ln> lineNumber

%%
root: program							{ root = $1; } /* MARCOS */

program: declaration					{ $$ = astCreate(AST_PROGRAM, $1->lineNumber, NULL, 2,$1, NULL); } /* PAULO */
       | declaration program			{ $$ = astCreate(AST_PROGRAM, $1->lineNumber, NULL, 2,$1, $2); }
       ;

declaration: global ';'					{ $$ = $1; } /* MARCOS */
           | function ';'				{ $$ = $1; } /* PAULO */
           ;

global: variable						{ $$ = $1; } /* MARCOS */
      | array							{ $$ = $1; } /* MARCOS */
      ;

variable: type TK_IDENTIFIER ':' literal	{ $$ = astCreate(AST_VARDEC, $1->lineNumber, $2, 2, $1, $4); } /* MARCOS */
        ;

type: KW_INT lineNumber					{ $$ = astCreate(AST_INT, $2, NULL, 0); } /* MARCOS */
    | KW_REAL lineNumber				{ $$ = astCreate(AST_REAL, $2, NULL, 0); } /* MARCOS */
    | KW_CHAR lineNumber				{ $$ = astCreate(AST_CHAR, $2, NULL, 0); } /* MARCOS */
    | KW_BOOL lineNumber				{ $$ = astCreate(AST_BOOL, $2, NULL, 0); } /* MARCOS */
    ;

literalBool: LIT_TRUE lineNumber		{ $$ = astCreate(AST_LIT, $2, $1, 0); }
           | LIT_FALSE lineNumber		{ $$ = astCreate(AST_LIT, $2, $1, 0); }
           ;

literalInteger: LIT_INTEGER	lineNumber	{ $$ = astCreate(AST_LIT, $2, $1, 0); } /* MARCOS */
              ;

literal: literalBool					{ $$ = $1; } /* MARCOS */
       | literalInteger					{ $$ = $1; } /* MARCOS */
       | LIT_REAL lineNumber			{ $$ = astCreate(AST_LIT, $2, $1, 0); } /* PAULO */
       | LIT_CHAR lineNumber			{ $$ = astCreate(AST_LIT, $2, $1, 0); } /* MARCOS */
       ;

array: type TK_IDENTIFIER '[' literalInteger ']'					{ $$ = astCreate(AST_ARRDEC, $1->lineNumber, $2, 3, $1, $4, NULL); } /* MARCOS */
     | type TK_IDENTIFIER '[' literalInteger ']' ':' listOfLiteral	{ $$ = astCreate(AST_ARRDEC, $1->lineNumber, $2, 3, $1, $4, $7); } /* PAULO */
     ;

listOfLiteral: literal					{ $$ = astCreate(AST_LLIT, $1->lineNumber, NULL, 2, $1, NULL); } /* PAULO */
             | literal listOfLiteral    { $$ = astCreate(AST_LLIT, $1->lineNumber, NULL, 2, $1, $2); }
             ;

function: type TK_IDENTIFIER '(' listOfParameters ')' cmd   { $$ = astCreate(AST_FUNDEC, $1->lineNumber, $2, 3, $1, $4, $6); } /* PAULO */
        | type TK_IDENTIFIER '(' ')' cmd                    { $$ = astCreate(AST_FUNDEC, $1->lineNumber, $2, 3, $1, NULL, $5); } 
        ;

listOfParameters: type TK_IDENTIFIER    { $$ = astCreate(AST_LPAR, $1->lineNumber, $2, 2, $1, NULL); } /* PAULO */
                | type TK_IDENTIFIER ',' listOfParameters { $$ = astCreate(AST_LPAR, $1->lineNumber, $2, 2, $1, $4);} /* PAULO */
                ;

exp: literal									{ $$ = $1; } /* MARCOS */
   | TK_IDENTIFIER lineNumber					{ $$ = astCreate(AST_VAR, $2, $1, 0); } /* MARCOS */
   | TK_IDENTIFIER lineNumber '[' exp ']'		{ $$ = astCreate(AST_ARRACCESS, $2, $1, 1, $4); } /* PAULO */
   | TK_IDENTIFIER lineNumber '(' listOfExp ')'	{ $$ = astCreate(AST_FUNCALL, $2, $1, 1, $4); } /* PAULO */
   | TK_IDENTIFIER lineNumber '(' ')'			{ $$ = astCreate(AST_FUNCALL, $2, $1, 1, NULL); } /* PAULO */
   | '(' lineNumber exp ')'						{ $$ = astCreate(AST_PAR, $2, NULL, 1, $3); } /* MARCOS */
   | '!' lineNumber exp %prec OP_NOT			{ $$ = astCreate(AST_NOT, $2, NULL, 1, $3); } /* MARCOS */
   | exp OPERATOR_LE exp						{ $$ = astCreate(AST_LE, $1->lineNumber, NULL, 2, $1, $3); } /* MARCOS */
   | exp OPERATOR_GE exp						{ $$ = astCreate(AST_GE, $1->lineNumber, NULL, 2, $1, $3); } /* MARCOS */
   | exp OPERATOR_EQ exp						{ $$ = astCreate(AST_EQ, $1->lineNumber, NULL, 2, $1, $3); } /* MARCOS */
   | exp OPERATOR_NE exp						{ $$ = astCreate(AST_NE, $1->lineNumber, NULL, 2, $1, $3); } /* MARCOS */
   | exp OPERATOR_AND exp						{ $$ = astCreate(AST_AND, $1->lineNumber, NULL, 2, $1, $3); } /* MARCOS */
   | exp OPERATOR_OR exp						{ $$ = astCreate(AST_OR, $1->lineNumber, NULL, 2, $1, $3); } /* MARCOS */
   | exp '+'exp									{ $$ = astCreate(AST_ADD, $1->lineNumber, NULL, 2, $1, $3); } /* MARCOS */
   | exp '-'exp									{ $$ = astCreate(AST_SUB, $1->lineNumber, NULL, 2, $1, $3); } /* MARCOS */
   | exp '*'exp									{ $$ = astCreate(AST_MUL, $1->lineNumber, NULL, 2, $1, $3); } /* MARCOS */
   | exp '/'exp									{ $$ = astCreate(AST_DIV, $1->lineNumber, NULL, 2, $1, $3); } /* MARCOS */
   | exp '<'exp									{ $$ = astCreate(AST_LESS, $1->lineNumber, NULL, 2, $1, $3); } /* MARCOS */
   | exp '>'exp									{ $$ = astCreate(AST_GREATER, $1->lineNumber, NULL, 2, $1, $3); } /* MARCOS */
   ;

listOfExp: exp							{ $$ = astCreate(AST_LEXP, $1->lineNumber, NULL, 2, $1, NULL); } /* MARCOS */
         | exp ',' listOfExp 		    { $$ = astCreate(AST_LEXP, $1->lineNumber, NULL, 2, $1, $3); }

cmd: attr								{ $$ = $1; } /* MARCOS */
   | input								{ $$ = $1; } /* MARCOS */
   | output								{ $$ = $1; } /* MARCOS */
   | if									{ $$ = $1; } /* MARCOS */
   | while								{ $$ = $1; } /* MARCOS */
   | block								{ $$ = $1; } /* MARCOS */
   | return								{ $$ = $1; } /* MARCOS */
   | ';' lineNumber						{ $$ = astCreate(AST_EMPTY, $2, NULL, 0); } /* PAULO */
   ;

attr: TK_IDENTIFIER lineNumber '=' exp				{ $$ = astCreate(AST_ATTR, $2, $1, 1, $4); } /* MARCOS */
    | TK_IDENTIFIER lineNumber '[' exp ']' '=' exp	{ $$ = astCreate(AST_ATTRARR, $2, $1, 2, $4, $7); } /* PAULO */
    ;


input: KW_INPUT lineNumber listOfInput		{ $$ = astCreate(AST_INPUT, $2, NULL, 1, $3); } /* MARCOS */
     ;

listOfInput: TK_IDENTIFIER lineNumber		{ $$ = astCreate(AST_LIN, $2, $1, 1, NULL); } /* MARCOS */
           | TK_IDENTIFIER lineNumber ',' listOfInput  { $$ = astCreate(AST_LIN, $2, $1, 1, $4); }
           ;

output: KW_OUTPUT lineNumber listOfOutput	{ $$ = astCreate(AST_OUTPUT, $2, NULL, 1, $3); } /* PAULO */
      ;


listOfOutput: stringOrExp					{ $$ = astCreate(AST_LOUT, $1->lineNumber, NULL, 2, $1, NULL); } /* PAULO */
            | stringOrExp ',' listOfOutput  { $$ = astCreate(AST_LOUT, $1->lineNumber, NULL, 2, $1, $3); }
            ;

stringOrExp: exp						{ $$ = $1; } 
           | LIT_STRING lineNumber		{ $$ = astCreate(AST_LIT, $2, $1, 0); } /* PAULO */
           ;


if: KW_IF lineNumber '(' exp ')' cmd %prec KW_IFX	{ $$ = astCreate(AST_IF, $2, NULL, 2, $4, $6);} /* PAULO */
  | KW_IF lineNumber '(' exp ')' cmd KW_ELSE cmd	{ $$ = astCreate(AST_IFTE, $2, NULL, 3, $4, $6, $8); } /* PAULO */
  ;

while: KW_WHILE lineNumber '(' exp ')' cmd	{ $$ = astCreate(AST_WHILE, $2, NULL, 2, $4, $6); } /* PAULO */
     ;

block: '{' lineNumber listOfCmd '}'			{ $$ = astCreate(AST_BLOCK, $2, NULL, 1, $3); } /* MARCOS */
     ;

listOfCmd: cmd							{ $$ = astCreate(AST_LCMD, $1->lineNumber, NULL, 2, $1, NULL); } /* MARCOS */
         | cmd listOfCmd 			    { $$ = astCreate(AST_LCMD, $1->lineNumber, NULL, 2, $1, $2); }
         ;

return: KW_RETURN exp					{ $$ = astCreate(AST_RETURN, $2->lineNumber, NULL, 1, $2); } /* PAULO */

/* empty production for counting last token's line number */
lineNumber:								{ $$ = getLastTokenLineNumber(); }

%%
void yyerror (char const *msg) {
    fprintf(stderr, "Syntax error in line %d.\n", getLineNumber());
    exit(EXIT_SYNTAX_ERROR);
}
