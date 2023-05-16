#include <stdio.h>
#include "../common/readCode.h"
#include "cpu.h"
#include "asm.h"
#include "../debug/MY_ASSERT.h"

static char * nameWithPath (const char * path, char * nameFileWithoutDir);

const int NUM_TAGS = 20;

int main (int argc, char * argv[])
{
    MY_ASSERT (argc < 5, "Too few argument in a command line, you need 5");
    MY_ASSERT (argc > 5, "Too many arguments in a command line, you need 5");
    code_t codeInfo = {};

    char * nameSrcFile = nameWithPath ("./bin/", argv[2]);
    codeInfo = readCode (nameSrcFile);

    // int isRem = remove (nameSrcFile);
    // MY_ASSERT (isRem != 0, "Error deleting a file");

    MY_ASSERT (codeInfo.nStrs == 0, "There are no lines of code in your program");

    char * nameDstFile = nameWithPath ("./bin/", argv[4]);
    pushSignature (nameDstFile, codeInfo);

    createBinFile (codeInfo.arrStrs, &codeInfo, nameDstFile, NUM_TAGS);

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
