#ifndef __QUATI_UTILS_H__
#define __QUATI_UTILS_H__

#include <stdio.h>
#include "globals.h"
#include "includes.h"
#include "buffer.h"


void lexialError(unsigned int line, const char character);
void syntaxeError(unsigned int line, const char * world);

TreeNode * newStmtNode(StmtKind stmtKind);
TreeNode * newExpNode(ExpKind stmtKind);

#endif