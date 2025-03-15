#include "utils.h"


void exitOnError(const char * message, ...)
{
    va_list args;
    va_start(args, message);

    fprintf(stderr, "[CSL] ERROR: ");
    vfprintf(stderr, message, args);

    va_end(args);
    exit(EXIT_FAILURE);
}


void getProgram(int argc, char ** argv) 
{
    if (argc < 2) {
        printf("[CSL] Execute \"csl <program.ptl>\"\n");
        exit(EXIT_FAILURE);
    }

    char program[MAX_PROGRAM_NAME];
    strcpy(program, argv[1]);

    if (strchr(program, '.') == NULL) 
        strcat(program, ".ptl");

    source = fopen(program, "r");
}