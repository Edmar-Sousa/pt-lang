#include "utils.h"
#include "buffer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



void lexialError(Buffer * buf, uint32_t line, const char character)
{
    fprintf(stderr, "\n\033[31mErro: Não esperava o caracter \"%c\".\033[m\n\n", character);
    fprintf(stderr, "\t%d | %s\n", line, getBufferSlice(buf));
    fprintf(stderr, "\t%*s^-- Aqui.\n\n", getBufferPosition(buf) + 3, "");

    exit(EXIT_FAILURE);
}


void syntaxeError(Buffer * buf, uint32_t line, const char * world)
{
    fprintf(stderr, "\n\033[31mSintaxe Inválida: Não esperava por \"%s\".\033[m\n\n", world);
    fprintf(stderr, "\t%d | %s\n", line, getBufferSlice(buf));
    fprintf(stderr, "\t%*s\n\n", getBufferPosition(buf), "^");

    exit(EXIT_FAILURE);
}

void errorMemory() 
{
    fprintf(stderr, "\n\033[31mErro ao alocar memoria.\033[m\n\n");
    exit(EXIT_FAILURE);
}

TreeNode * newStmtNode(StmtKind stmtKind)
{
    TreeNode * newNode = (TreeNode *) malloc(sizeof(TreeNode));

    if (!newNode) errorMemory();

    for (int count = 0; count < MAX_CHILDREN; count++)
        newNode->childs[count] = NULL;

    newNode->next = NULL;
    newNode->nodekind = StmtK;
    newNode->kind.stmt = stmtKind;

    return newNode;
}

TreeNode * newExpNode(ExpKind expKind)
{
    TreeNode * newNode = (TreeNode *) malloc(sizeof(TreeNode));

    if (!newNode) errorMemory();

    for (int count = 0; count < MAX_CHILDREN; count++)
        newNode->childs[count] = NULL;

    newNode->next = NULL;
    newNode->nodekind = ExpK;
    newNode->kind.exp = expKind;

    return newNode;
}
