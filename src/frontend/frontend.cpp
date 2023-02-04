#include "../debug/MY_ASSERT.h"
#include "lexer.h"
#include "parser.h"
#include "../tree/graphDumpTree.h"
#include "textTree.h"
#include "simplify.h"
#include "../common/readCode.h"

#define DEF_CMD(nameCmd, value, ...)\
    const int nameCmd = value;

#include "../common/cmd.h"

#undef DEF_CMD

static char * nameWithPath (const char * path, char * nameFileWithoutDir);

int main (int argc, char * argv[])
{
    MY_ASSERT (argc < 5, "Too few argument in a command line, you need 5");
    MY_ASSERT (argc > 5, "Too many arguments in a command line, you need 5");

    char * nameSrcFile = nameWithPath ("./tests/", argv[2]);
    char * code = getCode (nameSrcFile);

    retLex_t tokenResult = getTokens (code);
    token_t * tmp = tokenResult.tokens;

    node_t * node = getGeneral(&tmp);

    simplify (&node);

    graphicDumpTree (node);

    char * nameDstFile = nameWithPath ("./bin/", argv[4]);
    textTree (node, nameDstFile);

    free (code);

    deleteTree (node, false);

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
    free (nameSrcFile);
    free (nameDstFile);
    return 0;
}

static char * nameWithPath (const char * path, char * nameFileWithoutDir)
{
    MY_ASSERT (path == nullptr, "No access to the file path");
    MY_ASSERT (path == nullptr, "No access to the file name");
    
    size_t lengthName = strlen (nameFileWithoutDir);
    size_t lengthPath = strlen (path);
    char * fullName = (char *) allocateMemory (lengthName + lengthPath + 1, sizeof(char));

    memcpy (fullName, path, sizeof(char)*lengthPath);
    memcpy (fullName + lengthPath, nameFileWithoutDir, sizeof(char)*lengthName);

    fullName[lengthName + lengthPath] = '\0';

    return fullName;
}