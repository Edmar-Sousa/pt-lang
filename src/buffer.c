#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * source = NULL;

static char buffer[MAX_BUFFER_SIZE];
static int bufferSize = 0;
static int bufferPos = 0;


static unsigned short findBreakLineOnBuffer(unsigned short start, char dir)
{
    if (start + dir < 0 || start + dir > bufferSize)
        return start;

    unsigned short position = start;

    while (
        position + dir >= 0 && position >= start - MAX_LINE_LIMIT
        && position + dir < bufferSize && position <= start + MAX_LINE_LIMIT
        && buffer[position] != '\n'
    ) {
        position = position + dir;
    }

    return position;
}


char * getBufferSlice()
{
    unsigned short startLine = findBreakLineOnBuffer(bufferPos, -1);
    unsigned short endLine = findBreakLineOnBuffer(bufferPos, 1);
    unsigned short length = endLine - startLine;


    char * slice = (char *) malloc(length + 1);

    if (!slice)
        return NULL;

    memcpy(slice, buffer + startLine, length);
    slice[length] = '\0';

    return slice;
}

char getNextChar() {
    if (bufferPos < bufferSize)
        return buffer[bufferPos++];

    if (fgets(buffer, MAX_BUFFER_SIZE - 1, source)) {
        bufferPos = 0;
        bufferSize = strlen(buffer);
        return buffer[bufferPos++];
    }

    return TOK_EOF;
}


void backCaracter()
{
    bufferPos = bufferPos > 0 ? bufferPos - 1 : 0;
}


void getProgram(int argc, char ** argv) 
{
    if (argc < 2) {
        printf("[Quati] Execute \"quati <program.quati>\"\n");
        exit(EXIT_FAILURE);
    }

    char program[MAX_PROGRAM_NAME];
    strcpy(program, argv[1]);

    if (strchr(program, '.') == NULL) 
        strcat(program, ".quati");

    source = fopen(program, "r");
}
