/*
 * parser.y
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include "lex.yy.h"
#include "hashtable.h"

int yylex();
void yyerror(const char *s);
%}

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

%token TK_IDENTIFIER
%token LIT_INTEGER  
%token LIT_FALSE    
%token LIT_TRUE	 
%token LIT_CHAR     
%token LIT_STRING   
%token TOKEN_ERROR  

/* ASSOCIATIVIDADE DOS OPERADORES BOOLEANOS LEFT TAMBEM??!!!! */
/* AND E OR tem menos prioridade que todos?? Acho, talvez botar por ultimo */
%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_NE OPERATOR_AND OPERATOR_OR
%left '=' '-'
%left '*' '/'

%%
program: declaration
	   | program declaration
	   ;

declaration: global ';'
		   | function ';'
		   ;

global: variable
	  | array
	  ;

variable: type TK_IDENTIFIER ':' literal
	    ;

type: KW_INT
	| KW_REAL
	| KW_CHAR
	| KW_BOOL
	;

literalBool: LIT_TRUE
		   | LIT_FALSE
		   ;

literal: literalBool
	   | LIT_INTEGER
	   | LIT_CHAR
	   ;


array: type TK_IDENTIFIER '[' LIT_INTEGER ']'
	 | type TK_IDENTIFIER '[' LIT_INTEGER ']' ':' listOfLiteral

listOfLiteral: literal
			 | listOfLiteral literal
			 ;

function: functionHeader block 
		;

functionHeader: type TK_IDENTIFIER '(' listOfParameters ')'
              ;

listOfParameters: parameter
                | listOfParameters ',' parameter
                :

parameter: type TK_IDENTIFIER
         |
         ;

exp: literal
   | '(' exp ')'
   | exp operator exp
   | TK_IDENTIFIER
   | TK_IDENTIFIER '[' exp ']'

operator: OPERATOR_LE  
        | OPERATOR_GE  
        | OPERATOR_EQ  
        | OPERATOR_NE  
        | OPERATOR_AND 
        | OPERATOR_OR 
        | '+'
        | '-'
        | '*'
        | '/'
        | '<'
        | '>'
        ;

cmd: attr
       | input
       | output
       | if
       | while
       | block
       ;

attr: TK_IDENTIFIER '=' exp
    | '[' exp ']' '=' exp
    ;


input:
     ;

output:
      ;

if: KW_IF '(' exp ')' cmd
  | KW_IF '(' exp ')' cmd KW_ELSE cmd
  ;

while: KW_WHILE '(' exp ')' cmd
     ;

block: '{' listOfCmd '}'
     ;

listOfCmd: cmd
		 | listOfCmd cmd
	     ;

%%
void yyerror (char const *msg) {
	fprintf(stderr, "Syntax error in line %d.\n", getLineNumber());
	exit(3);
}
