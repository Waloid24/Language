#ifndef GETCODE_H
#define GETCODE_H

#include <stdio.h>
#include <sys/stat.h>
#include "MY_ASSERT.h"

char * getCode (const char * nameFile);
void * allocateMemory (size_t numElem, size_t sizeElem);
FILE * openFile(const char * nameFile, const char * format);

#endif