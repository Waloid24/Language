#ifndef ASMFORMAT_H
#define ASMFORMAT_H

#include "tree.h"
#include "MY_ASSERT.h"

typedef struct {
    char * name;
    size_t num;
} var_t;

void asmFormat (node_t * node, char * nameFile);

#endif