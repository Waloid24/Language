#ifndef PROCESS_FILE_WITH_CODE
#define PROCESS_FILE_WITH_CODE

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "../src/logs/logs.hpp"

struct codeInfo
{
    size_t sizeBuf;
    void * buf;
    size_t nStrs;
};
typedef struct codeInfo code_t;

code_t readCode (const char * nameFile);

#endif