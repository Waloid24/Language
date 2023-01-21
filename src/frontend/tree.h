#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <string.h>

enum nodeType {
    OPER_T          = 1,
    VAR_T           = 2,
    NUM_T           = 3,
    FUNC_T          = 4,
    CONST_T         = 5,
    STATEMENT_T     = 6,
    DECISION_T      = 7,
    DEFINE_T        = 8,
    PARAM_T         = 9,
    RETURN_T        = 10,
    IF_T            = 11,
    WHILE_T         = 12
};

enum operationType {
    OP_ERROR        = 0,
    OP_ADD          = '+',
    OP_SUB          = '-',
    OP_MUL          = '*',
    OP_DIV          = '/',
    OP_DEG          = '^',
    OP_ASSIGN       = '=',
    OP_LESS_OR_EQ   = '<=',
    OP_GR_OR_EQ     = '>=',
    OP_LOG_AND      = '&&',
    OP_LOG_OR       = '||'
};

typedef double elem_t;

struct nodeTree {
    struct nodeTree * parent;
    elem_t elem;
    char * varName;
    const char * nameFunc;
    const char * nameInst;
    struct nodeTree * left;
    struct nodeTree * right;

    enum nodeType type;
    enum operationType op_t;
};

typedef struct nodeTree node_t;

node_t * createNodeWithNum (elem_t num);
node_t * createNodeWithOperation (int operation, node_t * nodeL, node_t * nodeR); //enum operationType operation
node_t * createNodeInstruction (enum nodeType type, node_t * nodeL, node_t * nodeR);
node_t * createNodeWithVariable (char * nameOfVar);
node_t * createNodeWithFunction (const char * nameFunction, node_t * leftDescendant);
node_t * createTerminalNode (enum nodeType type, node_t * condition, node_t * nodeL, node_t * nodeR);

void insert_node (node_t * node, node_t * root);
node_t * copyNode (node_t * nodeForCopy);
void deleteNode (node_t * node);
void deleteTree (node_t * node);

#endif
