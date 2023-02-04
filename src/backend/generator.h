#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../common/readCode.h"
#include "../tree/tree.h"
#include "../tree/graphDumpTree.h"
#include "../debug/MY_ASSERT.h"
#include "../debug/debug.h"
#include "asmFormat.h"

node_t * createTree (char ** code);

#endif