#ifndef __QUATI_BUFFER_H__
#define __QUATI_BUFFER_H__

#include "includes.h"
#include "globals.h"


#define MAX_BUFFER_SIZE         1024
#define MAX_LINE_LIMIT            30


extern FILE * source;


typedef struct {
    FILE * source;

    char buffer[MAX_BUFFER_SIZE];
    uint32_t bufferSize;
    uint32_t bufferPos;
} Buffer;


void initBuffer(Buffer *);
void openProgramFile(Buffer *, int, char **) ;

char getNextChar(Buffer *);

uint32_t getBufferPosition(Buffer *);

char * getBufferSlice(Buffer *);

void backCaracter(Buffer *); 

#endif