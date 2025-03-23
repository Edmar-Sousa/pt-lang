#include "buffer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void initBuffer(Buffer * buf)
{
    buf->source = NULL;
    buf->bufferPos = 0;
    buf->bufferSize = 0;
}


void openProgramFile(Buffer * buf, int argc, char ** argv) 
{
    if (argc < 2) {
        printf("[Quati] Execute \"quati <program.quati>\"\n");
        exit(EXIT_FAILURE);
    }

    char program[MAX_PROGRAM_NAME];
    strcpy(program, argv[1]);

    if (strchr(program, '.') == NULL) 
        strcat(program, ".quati");

    buf->source = fopen(program, "r");
}


static uint32_t findBreakLineOnBuffer(Buffer * buf, char dir)
{
    if (buf->bufferPos + dir < 0 || buf->bufferPos + dir > buf->bufferSize)
        return buf->bufferPos;

    int32_t position = buf->bufferPos;

    int32_t start = buf->bufferPos - MAX_LINE_LIMIT < 0 ? 0 : buf->bufferPos - MAX_LINE_LIMIT;
    int32_t end = buf->bufferPos + MAX_LINE_LIMIT > buf->bufferSize ? buf->bufferSize : buf->bufferPos + MAX_LINE_LIMIT;

    while (position > start && position < end)
        position = position + dir;

    return position;
}


char * getBufferSlice(Buffer * buf)
{
    int32_t startLine = findBreakLineOnBuffer(buf, -1);
    int32_t endLine = findBreakLineOnBuffer(buf, 1);
    int32_t length = endLine - startLine;

    char * slice = (char *) malloc(length + 1);

    if (!slice)
        return NULL;

    memcpy(slice, buf->buffer + startLine, length);
    slice[length] = '\0';

    return slice;
}

char getNextChar(Buffer * buf) 
{
    if (buf->bufferPos < buf->bufferSize)
        return buf->buffer[buf->bufferPos++];

    if (fgets(buf->buffer, MAX_LINE_SIZE - 1, buf->source)) 
    {
        buf->bufferPos = 0;
        buf->bufferSize = strlen(buf->buffer);

        return buf->buffer[buf->bufferPos++];
    }

    return TOK_EOF;
}


uint32_t getBufferPosition(Buffer * buf) 
{
    return buf->bufferPos;
}

void backCaracter(Buffer * buf)
{
    buf->bufferPos = buf->bufferPos > 0 ? buf->bufferPos - 1 : 0;
}

