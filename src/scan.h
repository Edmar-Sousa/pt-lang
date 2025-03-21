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


typedef struct {
    uint32_t index;

    char string[MAX_STRING_SIZE];
    char identifier[MAX_IDENTIFIER_SIZE];
    int32_t intval;

    uint32_t linepos;

    Buffer * buf;
} Scan;


void initScan(Scan *, Buffer *);

TokenType getNextToken(Scan *) ;

char * getAmountIdentifier(Scan *);
char * getStringValue(Scan *);

int32_t getIntvalue(Scan *);
uint32_t getLine(Scan *);

#endif