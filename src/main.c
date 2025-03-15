#include "globals.h"
#include "utils.h"
#include "scan.h"



int main(int argc, char ** argv)
{
    getProgram(argc, argv);
    TokenType tok;

    while ((tok = getNextToken()) != TOK_EOF);

    return 0;
}
