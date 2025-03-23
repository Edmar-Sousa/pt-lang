#include "buffer.h"
#include "globals.h"
#include "scan.h"
#include "parser.h"
#include <stdio.h>


Buffer buf;
Scan scan;


void init(int argc, char ** argv)
{
    initBuffer(&buf);
    openProgramFile(&buf, argc, argv);

    initScan(&scan, &buf);
}


int main(int argc, char ** argv)
{
    init(argc, argv);

    TreeNode * tree = getProgramAST(&scan);


    return 0;
}
