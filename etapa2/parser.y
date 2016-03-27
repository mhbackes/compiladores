%{
#include <stdio.h>
#include <stdlib.h>
#include "lex.yy.h"

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

%%
program: TK_IDENTIFIER

%%
void yyerror (char const *msg) {
	fprintf(stderr, "Syntax error in line %d.\n", getLineNumber());
	exit(3);
}
