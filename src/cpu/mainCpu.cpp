#include "cpu.h"

int main (int argc, char * argv[])
{
    MY_ASSERT (argc != 2, "Too few files");

    FILE * binFile = codeFile (argv[1]);

    size_t nStrs = readNumStrs (binFile);

    int * code = getCode (nStrs, binFile);
    
    cpu (code, nStrs, MAX_RAM, binFile);

    fclose (binFile);
    return 0;
}