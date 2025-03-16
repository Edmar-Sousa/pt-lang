#include "scan.h"
#include "globals.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

int linePos = 0;
static char identifier[MAX_IDENTIFIER_SIZE];

StructKeyWorld keyWorlds[MAX_NUMBER_KEY_WORLD] = {
    { .world = "variavel", .type = TOK_VAR },
    { .world = "funcao", .type = TOK_FUN },
    { .world = "para", .type = TOK_FOR },
    { .world = "se", .type = TOK_IF },
    { .world = "senao", .type = TOK_ELSE },
    { .world = "imprimir", .type = TOK_PRINT },
    { .world = "retorne", .type = TOK_RETURN }
};


char * getAmountIdentifier() 
{
    return identifier;
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

static TokenType getKeyWorldOrIdentifier()
{
    memset(identifier, '\0', MAX_IDENTIFIER_SIZE);
    
    backCaracter();
    unsigned char identifierPos = 0;

    char c = getNextChar();
    while (isAlphaNumeric(c) && identifierPos < MAX_IDENTIFIER_SIZE - 1) {
        identifier[identifierPos++] = c;
        c = getNextChar();
    }
    
    backCaracter();
    identifier[identifierPos] = '\0';

    for (int i = 0; i < MAX_NUMBER_KEY_WORLD; i++) {
        StructKeyWorld keyWorld = keyWorlds[i];

        if (strncmp(identifier, keyWorld.world, MAX_IDENTIFIER_SIZE) == 0) {
            #ifdef DEBUG
            printf("keyworld: <%s, %d>\n", keyWorld.world, keyWorld.type);
            #endif

            return keyWorld.type;
        }
    }

    #ifdef DEBUG
    printf("<%s, %d>\n", identifier, TOK_ID);
    #endif

    return TOK_ID;
}


static TokenType getNumber()
{
    backCaracter();
    int number = 0;

    char c = getNextChar();
    while (isNumeric(c)) {
        number = number * 10 + (c - '0');
        c = getNextChar();
    }

    backCaracter();

    #ifdef DEBUG
    printf("<%d, %d>\n", number, TOK_INT);
    #endif

    return TOK_INT;
}

TokenType getString()
{
    char string[MAX_STRING_SIZE];
    unsigned short position = 0;
    
    string[position++] = '"';

    char c = getNextChar();
    while (c != '"') 
    {
        string[position++] = c;
        c = getNextChar();
    }

    string[position++] = '"';
    string[position++] = '\0';

    #ifdef DEBUG
    printf("<%s, %d>\n", string, TOK_STRING);
    #endif

    return TOK_STRING;
}

TokenType getNextToken() 
{
    char c;

    while ((c = getNextChar()) != TOK_EOF) {
        switch (c) 
        {
            case '\n':
                linePos += 1;
                break;
            
            case ' ':
            case '\t':
                continue;

            case '"': {
                return getString();
            }

            case '=': {
                char next = getNextChar();

                if (next == '=') {
                    #ifdef DEBUG
                    printf("<'==', %d>\n", TOK_ASSIGN);
                    #endif

                    return TOK_EQUAL;
                }

                backCaracter();

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
                char next = getNextChar();

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
                char next = getNextChar();

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
                char next = getNextChar();

                printf("C: %c -> NEXT: %c\n", c, next);

                if (next == '+') {
                    #ifdef DEBUG
                    printf("<'++', %d>\n", TOK_INCREMENT);
                    #endif

                    return TOK_INCREMENT;
                }
                
                backCaracter();

                #ifdef DEBUG
                printf("<'+', %d>\n", TOK_PLUS);
                #endif

                return TOK_PLUS;
            }

            case '-': {
                char next = getNextChar();

                if (next == '-') {
                    #ifdef DEBUG
                    printf("<'--', %d>\n", TOK_DECREMENT);
                    #endif

                    return TOK_DECREMENT;
                }

                backCaracter();

                #ifdef DEBUG
                printf("<'+', %d>\n", TOK_MINUS);
                #endif

                return TOK_MINUS;
            }

            default:
                if (isAlpha(c)) return getKeyWorldOrIdentifier();
                if (isNumeric(c)) return getNumber();

                lexialError(linePos + 1, c);
                break;
        }
    }

    return TOK_EOF;
}
