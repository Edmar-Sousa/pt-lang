#include "buffer.h"
#include "includes.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void initBuffer(Buffer * buf)
{
    memset(buf->buffer, '\0', MAX_BUFFER_SIZE);

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

    uint32_t position = buf->bufferPos;

    while (
        position + dir >= 0 && position >= buf->bufferPos - MAX_LINE_LIMIT
        && position + dir < buf->bufferSize && position <= buf->bufferPos + MAX_LINE_LIMIT
        && buf->buffer[position] != '\n'
    ) {
        position = position + dir;
    }

    return position;
}


char * getBufferSlice(Buffer * buf)
{
    uint32_t startLine = findBreakLineOnBuffer(buf, -1);
    uint32_t endLine = findBreakLineOnBuffer(buf, 1);
    uint32_t length = endLine - startLine;


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

    if (fgets(buf->buffer, MAX_BUFFER_SIZE - 1, buf->source)) 
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

