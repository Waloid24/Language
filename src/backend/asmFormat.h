#ifndef ASMFORMAT_H
#define ASMFORMAT_H

#include "tree.h"
#include "MY_ASSERT.h"
#include "getCode.h"

typedef struct {
    char * name;
    size_t num;
} var_t;

const size_t PLACE_GLOB_VARS = 0;
const size_t MAX_NUM_GLOB_VARS = 10;
const size_t MAX_NUM_VARS = 50;

void asmFormat (node_t * node, char * nameFile);

#endif