#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "stack.h"

#define DEF_CMD(name, numLetters, numCmd, isArg, ...)    \
    CMD_##name = numCmd,

typedef enum {
    #include "cmd.h"
} COMMANDS;

#undef DEF_CMD

const int RAM = 7; //0000 | 0001  -> 1000 | 0000
const int REG = 6; //0000 | 0001  -> 0100 | 0000
const int NUM = 5; //0000 | 0001  -> 0010 | 0000

const size_t MAX_RAM = 100;
const int MASK = (1<<4) + (1<<3) + (1<<2) + (1<<1) + 1; //0001|1111

void cpu (int * code, size_t nStrs, size_t numTags, FILE * binFile);
size_t readNumStrs (FILE * binFile);
FILE * codeFile (char * nameFile);
int * getCode (size_t nStrs, FILE * binFile);
void dumpCode (int * code, size_t numElem);

#endif
