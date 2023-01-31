#ifndef BACKEND_H
#define BACKEND_H


#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include "../tree/tree.h"
#include "../tree/graphDumpTree.h"
#include "../debug/MY_ASSERT.h"


char * getCode (const char ** nameFile);

#endif