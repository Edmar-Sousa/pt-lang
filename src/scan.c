#include "scan.h"
#include "buffer.h"
#include "globals.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


StructKeyWorld keyWorlds[MAX_NUMBER_KEY_WORLD] = {
    { .world = "variavel", .type = TOK_VAR },
    { .world = "funcao", .type = TOK_FUN },
    { .world = "para", .type = TOK_FOR },
    { .world = "se", .type = TOK_IF },
    { .world = "senao", .type = TOK_ELSE },
    { .world = "imprimir", .type = TOK_PRINT },
    { .world = "retorne", .type = TOK_RETURN }
};

char * getAmountIdentifier(Scan * scan)
{
    char * identifier = (char *) malloc(sizeof(char) * MAX_IDENTIFIER_SIZE);

    if (!identifier)
        return NULL;

    strncpy(identifier, scan->identifier, MAX_IDENTIFIER_SIZE);
    return identifier;
}

int32_t getIntvalue(Scan * scan)
{
    return scan->intval;
}

uint32_t getLine(Scan * scan)
{
    return scan->linepos;
}

static void resetScanIdentifier(Scan * scan)
{
    memset(scan->string, '\0', MAX_STRING_SIZE);
    memset(scan->identifier, '\0', MAX_IDENTIFIER_SIZE);
    
    scan->intval = 0;
    scan->index = 0;
}

void initScan(Scan * scan, Buffer * buf)
{
    resetScanIdentifier(scan);
    scan->linepos = 0;
    scan->buf = buf;
}

static int isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static int isNumeric(char c) 
{
    return c >= '0' && c <= '9';
}

static int isAlphaNumeric(char c)
{
    return isAlpha(c) || isNumeric(c);
}


static TokenType getKeyWorldOrIdentifier(Scan * scan)
{
    resetScanIdentifier(scan);
    backCaracter(scan->buf);

    char c = getNextChar(scan->buf);

    while (isAlphaNumeric(c) && scan->index < MAX_IDENTIFIER_SIZE - 1) {
        scan->identifier[scan->index++] = c;
        c = getNextChar(scan->buf);
    }
    
    backCaracter(scan->buf);
    scan->identifier[scan->index++] = '\0';

    for (int i = 0; i < MAX_NUMBER_KEY_WORLD; i++) {
        StructKeyWorld keyWorld = keyWorlds[i];

        if (strncmp(scan->identifier, keyWorld.world, MAX_IDENTIFIER_SIZE) == 0) {
            #ifdef DEBUG
            printf("keyworld: <%s, %d>\n", keyWorld.world, keyWorld.type);
            #endif

            return keyWorld.type;
        }
    }

    #ifdef DEBUG
    printf("<%s, %d>\n", scan->identifier, TOK_ID);
    #endif

    return TOK_ID;
}


static TokenType getNumber(Scan * scan)
{
    resetScanIdentifier(scan);
    backCaracter(scan->buf);

    char c = getNextChar(scan->buf);
    scan->index = 0;
    
    while (isNumeric(c)) {
        scan->identifier[scan->index++] = c;
        scan->intval = scan->intval * 10 + (c - '0');

        c = getNextChar(scan->buf);
    }

    backCaracter(scan->buf);

    #ifdef DEBUG
    printf("<%d, %d>\n", scan->intval, TOK_INT);
    #endif

    return TOK_INT;
}

TokenType getString(Scan * scan)
{
    resetScanIdentifier(scan);
    scan->string[scan->index++] = '"';

    char c = getNextChar(scan->buf);
    while (c != '"') 
    {
        scan->string[scan->index++] = c;
        c = getNextChar(scan->buf);
    }

    scan->string[scan->index++] = '"';
    scan->string[scan->index++] = '\0';

    #ifdef DEBUG
    printf("<%s, %d>\n", scan->string, TOK_STRING);
    #endif

    return TOK_STRING;
}

