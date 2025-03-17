#include "utils.h"
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>



void lexialError(unsigned int line, const char character)
{
    fprintf(stderr, "\n\033[31mErro: Não esperava o caracter \"%c\".\033[m\n\n", character);
    fprintf(stderr, "\t%d | %s\n", line, getBufferSlice());
    fprintf(stderr, "\t%*s^-- Aqui.\n\n", getBufferPosition() + 3, "");

    exit(EXIT_FAILURE);
}


void syntaxeError(unsigned int line, const char * world)
{
    fprintf(stderr, "\n\033[31mSintaxe Inválida: Não esperava por \"%s\".\033[m\n\n", world);
    fprintf(stderr, "\t%d | %s\n", line, getBufferSlice());
    fprintf(stderr, "\t%*s\n\n", getBufferPosition(), "^");

    exit(EXIT_FAILURE);
}

