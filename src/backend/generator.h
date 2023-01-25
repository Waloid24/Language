#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "getCode.h"
#include "tree.h"
#include "graphDumpTree.h"
#include "MY_ASSERT.h"
#include "debug.h"
#include "asmFormat.h"

node_t * createTree (char ** code);

#endif