/*
 * parser.y
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "hashtable.h"
#include "lex.yy.h"

int yylex();
void yyerror(const char *s);
int getLineNumber();

AST_NODE* root;
%}

%union {
    HASH_NODE *symbol;
    AST_NODE *ast;
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
%left '<' '>' OPERATOR_LE OPERATOR_GE 
%left '+' '-'
%left '*' '/'
%left '!'

%type<ast> program declaration global variable array listOfLiteral
%type<ast> function functionHeader listOfParameters listOfExp exp
%type<ast> cmd attr input listOfInput output listOfOutput stringOrExp
%type<ast> if while block listOfCmd return literal type literalInteger

%%
root: program							{ root = $1; } /* MARCOS */

program: declaration					{ $$ = NULL; } /* TODO */
	   | program declaration			{ $$ = NULL; } /* TODO */
	   ;

declaration: global ';'					{ $$ = $1; } /* MARCOS */
		   | function ';'				{ $$ = NULL; } /* TODO */
		   ;

global: variable						{ $$ = $1; } /* MARCOS */
	  | array							{ $$ = $1; } /* MARCOS */
	  ;

variable: type TK_IDENTIFIER ':' literal	{ $$ = astCreate(AST_VARDEC, $2, 2, $1, $4); } /* MARCOS */
	    ;

type: KW_INT							{ $$ = astCreate(AST_INT, NULL, 0); } /* MARCOS */
	| KW_REAL							{ $$ = astCreate(AST_REAL, NULL, 0); } /* MARCOS */
	| KW_CHAR							{ $$ = astCreate(AST_CHAR, NULL, 0); } /* MARCOS */
	| KW_BOOL							{ $$ = astCreate(AST_BOOL, NULL, 0); } /* MARCOS */
	;

literalBool: LIT_TRUE
		   | LIT_FALSE
		   ;

literalInteger: LIT_INTEGER				{ $$ = astCreate(AST_SYMBOL, $1, 0); } /* MARCOS */
			  ;

literal: literalBool					{ $$ = NULL; } /* TODO */
	   | literalInteger					{ $$ = $1; } /* MARCOS */
       | LIT_REAL                       { $$ = astCreate(AST_SYMBOL, $1, 0); } /* PAULO */
	   | LIT_CHAR						{ $$ = astCreate(AST_SYMBOL, $1, 0); } /* MARCOS */
	   ;

array: type TK_IDENTIFIER '[' literalInteger ']'					{ $$ = astCreate(AST_ARRDEC, $2, 2, $1, $4, NULL); } /* MARCOS */
	 | type TK_IDENTIFIER '[' literalInteger ']' ':' listOfLiteral	{ $$ = astCreate(AST_ARRDEC, $2, 3, $1, $4, $7); } /* TODO */
	 ;

listOfLiteral: literal					{ $$ = astCreate(AST_LLIT, NULL, 2, $1, NULL); } /* PAULO */
			 | listOfLiteral literal	{ $$ = astCreate(AST_LLIT, NULL, 2, $2, $1); } /* PAULO */
			 ;

function: functionHeader cmd			{ $$ = astCreate(AST_FUNDEC, NULL, 2, $1, $2); } /* PAULO */
		;

functionHeader: type TK_IDENTIFIER '(' listOfParameters ')'	{ $$ = astCreate(AST_FUNHD, $2, 2, $1, $4); } /* PAULO */
			  | type TK_IDENTIFIER '(' ')'					{ $$ = astCreate(AST_FUNHD, $2, 1, $1); } /* PAULO */
              ;


listOfParameters: type TK_IDENTIFIER    { $$ = astCreate(AST_LPAR, $2, 2, $1, NULL); } /* PAULO */
                | listOfParameters ',' type TK_IDENTIFIER	{ $$ = astCreate(AST_LPAR, $4, 2, $3, $1); } /* PAULO */
                ;


