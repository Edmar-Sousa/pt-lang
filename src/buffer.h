#ifndef __QUATI_BUFFER_H__
#define __QUATI_BUFFER_H__

#include "includes.h"
#include "globals.h"


#define MAX_BUFFER_SIZE         1024


extern FILE * source;

static char buffer[MAX_BUFFER_SIZE];
static int bufferSize = 0;
static int bufferPos = 0;


char getNextChar();
void backCaracter();

#endif