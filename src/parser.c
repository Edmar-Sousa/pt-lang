#include "parser.h"
#include "globals.h"
#include "scan.h"
#include "utils.h"
#include <stdio.h>

static TokenType currentToken;

void showSyntaxeError()
{
    syntaxeError(getLine() + 1, getAmountIdentifier());
}


static void match(TokenType tok)
{
    if (currentToken != tok)
        showSyntaxeError();

    #ifdef DEBUG
    printf("MATCH: %d\n", tok);
    #endif

    currentToken = getNextToken();
}

static void number();
static void factor();
static void term();

static void expArithmetic();
static void stmtExp();

static void stmtPrint();

static void funcBody();
static void paramList();

static void loopBody();
static void loopIncrement();
static void stmtLoop();

static void stmtIf();
static void stmtFunc();
static void stmtAssign();
static void stmtVar();

static void stmt();
static void stmtSequence();

TreeNode * getProgramAST() 
{
    currentToken = getNextToken();

    stmtSequence();
    match(TOK_EOF);

    return NULL;
};

static void stmtSequence()
{
    while (currentToken == TOK_PRINT || currentToken == TOK_IF || currentToken == TOK_FOR || currentToken == TOK_VAR || currentToken == TOK_ID || currentToken == TOK_FUN) 
    {
        switch (currentToken) 
        {
            case TOK_PRINT:
                stmtPrint();
                break;

            case TOK_FOR:
                stmtLoop();
                break;

            case TOK_IF:
                stmtIf();
                break;

            case TOK_FUN:
                stmtFunc();
                break;

            case TOK_VAR:
                stmtVar();
                break;
    
            case TOK_ID:
                stmtAssign();
                break;
    
            default:
                showSyntaxeError();
        }
    }
}

static void paramList()
{
    match(TOK_ID);
}

static void loopIncrement()
{
    match(TOK_ID);

    if (currentToken == TOK_INCREMENT || currentToken == TOK_DECREMENT)
        match(currentToken);

    else if (currentToken == TOK_ASSIGN)
    {
        match(TOK_ASSIGN);
        stmtExp();
    }
}

static void loopBody()
{
    while (currentToken == TOK_PRINT || currentToken == TOK_VAR || currentToken == TOK_ID || currentToken == TOK_IF) 
    {
        switch (currentToken) 
        {
            case TOK_PRINT:
                stmtPrint();
                break;

            case TOK_IF:
                stmtIf();
                break;
            
            case TOK_VAR:
                stmtVar();
                break;
    
            case TOK_ID:
                stmtAssign();
                break;
    
            default:
                showSyntaxeError();
        }
    }
}

static void stmtPrint()
{
    match(TOK_PRINT);
    match(TOK_LPAR);

    if (currentToken == TOK_STRING)
        match(TOK_STRING);

    else
        stmtExp();

    match(TOK_RPAR);
    match(TOK_SEMICOLON);
}

static void stmtLoop()
{
    match(TOK_FOR);
    match(TOK_LPAR);
    stmtVar();
    stmtExp();
    match(TOK_SEMICOLON);
    loopIncrement();
    match(TOK_RPAR);

    match(TOK_LBRACE);
    loopBody();
    match(TOK_RBRACE);
}

static void stmtIf()
{
    match(TOK_IF);
    match(TOK_LPAR);
    stmtExp();
    match(TOK_RPAR);
    match(TOK_LBRACE);
    stmtSequence();
    match(TOK_RBRACE);

    if (currentToken == TOK_ELSE) 
    {
        match(TOK_ELSE);
        match(TOK_LBRACE);
        stmtSequence();
        match(TOK_RBRACE);
    }
}

static void funcBody()
{
    while (currentToken == TOK_PRINT || currentToken == TOK_VAR || currentToken == TOK_ID || currentToken == TOK_IF) 
    {
        switch (currentToken) 
        {
            case TOK_PRINT:
                stmtPrint();
                break;

            case TOK_IF:
                stmtIf();
                break;
            
            case TOK_VAR:
                stmtVar();
                break;
    
            case TOK_ID:
                stmtAssign();
                break;
    
            default:
                showSyntaxeError();
        }
    }

    if (currentToken == TOK_RETURN) {
        match(TOK_RETURN);

        if (currentToken == TOK_STRING)
            match(TOK_STRING);
        else
            stmtExp();

        match(TOK_SEMICOLON);
    }
}

static void stmtFunc()
{
    match(TOK_FUN);
    match(TOK_ID);
    match(TOK_LPAR);
    paramList();
    match(TOK_RPAR);
    match(TOK_LBRACE);
    funcBody();
    match(TOK_RBRACE);
}

static void stmtAssign() 
{
    match(TOK_ID);
    match(TOK_ASSIGN);
    stmtExp();
    match(TOK_SEMICOLON);
}

static void stmtVar() 
{
    match(TOK_VAR);
    match(TOK_ID);
    match(TOK_ASSIGN);
    stmtExp();
    match(TOK_SEMICOLON);
}

static int isRelationalOperator()
{
    return (currentToken >= TOK_EQUAL) && (currentToken <= TOK_GTE);
}

static void stmtExp() 
{
    expArithmetic();

    if (isRelationalOperator()) {
        match(currentToken);
        expArithmetic();
    }
}

static void expArithmetic()
{
    term();

    while (currentToken == TOK_PLUS || currentToken == TOK_MINUS) 
    {
        if (currentToken == TOK_PLUS)
            match(TOK_PLUS);

        else if (currentToken == TOK_MINUS)
            match(TOK_MINUS);

        term();
    }

}

static void term()
{
    factor();

    while (currentToken == TOK_SLASH || currentToken == TOK_STARS) 
    {
        if (currentToken == TOK_SLASH)
            match(TOK_SLASH);

        else if (currentToken == TOK_STARS)
            match(TOK_STARS);

        factor();
    }
    
}

static void factor() 
{
    if (currentToken == TOK_INT)
        match(TOK_INT);

    else if (currentToken == TOK_ID)
        match(TOK_ID);

    else if (currentToken == TOK_LPAR) 
    {
        match(TOK_LPAR);
        stmtExp();
        match(TOK_RPAR);
    }

    else
        showSyntaxeError();
}
