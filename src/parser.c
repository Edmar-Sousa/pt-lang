#include "parser.h"
#include "globals.h"
#include "scan.h"
#include "utils.h"
#include <stdio.h>

static TokenType currentToken;

void showSyntaxeError(Scan * scan)
{
    syntaxeError(
        scan->buf,
        getLine(scan) + 1, 
        getAmountIdentifier(scan)
    );
}


static void match(Scan * scan, TokenType tok)
{
    if (currentToken != tok)
        showSyntaxeError(scan);

    #ifdef DEBUG
    printf("MATCH: %d\n", tok);
    #endif

    currentToken = getNextToken(scan);
}

static TreeNode * factor(Scan *);
static TreeNode * term(Scan *);

static void expArithmetic(Scan *);
static void stmtExp(Scan *);

static void stmtPrint(Scan *);

static void funcBody(Scan *);
static void paramList(Scan *);

static void loopBody(Scan *);
static void loopIncrement(Scan *);
static void stmtLoop(Scan *);

static void stmtIf(Scan *);
static void stmtFunc(Scan *);
static void stmtAssign(Scan *);
static void stmtVar(Scan *);

static void argumentsList(Scan *);
static void stmtSequence(Scan *);

TreeNode * getProgramAST(Scan * scan) 
{
    currentToken = getNextToken(scan);

    stmtSequence(scan);
    match(scan, TOK_EOF);

    return NULL;
};

static void stmtSequence(Scan * scan)
{
    while (currentToken == TOK_PRINT || currentToken == TOK_IF || currentToken == TOK_FOR || currentToken == TOK_VAR || currentToken == TOK_ID || currentToken == TOK_FUN) 
    {
        switch (currentToken) 
        {
            case TOK_PRINT:
                stmtPrint(scan);
                break;

            case TOK_FOR:
                stmtLoop(scan);
                break;

            case TOK_IF:
                stmtIf(scan);
                break;

            case TOK_FUN:
                stmtFunc(scan);
                break;

            case TOK_VAR:
                stmtVar(scan);
                break;
    
            case TOK_ID:
                stmtAssign(scan);
                break;
    
            default:
                showSyntaxeError(scan);
        }
    }
}

static void paramList(Scan * scan)
{
    while (currentToken == TOK_ID)
    {
        match(scan, TOK_ID);

        if (currentToken != TOK_RPAR)
            match(scan, TOK_COMMAN);
    }
}

static void loopIncrement(Scan * scan)
{
    match(scan, TOK_ID);

    if (currentToken == TOK_INCREMENT || currentToken == TOK_DECREMENT)
        match(scan, currentToken);

    else if (currentToken == TOK_ASSIGN)
    {
        match(scan, TOK_ASSIGN);
        stmtExp(scan);
    }
}

static void loopBody(Scan * scan)
{
    while (currentToken == TOK_PRINT || currentToken == TOK_VAR || currentToken == TOK_ID || currentToken == TOK_IF) 
    {
        switch (currentToken) 
        {
            case TOK_PRINT:
                stmtPrint(scan);
                break;

            case TOK_IF:
                stmtIf(scan);
                break;
            
            case TOK_VAR:
                stmtVar(scan);
                break;
    
            case TOK_ID:
                stmtAssign(scan);
                break;
    
            default:
                showSyntaxeError(scan);
        }
    }
}

static void stmtPrint(Scan * scan)
{
    match(scan, TOK_PRINT);
    match(scan, TOK_LPAR);

    if (currentToken == TOK_STRING)
        match(scan, TOK_STRING);

    else
        stmtExp(scan);

    match(scan, TOK_RPAR);
    match(scan, TOK_SEMICOLON);
}

static void stmtLoop(Scan * scan)
{
    match(scan, TOK_FOR);
    match(scan, TOK_LPAR);
    stmtVar(scan);
    stmtExp(scan);
    match(scan, TOK_SEMICOLON);
    loopIncrement(scan);
    match(scan, TOK_RPAR);

    match(scan, TOK_LBRACE);
    loopBody(scan);
    match(scan, TOK_RBRACE);
}

static void stmtIf(Scan * scan)
{
    match(scan, TOK_IF);
    match(scan, TOK_LPAR);
    stmtExp(scan);
    match(scan, TOK_RPAR);
    match(scan, TOK_LBRACE);
    stmtSequence(scan);
    match(scan, TOK_RBRACE);

    if (currentToken == TOK_ELSE) 
    {
        match(scan, TOK_ELSE);
        match(scan, TOK_LBRACE);
        stmtSequence(scan);
        match(scan, TOK_RBRACE);
    }
}

