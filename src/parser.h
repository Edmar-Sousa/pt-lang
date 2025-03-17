#ifndef __QUATI_PARSER_H__
#define __QUATI_PARSER_H__

#include "utils.h"
#include "globals.h"

#define MAX_CHILDREN   3

typedef enum { StmtK, ExpK } NodeKind;
typedef enum { OpK, ConstK, IdK } ExpKind;
typedef enum { IfK, ForK, FuncK, AssignK, WriteK } StmtKind;

typedef enum { Void, Int, Bool } ExpType;

typedef struct TreeNode {
    struct TreeNode * childs;
    struct TreeNode * next;

    NodeKind nodekind;
    union { StmtKind stmt; ExpType exp; } kind;
    union {
        TokenType op;
        int val;
        char * name;
    } attrs;

    ExpType type;
} TreeNode;


TreeNode * getProgramAST();

#endif