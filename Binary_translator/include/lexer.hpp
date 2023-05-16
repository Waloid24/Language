#ifndef LEXER_HPP
#define LEXER_HPP

#include "../src/logs/logs.hpp"
#include "processFileWithCode.hpp"
#include <stdlib.h>

typedef enum argument {
    NUMBER      = 1,
    REGISTER    = 2,
    NUM_REG     = 3,
    MEM_NUM     = 4,
    MEM_REG     = 5,
    MEM_NUM_REG = 6,
    LABEL
} argument_t;

typedef struct command {

    const char * name;
    int cmd;
    int nativeSize;
    size_t nativeIP;
    argument_t type_argument;
    int argument;

} cmd_t;

cmd_t * createIRArray (code_t code);
size_t setIR (cmd_t * commandsArray, int * byteCode);

#endif