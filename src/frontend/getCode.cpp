#include "getCode.h"

FILE * openFile(const char * nameFile, const char * format)
{
    MY_ASSERT (nameFile == nullptr, "Wrong pointer to name file with source code");

    FILE *file = fopen (nameFile, format);
    MY_ASSERT (file == nullptr, "Unable to open the source file");
    setbuf (file, NULL);

    return file;
}

static size_t sizeFile (const char * argConsole)
{
    struct stat code = {};
    stat (argConsole, &code);

    return (size_t)code.st_size;
}

void * allocateMemory (size_t numElem, size_t sizeElem)
{
    void * ptr = calloc (numElem, sizeElem);
    MY_ASSERT (ptr == nullptr, "Unable to allocate new memory");
    return ptr;
}

static void copyFromFile (FILE * src, void * dst, size_t sizeFile)
{
    MY_ASSERT (src == nullptr, "Wrong pointer to the file");

    fread (dst, sizeof(char), sizeFile, src);
}

char * getCode (const char * nameFile)
{
    FILE * fileCode = openFile (nameFile, "rb");
    size_t sizeArr = sizeFile (nameFile);
    char * code = (char *) allocateMemory(sizeArr + 1, sizeof(char));
    copyFromFile (fileCode, code, sizeArr);
    code[sizeArr] = '\0';

    fclose (fileCode);

    return code;
}