#ifndef ASM_H
#define ASM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../string/myStrcmp.h"
#include "../common/readCode.h"
#include "../debug/MY_ASSERT.h"

#define STANDART_SIZE 20
#define LONG_LINE "-----------------------------------------------------------------------------------------------------\n"

enum {
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3
};

struct tag {
    int * ptr;
    char * nameTag;
};
typedef struct tag tag_t;

struct freeCall {
    int * ptrToArrWithCode;
    char * tag;
    int numFreeCall;
};
typedef struct freeCall freeCall_t;

void createBinFile (char ** arrStrs, code_t * prog, char * nameBinFile, size_t numTags);
void pushSignature (char * nameBinFile, code_t code);
void dumpCode (char ** arrStrs, int * code, size_t numElem);

#endif