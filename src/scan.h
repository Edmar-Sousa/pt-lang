#ifndef __QUATI_LEXER_H__
#define __QUATI_LEXER_H__

#include "includes.h"
#include "globals.h"
#include "utils.h"
#include "buffer.h"


#define MAX_STRING_SIZE         1024
#define MAX_IDENTIFIER_SIZE       30
#define MAX_NUMBER_KEY_WORLD       7



typedef struct { 
    char world[MAX_IDENTIFIER_SIZE]; 
    TokenType type; 
} StructKeyWorld;


TokenType getNextToken();
int getLine();
char * getAmountIdentifier();

#endif