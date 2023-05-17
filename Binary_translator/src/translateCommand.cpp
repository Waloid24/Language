#include "../include/translateCommand.hpp"



void JITCompile (compilerInfo_t * compilerInfo)
{
    MY_ASSERT (compilerInfo->irInfo.irArray == nullptr, "There is no access to commands array");
    MY_ASSERT (compilerInfo->irInfo.irArray[0].cmd == CMD_POP, "First command is pop, but stack is empty");

    size_t sizeArr = compilerInfo->irInfo.sizeArray;
    ir_t * irArr   = compilerInfo->irInfo.irArray;

    char * machineCode = (char *) calloc (sizeArr*4, sizeof(char));

    for (size_t irIndex, machineIndex = 0; irIndex < sizeArr; irIndex++, machineIndex++)
    {
        if (irArr[irIndex].cmd == CMD_PUSH)
        {
            machineCode[machineIndex] = 3;
        }
    }
}

void insertBinPush (char * machineCode, ir_t irCommand)
{
    char binPush[] = {};



}