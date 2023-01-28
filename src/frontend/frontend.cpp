#include "frontend.h"

#define DEF_CMD(nameCmd, value, ...)\
    const int nameCmd = value;

#include "cmd.h"

#undef DEF_CMD

int main (int argc, char * argv[])
{
    MY_ASSERT (argc != 3, "Too few argument in a command line");

    char * code = getCode (argv[1]);

    retLex_t tokenResult = getTokens (code);
    token_t * tmp = tokenResult.tokens;

    node_t * node = getGeneral(&tmp);

    graphicDumpTree (node);

    textTree (node, argv[2]);

    free (code);

    deleteTree (node);

    for (size_t i = 0; i < tokenResult.nTokens; i++)
    {
        if (tokenResult.tokens[i].type == TYPE_ID || tokenResult.tokens[i].type == TYPE_COS ||
            tokenResult.tokens[i].type == TYPE_SIN || tokenResult.tokens[i].type == TYPE_LN ||
            tokenResult.tokens[i].type == TYPE_PRINT || tokenResult.tokens[i].type == TYPE_SCAN ||
            tokenResult.tokens[i].type == TYPE_SQRT)
        {
            free (tokenResult.tokens[i].u1.id);
        }
    }
    free (tokenResult.tokens);
    return 0;
}