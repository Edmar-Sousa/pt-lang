#ifndef __QUATI_BUFFER_H__
#define __QUATI_BUFFER_H__

#include "globals.h"


#define MAX_LINE_SIZE           1024
#define MAX_LINE_LIMIT            30


typedef struct {
    FILE * source;

    char buffer[MAX_LINE_SIZE];
    int32_t bufferSize;
    int32_t bufferPos;
} Buffer;


void initBuffer(Buffer *);
void openProgramFile(Buffer *, int, char **) ;

char getNextChar(Buffer *);

uint32_t getBufferPosition(Buffer *);

char * getBufferSlice(Buffer *);

void backCaracter(Buffer *); 

#endif