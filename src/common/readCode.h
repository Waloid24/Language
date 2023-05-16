#ifndef READ_CODE_H
#define READ_CODE_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "../debug/MY_ASSERT.h"

struct codeInfo
{
    size_t sizeFile;
    char * buf;
    size_t nStrs;
    char ** arrStrs;
};
typedef struct codeInfo code_t;

code_t readCode (const char * nameFile);
void * allocateMemory (size_t size, size_t sizeElem);
FILE * openFile(const char * nameFile, const char * format);
char * getCode (const char * nameFile);

#endif
