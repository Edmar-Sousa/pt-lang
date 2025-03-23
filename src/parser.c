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

static TreeNode * expArithmetic(Scan *);
static TreeNode * stmtExp(Scan *);

static TreeNode * stmtPrint(Scan *);

static TreeNode * funcBody(Scan *);
static TreeNode * paramList(Scan *);

static TreeNode * loopBody(Scan *);
static TreeNode * loopIncrement(Scan *);
static TreeNode * stmtLoop(Scan *);

static TreeNode * stmtIf(Scan *);
static TreeNode * stmtFunc(Scan *);
static TreeNode * stmtAssign(Scan *);
static TreeNode * stmtVar(Scan *);

static void argumentsList(Scan *);
static TreeNode * stmt(Scan *);
static TreeNode * stmtSequence(Scan *);

TreeNode * getProgramAST(Scan * scan) 
{
    currentToken = getNextToken(scan);

    TreeNode * program = stmtSequence(scan);
    match(scan, TOK_EOF);

    return program;
};

static TreeNode * stmtSequence(Scan * scan)
{
    TreeNode * stmtNode = stmt(scan);
    TreeNode * auxNode = stmtNode;

    while (currentToken == TOK_PRINT || currentToken == TOK_IF || currentToken == TOK_FOR || currentToken == TOK_VAR || currentToken == TOK_ID || currentToken == TOK_FUN) 
    {
        TreeNode * nextStmt = stmt(scan);

        if (nextStmt) {
            auxNode->next = nextStmt;
            auxNode = nextStmt;
        }
    }

    return stmtNode;
}

static TreeNode * stmt(Scan * scan) 
{
    switch (currentToken) 
    {
        case TOK_PRINT: return stmtPrint(scan);
        case TOK_FOR: return stmtLoop(scan);
        case TOK_IF: return stmtIf(scan);
        case TOK_FUN: return stmtFunc(scan);
        case TOK_VAR: return stmtVar(scan);
        case TOK_ID: return stmtAssign(scan);

        default:
            return NULL;
    }

    return NULL;
}


static TreeNode * paramList(Scan * scan)
{
    TreeNode * paramsNode = NULL;
    TreeNode * auxNode = paramsNode;

    while (currentToken == TOK_ID)
    {
        TreeNode * param = newExpNode(IdK);
        param->attrs.name = getAmountIdentifier(scan);

        match(scan, TOK_ID);

        if (!paramsNode) paramsNode = auxNode = param;
        else {
            auxNode->next = param;
            auxNode = param;
        }

        if (currentToken != TOK_RPAR)
            match(scan, TOK_COMMAN);
    }

    return paramsNode;
}

static TreeNode * loopIncrement(Scan * scan)
{
    TreeNode * loopIncrement = newExpNode(OpK);

    TreeNode * left = newExpNode(IdK);
    left->attrs.name = getAmountIdentifier(scan);

    match(scan, TOK_ID);

    loopIncrement->childs[0] = left;

    if (currentToken == TOK_INCREMENT || currentToken == TOK_DECREMENT) {
        loopIncrement->attrs.op = currentToken;
        match(scan, currentToken);
    }

    else if (currentToken == TOK_ASSIGN)
    {
        match(scan, TOK_ASSIGN);
        loopIncrement->childs[0] = stmtExp(scan);
    }

    return loopIncrement;
}

static TreeNode * loopBodyStmt(Scan * scan)
{
    switch (currentToken) 
    {
        case TOK_PRINT: return stmtPrint(scan);
        case TOK_IF: return stmtIf(scan);
        case TOK_VAR: return stmtVar(scan);
        case TOK_ID: return stmtAssign(scan);

        default:
            return NULL;
    }

    return NULL;
}

static TreeNode * loopBody(Scan * scan)
{
    TreeNode * loopNode = NULL;
    TreeNode * auxNode = loopNode;

    while (currentToken == TOK_PRINT || currentToken == TOK_VAR || currentToken == TOK_ID || currentToken == TOK_IF) 
    {
        TreeNode * node = loopBodyStmt(scan);

        if (!node) loopNode = auxNode = node;
        else {
            auxNode->next = node;
            auxNode = node;
        }
    }

    return loopNode;
}

static TreeNode * stmtPrint(Scan * scan)
{
    TreeNode * stmtNode = newStmtNode(WriteK);

    match(scan, TOK_PRINT);
    match(scan, TOK_LPAR);

    if (currentToken == TOK_STRING) 
    {
        TreeNode * stringNode = newExpNode(StringK);
        stringNode->attrs.name = getStringValue(scan);

        match(scan, TOK_STRING);
        stmtNode->childs[0] = stringNode;
    }

    else
        stmtNode->childs[0] = stmtExp(scan);

    match(scan, TOK_RPAR);
    match(scan, TOK_SEMICOLON);

    return stmtNode;
}

static TreeNode * stmtLoop(Scan * scan)
{
    TreeNode * loopStmt = newStmtNode(ForK);

    match(scan, TOK_FOR);
    match(scan, TOK_LPAR);

    loopStmt->childs[0] = stmtVar(scan);
    loopStmt->childs[1] = stmtExp(scan);

    match(scan, TOK_SEMICOLON);
    loopStmt->childs[2] = loopIncrement(scan);
    
    match(scan, TOK_RPAR);
    match(scan, TOK_LBRACE);
    loopStmt->childs[3] = loopBody(scan);
    match(scan, TOK_RBRACE);

    return loopStmt;
}

