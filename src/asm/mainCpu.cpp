#include "cpu.h"


static char * nameWithPath (const char * path, char * nameFileWithoutDir);

int main (int argc, char * argv[])
{
    MY_ASSERT ((argc < 3) || (argc > 3), "Incorrect number of arguments, you need 3");

    char * nameSrcFile = nameWithPath ("./bin/", argv[2]);
    FILE * binFile = codeFile (nameSrcFile);

    size_t nStrs = readNumStrs (binFile);

    int * code = getCode (nStrs, binFile);

    
        cpu (code, nStrs, MAX_RAM, binFile);

    

    free (nameSrcFile);
    fclose (binFile);
    return 0;
}

static char * nameWithPath (const char * path, char * nameFileWithoutDir)
{
    MY_ASSERT (path == nullptr, "No access to the file path");
    MY_ASSERT (path == nullptr, "No access to the file name");
    
    size_t lengthName = strlen (nameFileWithoutDir);
    size_t lengthPath = strlen (path);
    char * fullName = (char *) calloc (lengthName + lengthPath + 1, sizeof(char));
    MY_ASSERT (fullName == nullptr, "Unable to allocate new memory");

    memcpy (fullName, path, sizeof(char)*lengthPath);
    memcpy (fullName + lengthPath, nameFileWithoutDir, sizeof(char)*lengthName);

    fullName[lengthName + lengthPath] = '\0';

    return fullName;
}