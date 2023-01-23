#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <string.h>

enum standartFunc {
    NO_SF           = 0,
    SIN_T           = 1,
    COS_T           = 2,
    LN_T            = 3,
    PRINT_T         = 4,
    SCAN_T          = 5
};

enum keyword {
    NO_KEY          = 0,
    IF_T            = 1,
    DECISION_T      = 2,
    WHILE_T         = 3,
    RETURN_T        = 4,
    DEFINE_T        = 5,
    FUNC_T          = 6,
    PARAM_T         = 7,
    STATEMENT_T     = 8,
    CONST_T         = 9,
    MAIN_T          = 10,
    CALL_T          = 11,
    INITIALIZER_T   = 12
};

enum operation {
    NO_OP           = 0,
    OP_LESS_OR_EQ   = 1,
    OP_GR_OR_EQ     = 2,
    OP_LOG_AND      = 3,
    OP_LOG_OR       = 4,
    OP_NOT_EQUAL    = 5,
    OP_CELEBRATION  = 6,
    OP_ADD          = '+',
    OP_SUB          = '-',
    OP_MUL          = '*',
    OP_DIV          = '/',
    OP_DEG          = '^',
    OP_ASSIGN       = '=',
    OP_LESS         = '<',
    OP_GR           = '>',
    OP_DENIAL       = '!'
};

enum identifier {
    NO_ID = 0,
    ID_FUNC = 1,
    ID_VAR = 2
};

typedef double elem_t;

struct nodeTree {
    struct nodeTree * parent;
    struct nodeTree * left;
    struct nodeTree * right;

    enum operation op_t;
    enum standartFunc s_func_t;
    enum keyword key_t;
    enum identifier id_t;

    bool isNum;
    elem_t elem;
    char * name;
    const char * supportName;
};

typedef struct nodeTree node_t;

node_t * createNodeWithNum (elem_t num);
node_t * createNodeWithOperation (enum operation oper, node_t * nodeL, node_t * nodeR, const char * operName);
node_t * createKeyNode (enum keyword type, node_t * nodeL, node_t * nodeR, const char * supportName); 
node_t * createNodeWithVariable (char * nameOfVar);
node_t * createNodeWithFunction (char * funcName);

void insert_node (node_t * node, node_t * root);
node_t * copyNode (node_t * nodeForCopy);
void deleteNode (node_t * node);
void deleteTree (node_t * node);

#endif