TokenType getNextToken(Scan * scan) 
{
    char c;

    while ((c = getNextChar(scan->buf)) != TOK_EOF) {
        switch (c) 
        {
            case '\n':
                scan->linepos += 1;
                break;
            
            case ' ':
            case '\t':
                continue;

            case '"': {
                return getString(scan);
            }

            case '=': {
                char next = getNextChar(scan->buf);

                if (next == '=') {
                    #ifdef DEBUG
                    printf("<'==', %d>\n", TOK_ASSIGN);
                    #endif

                    return TOK_EQUAL;
                }

                backCaracter(scan->buf);

                #ifdef DEBUG
                printf("<'=', %d>\n", TOK_ASSIGN);
                #endif

                return TOK_ASSIGN;
            }
            case ';': {
                #ifdef DEBUG
                printf("<';', %d>\n", TOK_SEMICOLON);
                #endif

                return TOK_SEMICOLON;
            }
            case '(': {
                #ifdef DEBUG
                printf("<'(', %d>\n", TOK_LPAR);
                #endif

                return TOK_LPAR;
            }
            case ')': {
                #ifdef DEBUG
                printf("<')', %d>\n", TOK_RPAR);
                #endif

                return TOK_RPAR;
            }
            case '<': {
                char next = getNextChar(scan->buf);

                if (next == '=') {
                    #ifdef DEBUG
                    printf("<'<=', %d>\n", TOK_LTE);
                    #endif

                    return TOK_LTE;
                }

                #ifdef DEBUG
                printf("<'<', %d>\n", TOK_LT);
                #endif

                return TOK_LT;
            }
            case '>': {
                char next = getNextChar(scan->buf);

                if (next == '=') {
                    #ifdef DEBUG
                    printf("<'>=', %d>\n", TOK_GTE);
                    #endif

                    return TOK_GTE;
                }

                #ifdef DEBUG
                printf("<'>', %d>\n", TOK_GT);
                #endif

                return TOK_GT;
            }
            case '{': {
                #ifdef DEBUG
                printf("<'{', %d>\n", TOK_LBRACE);
                #endif

                return TOK_LBRACE;
            }
            case '}': {
                #ifdef DEBUG
                printf("<'}', %d>\n", TOK_RBRACE);
                #endif

                return TOK_RBRACE;
            }
            case ',': {
                #ifdef DEBUG
                printf("<',', %d>\n", TOK_COMMAN);
                #endif

                return TOK_COMMAN;
            }
            case '*': {
                #ifdef DEBUG
                printf("<'*', %d>\n", TOK_STARS);
                #endif

                return TOK_STARS;
            }
            case '/': {
                #ifdef DEBUG
                printf("<'/', %d>\n", TOK_SLASH);
                #endif

                return TOK_SLASH;
            }
            case '+': {
                char next = getNextChar(scan->buf);

                if (next == '+') {
                    #ifdef DEBUG
                    printf("<'++', %d>\n", TOK_INCREMENT);
                    #endif

                    return TOK_INCREMENT;
                }
                
                backCaracter(scan->buf);

                #ifdef DEBUG
                printf("<'+', %d>\n", TOK_PLUS);
                #endif

                return TOK_PLUS;
            }

            case '-': {
                char next = getNextChar(scan->buf);

                if (next == '-') {
                    #ifdef DEBUG
                    printf("<'--', %d>\n", TOK_DECREMENT);
                    #endif

                    return TOK_DECREMENT;
                }

                backCaracter(scan->buf);

                #ifdef DEBUG
                printf("<'+', %d>\n", TOK_MINUS);
                #endif

                return TOK_MINUS;
            }

            default:
                if (isAlpha(c)) return getKeyWorldOrIdentifier(scan);
                if (isNumeric(c)) return getNumber(scan);

                lexialError(scan->buf, scan->linepos + 1, c);
                break;
        }
    }

    return TOK_EOF;
}
