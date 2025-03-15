#include "utils.h"
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>



void lexialError(unsigned int line, const char * character)
{
    fprintf(stderr, "\nErro: Não esperava o caracter \"%s\".\n\n", character);
    fprintf(stderr, "\t%d | %s\n", line, getBufferSlice());
    fprintf(stderr, "\t%*s^-- Aqui.\n\n", getBufferPosition() + 3, "");

    exit(EXIT_FAILURE);
}


void exitOnError(const char * message, ...)
{
    va_list args;
    va_start(args, message);

    fprintf(stderr, "[Quati] ERROR: ");
    vfprintf(stderr, message, args);

    va_end(args);
    exit(EXIT_FAILURE);
}