static void funcBody(Scan * scan)
{
    while (currentToken == TOK_PRINT || currentToken == TOK_VAR || currentToken == TOK_ID || currentToken == TOK_IF) 
    {
        switch (currentToken) 
        {
            case TOK_PRINT:
                stmtPrint(scan);
                break;

            case TOK_IF:
                stmtIf(scan);
                break;
            
            case TOK_VAR:
                stmtVar(scan);
                break;
    
            case TOK_ID:
                stmtAssign(scan);
                break;
    
            default:
                showSyntaxeError(scan);
        }
    }

    if (currentToken == TOK_RETURN) {
        match(scan, TOK_RETURN);

        if (currentToken == TOK_STRING)
            match(scan, TOK_STRING);
        else
            stmtExp(scan);

        match(scan, TOK_SEMICOLON);
    }
}

static void stmtFunc(Scan * scan)
{
    match(scan, TOK_FUN);
    match(scan, TOK_ID);
    match(scan, TOK_LPAR);
    paramList(scan);
    match(scan, TOK_RPAR);
    match(scan, TOK_LBRACE);
    funcBody(scan);
    match(scan, TOK_RBRACE);
}

static void argumentsList(Scan * scan)
{
    while (currentToken == TOK_ID || currentToken == TOK_INT || currentToken == TOK_STRING)
    {
        match(scan, currentToken);

        if (currentToken != TOK_RPAR)
            match(scan, TOK_COMMAN);
    }
}

static void stmtAssign(Scan * scan) 
{
    match(scan, TOK_ID);

    if (currentToken == TOK_ASSIGN) {
        match(scan, TOK_ASSIGN);
        stmtExp(scan);
    }
    
    else if (currentToken == TOK_LPAR) {
        match(scan, TOK_LPAR);
        argumentsList(scan);
        match(scan, TOK_RPAR);
    }
    
    match(scan, TOK_SEMICOLON);
}

static void stmtVar(Scan * scan) 
{
    match(scan, TOK_VAR);
    match(scan, TOK_ID);
    match(scan, TOK_ASSIGN);
    stmtExp(scan);
    match(scan, TOK_SEMICOLON);
}

static int isRelationalOperator()
{
    return (currentToken >= TOK_EQUAL) && (currentToken <= TOK_GTE);
}

static void stmtExp(Scan * scan) 
{
    expArithmetic(scan);

    if (isRelationalOperator()) {
        match(scan, currentToken);
        expArithmetic(scan);
    }
}

static void expArithmetic(Scan * scan)
{
    TreeNode * leftTree = term(scan);

    while (currentToken == TOK_PLUS || currentToken == TOK_MINUS) 
    {
        if (currentToken == TOK_PLUS)
            match(scan, TOK_PLUS);

        else if (currentToken == TOK_MINUS)
            match(scan, TOK_MINUS);

        term(scan);
    }

}

static TreeNode * term(Scan * scan)
{
    TreeNode * leftTree = factor(scan);

    while (currentToken == TOK_SLASH || currentToken == TOK_STARS) 
    {
        TreeNode * rightTree = newExpNode(OpK);

        if (rightTree) {
            rightTree->childs[0] = leftTree;
            rightTree->attrs.op = currentToken;
    
            leftTree = rightTree;
    
            match(scan, currentToken);
    
            leftTree->childs[1] = factor(scan);
        }
    }

    return leftTree;    
}

static TreeNode * factor(Scan * scan) 
{
    TreeNode * leaft = NULL;

    if (currentToken == TOK_INT) {
        leaft = newExpNode(ConstK);

        if (leaft)
            leaft->attrs.val = getIntvalue(scan);

        match(scan, TOK_INT);
    }

    else if (currentToken == TOK_ID) {
        leaft = newExpNode(IdK);

        if (leaft)
            leaft->attrs.name = getAmountIdentifier(scan);

        match(scan, TOK_ID);
    }

    else if (currentToken == TOK_LPAR) 
    {
        match(scan, TOK_LPAR);
        stmtExp(scan);
        match(scan, TOK_RPAR);
    }

    else
        showSyntaxeError(scan);

    return leaft;
}
