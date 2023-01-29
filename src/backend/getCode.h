#ifndef GETCODE_H
#define GETCODE_H

#include <stdio.h>
#include <sys/stat.h>
#include "MY_ASSERT.h"

char * getCode (const char * nameFile);
FILE * openFile(const char * nameFile, const char * mode);
void * allocateMemory (size_t size, size_t sizeElem);

#endif