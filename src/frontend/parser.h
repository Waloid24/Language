#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "../tree/tree.h"
#include "../tree/graphDumpTree.h"

node_t * getGeneral (token_t ** tokens);

#endif