exp: literal							{ $$ = $1; } /* MARCOS */
   | TK_IDENTIFIER						{ $$ = astCreate(AST_SYMBOL, $1, 0); } /* MARCOS */
   | TK_IDENTIFIER '[' exp ']'			{ $$ = astCreate(AST_ARRACESS, $1, 1, $3); } /* PAULO */
   | TK_IDENTIFIER '(' listOfExp ')'	{ $$ = astCreate(AST_FUNCALL, $1, 1, $3); } /* PAULO */
   | TK_IDENTIFIER '(' ')'				{ $$ = astCreate(AST_FUNCALL, $1, 1, NULL); } /* PAULO - TODO REVIEW*/
   | '(' exp ')'						{ $$ = astCreate(AST_PAR, NULL, 1, $2); } /* MARCOS */
   | '!' exp %prec OP_NOT				{ $$ = astCreate(AST_NOT, NULL, 1, $2); } /* MARCOS */
   | exp OPERATOR_LE exp				{ $$ = astCreate(AST_LE, NULL, 2, $1, $3); } /* MARCOS */
   | exp OPERATOR_GE exp				{ $$ = astCreate(AST_GE, NULL, 2, $1, $3); } /* MARCOS */
   | exp OPERATOR_EQ exp				{ $$ = astCreate(AST_EQ, NULL, 2, $1, $3); } /* MARCOS */
   | exp OPERATOR_NE exp				{ $$ = astCreate(AST_NE, NULL, 2, $1, $3); } /* MARCOS */
   | exp OPERATOR_AND exp				{ $$ = astCreate(AST_AND, NULL, 2, $1, $3); } /* MARCOS */
   | exp OPERATOR_OR exp				{ $$ = astCreate(AST_OR, NULL, 2, $1, $3); } /* MARCOS */
   | exp '+'exp							{ $$ = astCreate(AST_ADD, NULL, 2, $1, $3); } /* MARCOS */
   | exp '-'exp							{ $$ = astCreate(AST_SUB, NULL, 2, $1, $3); } /* MARCOS */
   | exp '*'exp							{ $$ = astCreate(AST_MUL, NULL, 2, $1, $3); } /* MARCOS */
   | exp '/'exp							{ $$ = astCreate(AST_DIV, NULL, 2, $1, $3); } /* MARCOS */
   | exp '<'exp							{ $$ = astCreate(AST_LESS, NULL, 2, $1, $3); } /* MARCOS */
   | exp '>'exp							{ $$ = astCreate(AST_GREATER, NULL, 2, $1, $3); } /* MARCOS */
   ;

listOfExp: exp							{ $$ = astCreate(AST_LEXP, NULL, 2, $1, NULL); } /* MARCOS */
		 | listOfExp ',' exp			{ $$ = astCreate(AST_LEXP, NULL, 2, $3, $1); } /* MARCOS */

cmd: attr								{ $$ = $1; } /* MARCOS */
   | input								{ $$ = $1; } /* MARCOS */
   | output								{ $$ = $1; } /* MARCOS */
   | if									{ $$ = $1; } /* MARCOS */
   | while								{ $$ = $1; } /* MARCOS */
   | block								{ $$ = $1; } /* MARCOS */
   | return								{ $$ = $1; } /* MARCOS */
   | ';'								{ $$ = NULL; } /* TODO */
   ;

attr: TK_IDENTIFIER '=' exp				{ $$ = astCreate(AST_ATTR, $1, 1, $3); } /* MARCOS */
    | TK_IDENTIFIER '[' exp ']' '=' exp	{ $$ = astCreate(AST_ATTRARR, $1, 2, $3, $6); } /* PAULO */
    ;


input: KW_INPUT listOfInput				{ $$ = astCreate(AST_INPUT, NULL, 1, $2); } /* MARCOS */
     ;

listOfInput: TK_IDENTIFIER					{ $$ = astCreate(AST_LIN, $1, 1, NULL); } /* MARCOS */
		   | listOfInput ',' TK_IDENTIFIER	{ $$ = astCreate(AST_LIN, $3, 1, $1); } /* MARCOS */
		   ;

output: KW_OUTPUT listOfOutput			{ $$ = astCreate(AST_OUTPUT, NULL, 1, $2); } /* PAULO */
      ;


listOfOutput: stringOrExp					{ $$ = astCreate(AST_LOUT, NULL, 2, $1, NULL); } /* PAULO */
			| listOfOutput ',' stringOrExp	{ $$ = astCreate(AST_LOUT, NULL, 2, $3, $1); } /* PAULO */
			;

stringOrExp: exp						{ $$ = $1; } 
		   | LIT_STRING					{ $$ = astCreate(AST_SYMBOL, $1, 0); } /* PAULO */
		   ;
			

if: KW_IF '(' exp ')' cmd %prec KW_IFX	{ $$ = astCreate(AST_IF, NULL, 2, $3, $5);} /* PAULO */
  | KW_IF '(' exp ')' cmd KW_ELSE cmd	{ $$ = astCreate(AST_IFTE, NULL, 3, $3, $5, $7); } /* PAULO */
  ;

while: KW_WHILE '(' exp ')' cmd			{ $$ = astCreate(AST_WHILE, NULL, 2, $3, $5); } /* PAULO */
     ;

block: '{' listOfCmd '}'				{ $$ = astCreate(AST_BLOCK, NULL, 1, $2); } /* MARCOS */
     ;

listOfCmd: cmd							{ $$ = astCreate(AST_LCMD, NULL, 2, $1, NULL); } /* MARCOS */
		 | listOfCmd cmd				{ $$ = astCreate(AST_LCMD, NULL, 2, $2, $1); } /* MARCOS */
	     ;

return: KW_RETURN exp					{ $$ = astCreate(AST_RETURN, NULL, 1, $2); } /* PAULO */

%%
void yyerror (char const *msg) {
	fprintf(stderr, "Syntax error in line %d.\n", getLineNumber());
	exit(3);
}
