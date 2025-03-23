#ifndef __QUATI_UTILS_H__
#define __QUATI_UTILS_H__

#include <stdio.h>
#include "globals.h"
#include "buffer.h"


void lexialError(Buffer * buf, uint32_t line, const char character);
void syntaxeError(Buffer * buf, uint32_t line, const char * world);

TreeNode * newStmtNode(StmtKind stmtKind);
TreeNode * newExpNode(ExpKind stmtKind);

#endif