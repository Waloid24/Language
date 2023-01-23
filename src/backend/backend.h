#ifndef BACKEND_H
#define BACKEND_H


#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
// #include "../frontend/frontend.h"
#include "tree.h"
#include "graphDumpTree.h"
#include "MY_ASSERT.h"


char * getCode (const char ** nameFile);

#endif