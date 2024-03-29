#ifndef LEXER_H
#define LEXER_H

#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include "../debug/MY_ASSERT.h"
#include "../common/readCode.h"

typedef double elem_t;

struct token {
    int type;
    union {
        int value;
        char * id;
    } u1;
};
typedef struct token token_t;

struct retLex 
{
    token_t * tokens;
    size_t nTokens;
};
typedef struct retLex retLex_t;

const size_t STANDART_SIZE_TOKEN_ARR = 10;

retLex_t getTokens (char * code);

#endif