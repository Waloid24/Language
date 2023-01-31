#include "generator.h"

int main (int argc, char * argv[])
{
    char * code = getCode (argv[1]);
    char * tmpCode = code;

    node_t * node = createTree (&code);

    graphicDumpTree (node);

    asmFormat (node, argv[2]);

    graphicDumpTree (node);

    deleteTree (node, true);

    free (tmpCode);

    return 0;
}


