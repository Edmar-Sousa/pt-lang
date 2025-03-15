#ifndef __QUATI_BUFFER_H__
#define __QUATI_BUFFER_H__

#include "includes.h"
#include "globals.h"


#define MAX_BUFFER_SIZE         1024
#define MAX_LINE_LIMIT            30


extern FILE * source;


char getNextChar();
char * getBufferSlice();
void backCaracter();

void getProgram(int argc, char ** argv);

int getBufferPosition();

#endif