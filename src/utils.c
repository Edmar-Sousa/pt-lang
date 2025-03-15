#include "utils.h"
#include <stdio.h>
#include <stdlib.h>



void lexialError(unsigned int line, const char * character)
{
    fprintf(stderr, "Erro: Não esperava o caracter \"%s\".\n", character);
    fprintf(stderr, "\t%d | [buffer]\n", line);

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