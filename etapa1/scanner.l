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

%%

#include "main.c"

int yywrap() {
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
