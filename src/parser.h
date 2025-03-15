#ifndef __CSL_PARSER_H__
#define __CSL_PARSER_H__

#include "utils.h"
#include "globals.h"

#define MAX_CHILDREN   3

typedef struct TreeNode {
    struct TreeNode * childs;
    struct TreeNode * next;
} TreeNode;


TreeNode * getProgramAST();

#endif