#include "utils.h"
#include <stdio.h>
#include <stdlib.h>



void lexialError(unsigned int line, const char * character)
{
    fprintf(stderr, "\nErro: Não esperava o caracter \"%s\".\n", character);
    fprintf(stderr, "\t%d | %s\n\n", line, getBufferSlice());

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

