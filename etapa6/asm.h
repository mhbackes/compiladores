/*
 * asm.h
 * ALUNOS:
 * MARCOS HENRIQUE BACKES
 * PAULO RENATO LANZARIN
 */

#ifndef ASM_
#define ASM_

#include <stdio.h>
#include "tac.h"
#include "hashtable.h"
#include "ast.h"

void asmWriteCode(FILE* file, TAC* tac);

void asmDeclareVariables(FILE* file);

#endif
