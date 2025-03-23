#ifndef __QUATI_GLOBALS_H__
#define __QUATI_GLOBALS_H__


// #define DEBUG   1


typedef enum {
    TOK_VAR,     // variavel
    TOK_FUN,     // funcao
    TOK_FOR,     // para
    TOK_IF,      // se
    TOK_ELSE,    // senao
    TOK_PRINT,   // imprimir
    TOK_RETURN,  // retorne

    /* Identifier */
    TOK_ID,

    /* Types */
    TOK_INT,
    TOK_STRING,

    /* Symbols */
    TOK_ASSIGN,     // =
    TOK_SEMICOLON,  // ;
    TOK_COMMAN,     // ,
    TOK_LPAR,       // (
    TOK_RPAR,       // )
    TOK_LBRACE,     // {
    TOK_RBRACE,     // }

    /* Arithmetic Operators */
    TOK_INCREMENT,  // ++
    TOK_DECREMENT,  // --
    TOK_PLUS,       // +
    TOK_MINUS,      // -
    TOK_STARS,      // *
    TOK_SLASH,      // /

    /* Relational Operators */
    TOK_EQUAL,      // ==
    TOK_DIFFERENT,  // !=
    TOK_LT,         // <
    TOK_LTE,        // <=
    TOK_GT,         // >
    TOK_GTE,        // >=

    /* End Of File */
    TOK_EOF
} TokenType;


#define MAX_CHILDREN   4

typedef enum { StmtK, ExpK } NodeKind;
typedef enum { OpK, ConstK, IdK, StringK } ExpKind;
typedef enum { IfK, ForK, FuncK, AssignK, WriteK } StmtKind;

typedef enum { Void, Int, Bool } ExpType;

typedef struct TreeNode {
    struct TreeNode * childs[MAX_CHILDREN];
    struct TreeNode * next;

    NodeKind nodekind;
    union { StmtKind stmt; ExpKind exp; } kind;
    union {
        TokenType op;
        int val;
        char * name;
    } attrs;

    ExpType type;
} TreeNode;

#endif