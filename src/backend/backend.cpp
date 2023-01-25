#include "generator.h"

int main (int argc, char * argv[])
{
    char * code = getCode (argv[1]);

    node_t * node = createTree (&code);

    graphicDumpTree (node);

    asmFormat (node, argv[2]);

    deleteTree (node);

    return 0;
}

