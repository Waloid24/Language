#include "generator.h"

static char * nameWithPath (const char * path, char * nameFileWithoutDir);

int main (int argc, char * argv[])
{
    MY_ASSERT (argc < 5, "Too few argument in a command line, you need 5");
    MY_ASSERT (argc > 5, "Too many arguments in a command line, you need 5");

    char * nameSrcFile = nameWithPath ("./bin/", argv[2]);
    char * code = getCode (nameSrcFile);
    char * tmpCode = code;

    node_t * node = createTree (&code);

    // int isRem = remove (nameSrcFile);
    // MY_ASSERT (isRem != 0, "Error deleting a file");

    char * nameDstFile = nameWithPath ("./bin/", argv[4]);
    asmFormat (node, nameDstFile);

    deleteTree (node, true);

    free (tmpCode);
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