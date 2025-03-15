#ifndef __QUATI_UTILS_H__
#define __QUATI_UTILS_H__

#include <stdio.h>
#include "includes.h"

void getProgram(int argc, char ** argv);
void lexialError(unsigned int line, const char * character);
void exitOnError(const char * message, ...);

#endif