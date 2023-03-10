#include <stdio.h>
#include "../common/readCode.h"
#include "cpu.h"
#include "asm.h"
#include "../debug/MY_ASSERT.h"

const int NUM_TAGS = 20;

int main (int argc, char * argv[])
{
    MY_ASSERT (argc != 3, "Too few command line arguments!");
    code_t codeInfo = {};

    codeInfo = readCode (argv[1]);

    MY_ASSERT (codeInfo.nStrs == 0, "There are no lines of code in your program");

    pushSignature (argv[2], codeInfo);

    createBinFile (codeInfo.arrStrs, &codeInfo, argv[2], NUM_TAGS);
}

