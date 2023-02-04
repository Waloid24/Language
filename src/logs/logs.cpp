#include "logs.h"

#define DEF_CMD(nameCmd, value, ...)\
    const int nameCmd = value;

#include "../common/cmd.h"
#include "../debug/MY_ASSERT.h"

#undef DEF_CMD

static FILE * openLogfile (const char * nameFile);

FILE * openLogfile (const char * nameFile)
{
    FILE * file = fopen (nameFile, "ab");
    MY_ASSERT (file == nullptr, "Unable to open the file");
    setbuf (file, NULL);
    return file;
}

void dumpLexer (token_t * tokens, size_t numTokens)
{
    FILE * logfile = openLogfile ("../logLexer.txt");
    for (size_t i = 0; i < numTokens; i++)
    {
        fprintf (logfile, "tokens[%zu]\n", i);
        fprintf (logfile, "----> type = %d\n", tokens[i].type);
        if (tokens[i].type == TYPE_ID)
        {
            fprintf (logfile, "----> id   = %s\n", tokens[i].u1.id);
        }
        else if (tokens[i].type == TYPE_SIN || tokens[i].type == TYPE_COS ||
                tokens[i].type == TYPE_LN ||  tokens[i].type == TYPE_PRINT ||
                tokens[i].type == TYPE_SCAN || tokens[i].type == TYPE_SQRT) 
        {
            fprintf (logfile, "----> id   = %s\n", tokens[i].u1.id);
        }
        else if (tokens[i].type == TYPE_NUM)
        {
            fprintf (logfile, "----> val  = %d\n", tokens[i].u1.value);
        }
        
    }
    fclose (logfile);
}

