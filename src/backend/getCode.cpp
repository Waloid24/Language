#include "getCode.h"

static FILE * openFile(const char * nameFile)
{
    MY_ASSERT (nameFile == nullptr, "Wrong pointer to name file with source code");

    FILE *file = fopen (nameFile, "rb");
    MY_ASSERT (file == nullptr, "Unable to open the source file");

    return file;
}

static size_t sizeFile (const char * argConsole)
{
    struct stat code = {};
    stat (argConsole, &code);

    return (size_t)code.st_size;
}

static void * createArr (size_t size, size_t sizeElem)
{
    void * ptrToArr = calloc (size, sizeElem);
    MY_ASSERT (ptrToArr == nullptr, "Unable to allocate new memory");

    return ptrToArr;
}

static void copyFromFile (FILE * src, void * dst, size_t sizeFile)
{
    MY_ASSERT (src == nullptr, "Wrong pointer to the file");

    fread (dst, sizeof(char), sizeFile, src);
}

char * getCode (const char * nameFile)
{
    FILE * fileCode = openFile (nameFile);
    size_t sizeArr = sizeFile (nameFile);
    char * code = (char *) createArr(sizeArr + 1, sizeof(char));
    copyFromFile (fileCode, code, sizeArr);
    code[sizeArr] = '\0';

    fclose (fileCode);

    return code;
}