#include "../include/processFileWithCode.hpp"
#include "../include/lexer.hpp"
#include <string.h>

void graphvizDumpIR (cmd_t * commandsArray, size_t numElem);

int main (int argc, char * argv[])
{
    MY_ASSERT (argc != 2, "You should enter 2 arguments: executable file and bytecode file");
    code_t code = readCode (argv[1]);

    dumpCode ((int *) code.buf, code.sizeBuf/sizeof(int));

    cmd_t * commandsArray = createIRArray (code);

    size_t numCmds = setIR (commandsArray, (int *) code.buf);
    printf ("numCmds = %zu\n", numCmds);
    graphvizDumpIR (commandsArray, numCmds);

    free (code.buf);

    return 0;
}


#define dumpline(text, ...)\
		fprintf (IRdumpFile, text, ##__VA_ARGS__)

static int isJump (int cmd)
{
    if (cmd == 9 || (cmd >= 12 && cmd <= 17) )
    {
        return 1;
    }
    return 0;
}

static int isPushPop (int cmd)
{
    if (cmd == 1 || cmd == 2)
    {
        return 1;
    }
    return 0;
}

void graphvizDumpIR (cmd_t * commandsArray, size_t numElem) 
{
    MY_ASSERT (commandsArray == nullptr, "");

    FILE* IRdumpFile = fopen("./graph/ir_dump.dot", "w");
    MY_ASSERT (IRdumpFile == nullptr, "Unable to open the file");

    dumpline("digraph {\n");
    dumpline("rankdir=LR;\n");
    dumpline("node [ shape=record ];\n");

    for (size_t i = 0; i < numElem; i++) 
    {
        const char* name = commandsArray[i].name;
        if (commandsArray[i].cmd != 0)
        {
            if (isJump(commandsArray[i].cmd)) 
            {
                dumpline("struct%zu [\nlabel = \"<index> index: %zu|<name>name: %s|<size> size(native): %d\", style = \"filled\", fillcolor = \"cyan\" \n];\n", i, i, name, commandsArray[i].nativeSize);
            }
            else if (isPushPop(commandsArray[i].cmd)) 
            {
                dumpline("struct%zu [\nlabel = \"<index> index: %zu|<name>name: %s|<size> size(native): %d\", style = \"filled\", fillcolor = \"green\" \n];\n", i, i, commandsArray[i].name, commandsArray[i].nativeSize);
            }
            else 
            {
                dumpline("struct%zu [\nlabel = \"<index> index: %zu|<name>name: %s|<size> size(native): %d \", style = \"filled\", fillcolor = \"gray\" \n];\n", i, i, commandsArray[i].name, commandsArray[i].nativeSize);
            }
            if (i == 0)
            {
                continue;
            }

            dumpline("struct%zu -> struct%zu [weight=100];\n", i-1, i);
        }
    }
    dumpline("}\n");

    fclose (IRdumpFile);

    char cmd[100] = {0};

    sprintf(cmd, "dot -T png ./graph/ir_dump.dot -o ./graph/IR_DUMP.png");
    system(cmd);
}