static TreeNode * stmtIf(Scan * scan)
{
    TreeNode * ifNode = newStmtNode(IfK);

    match(scan, TOK_IF);
    match(scan, TOK_LPAR);

    ifNode->childs[0] = stmtExp(scan);
    
    match(scan, TOK_RPAR);
    match(scan, TOK_LBRACE);

    ifNode->childs[1] = stmtSequence(scan);

    match(scan, TOK_RBRACE);

    if (currentToken == TOK_ELSE) 
    {
        match(scan, TOK_ELSE);
        match(scan, TOK_LBRACE);

        ifNode->childs[2] = stmtSequence(scan);
        
        match(scan, TOK_RBRACE);
    }

    return ifNode;
}


static TreeNode * funcStmtBody(Scan * scan)
{
    switch (currentToken) 
    {
        case TOK_PRINT: return stmtPrint(scan);
        case TOK_IF: return stmtIf(scan);
        case TOK_VAR: return stmtVar(scan);
        case TOK_ID: return stmtAssign(scan);

        default: return NULL;
    }

    return NULL;
}

static TreeNode * funcBody(Scan * scan)
{
    TreeNode * bodyNode = NULL;
    TreeNode * auxNode = bodyNode;

    while (currentToken == TOK_PRINT || currentToken == TOK_VAR || currentToken == TOK_ID || currentToken == TOK_IF) 
    {
        TreeNode * bodyStmt = funcStmtBody(scan);

        if (!bodyNode) bodyNode = auxNode = bodyStmt;
        else {
            auxNode->next = bodyStmt;
            auxNode = bodyStmt;
        }
    }

    TreeNode * returnStmt = NULL;

    if (currentToken == TOK_RETURN) {
        match(scan, TOK_RETURN);

        if (currentToken == TOK_STRING) {
            returnStmt = newExpNode(StringK);
            returnStmt->attrs.name = getStringValue(scan);

            match(scan, TOK_STRING);
        }

        else
            returnStmt = stmtExp(scan);

        if (auxNode && returnStmt)
            auxNode->next = returnStmt;

        else if (returnStmt)
            bodyNode = returnStmt;


        match(scan, TOK_SEMICOLON);
    }

    return bodyNode;
}

static TreeNode * stmtFunc(Scan * scan)
{
    TreeNode * funcStmt = newStmtNode(FuncK);

    match(scan, TOK_FUN);

    funcStmt->attrs.name = getAmountIdentifier(scan);

    match(scan, TOK_ID);
    match(scan, TOK_LPAR);

    funcStmt->childs[0] = paramList(scan);
    
    match(scan, TOK_RPAR);
    match(scan, TOK_LBRACE);

    funcStmt->childs[1] = funcBody(scan);
    match(scan, TOK_RBRACE);

    return funcStmt;
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

static TreeNode * stmtAssign(Scan * scan) 
{
    TreeNode * assignNode = newStmtNode(AssignK);
    match(scan, TOK_ID);

    if (currentToken == TOK_ASSIGN) {
        match(scan, TOK_ASSIGN);
        assignNode->childs[0] = stmtExp(scan);
    }
    
    else if (currentToken == TOK_LPAR) {
        match(scan, TOK_LPAR);
        argumentsList(scan);
        match(scan, TOK_RPAR);
    }
    
    match(scan, TOK_SEMICOLON);
    return assignNode;
}

static TreeNode * stmtVar(Scan * scan) 
{
    TreeNode * stmtVarNode = newStmtNode(AssignK);

    match(scan, TOK_VAR);
    
    stmtVarNode->attrs.name = getAmountIdentifier(scan);
    match(scan, TOK_ID);
    match(scan, TOK_ASSIGN);
    
    stmtVarNode->childs[0] = stmtExp(scan);
    
    match(scan, TOK_SEMICOLON);

    return stmtVarNode;
}

static int isRelationalOperator()
{
    return (currentToken >= TOK_EQUAL) && (currentToken <= TOK_GTE);
}

static TreeNode * stmtExp(Scan * scan) 
{
    TreeNode * leftTree = expArithmetic(scan);

    if (isRelationalOperator()) {
        TreeNode * rightTree = newExpNode(OpK);

        rightTree->childs[0] = leftTree;
        rightTree->attrs.op = currentToken;

        leftTree = rightTree;
        match(scan, currentToken);

        leftTree->childs[1] = expArithmetic(scan);
    }

    return leftTree;
}

static TreeNode * expArithmetic(Scan * scan)
{
    TreeNode * leftTree = term(scan);

    while (currentToken == TOK_PLUS || currentToken == TOK_MINUS) 
    {
        TreeNode * rightTree = newExpNode(OpK);

        if (rightTree) {
            rightTree->childs[0] = leftTree;
            rightTree->attrs.op = currentToken;
    
            leftTree = rightTree;
            match(scan, currentToken);
    
            leftTree->childs[1] = term(scan);
        }
    }

    return leftTree;
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
