#include "parser.h"

#define DEF_CMD(nameCmd, value, ...)\
    const int nameCmd = value;

#include "cmd.h"

#undef DEF_CMD

size_t STANDART_NUM_OF_INSTRUCTION = 5;

static node_t * getDefine (token_t ** tokens);
static node_t * getStatement (token_t ** tokens);
static node_t * getAssign (token_t ** tokens);
static node_t * getTerminational (token_t ** tokens);
static node_t * getCycle (token_t ** tokens);
static node_t * getConditional (token_t ** tokens);
static node_t * getExpression (token_t ** tokens);
static node_t * getBoolean (token_t ** tokens);
static node_t * getArithmetic (token_t ** tokens);
static node_t * getTerm (token_t ** tokens);
static node_t * getPrimary (token_t ** tokens);

static void checkSemicolon (token_t ** tokens);
static node_t ** increaseMemory (node_t ** ptr, size_t * size);
static void * allocateMemory (size_t numElem, size_t sizeElem);
static node_t * createParams (token_t ** tokens);

node_t * getGeneral (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    node_t * node = nullptr;

    if (((*tokens)+1)->type != TYPE_O_BRACKET)
    {
        node = getAssign (tokens);
        return node;
    }
    else if (((*tokens)+1)->type == TYPE_O_BRACKET)
    {
        node = getDefine (tokens);
        return node;
    }
    else
    {
        MY_ASSERT (1, "Wrong command");
        return nullptr;
    }
}

static node_t * getDefine (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    node_t * nameFunc = getPrimary (tokens);

    if ((*tokens)->type != TYPE_O_BRACKET)
    {
        MY_ASSERT (1, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    }
    (*tokens)++;

    node_t ** multipleParams = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));

    if ((*tokens)->type != TYPE_C_BRACKET) 
    {
        multipleParams[0] = createParams (tokens);

        for (size_t i = 0; (*tokens)->type != TYPE_C_BRACKET; i++)
        {
            if (i == STANDART_NUM_OF_INSTRUCTION-1)
            {
                multipleParams = increaseMemory (multipleParams, &STANDART_NUM_OF_INSTRUCTION);
            }
            node_t * node = createParams (tokens);
            multipleParams[i]->left = node;
            node->parent = multipleParams[i];
            multipleParams[i+1] = node;
        }
        (*tokens)++;
    }

    if ((*tokens)->type != TYPE_O_F_BRACKET)
    {
        MY_ASSERT (1, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    }
    (*tokens)++;

    node_t ** multipleInstructions = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));
    multipleInstructions[0] = getStatement (tokens);

    for (size_t i = 0; (*tokens)->type != TYPE_C_F_BRACKET; i++)
    {
        if (i == STANDART_NUM_OF_INSTRUCTION-1)
        {
            multipleInstructions = increaseMemory (multipleInstructions, &STANDART_NUM_OF_INSTRUCTION);
        }
        node_t * node = getStatement (tokens);
        multipleInstructions[i]->left = node;
        node->parent = multipleInstructions[i];
        multipleInstructions[i+1] = node;
    }
    (*tokens)++;

    node_t * funcSignature = createNodeInstruction (FUNC_T, nameFunc, multipleParams[0]);
    node_t * headNode = createNodeInstruction (DEFINE_T, funcSignature, multipleInstructions[0]);

    return headNode;
}

static node_t * getStatement (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    node_t * node = nullptr;

    if ((*tokens)->type == TYPE_IF)
    {
        printf ("\033[31m erroneous entry1 \033[0m \n");
        node = getConditional (tokens);
    }
    else if ((*tokens)->type == TYPE_WHILE)
    {
        printf ("\033[31m erroneous entry2 \033[0m \n");
        node = getCycle (tokens);
    }
    else if ((*tokens + 1)->type == TYPE_ASSIGN)
    {
        node = getAssign (tokens);
    }
    else if ((*tokens)->type == TYPE_RETURN)
    {
        node = getTerminational (tokens);
    }
    else
    {
        printf ("\033[31m erroneous entry3 \033[0m \n");
        node = getExpression (tokens);
    }

    node_t * headNode = createNodeInstruction (STATEMENT_T, nullptr, node);

    return headNode;
}

static node_t * getAssign (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    node_t * node = getPrimary (tokens);

    (*tokens)++; //to skip a "="
    MY_ASSERT (node->type != VAR_T, "It is necessary to have a variable before the assignment sign");
    
    printf ("in getAssign (*tokens)->type = %d\n", (*tokens)->type);
    node_t * node_1 = getExpression (tokens);
    node_t * headNode = createNodeWithOperation (OP_ASSIGN, node, node_1);

    checkSemicolon (tokens);

    return headNode;
}

static node_t * getTerminational (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    (*tokens)++; //skip 'return'
    node_t * node = getExpression (tokens);
    checkSemicolon (tokens);
    node_t * headNode = createNodeInstruction (RETURN_T, nullptr, node);

    return headNode;
}

static node_t * getCycle (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    (*tokens)++;

    if ((*tokens)->type != TYPE_O_BRACKET)
    {
        MY_ASSERT (1, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    }
    (*tokens)++;

    node_t * condition = getExpression (tokens);

    if ((*tokens)->type != TYPE_C_BRACKET)
    {
        MY_ASSERT (1, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    }
    (*tokens)++;

    if ((*tokens)->type != TYPE_O_F_BRACKET)
    {
        MY_ASSERT (1, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    }
    (*tokens)++;

    node_t ** multipleInstructions = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));
    multipleInstructions[0] = getStatement (tokens);

    for (size_t i = 0; (*tokens)->type != TYPE_C_F_BRACKET; i++)
    {
        if (i == STANDART_NUM_OF_INSTRUCTION-1)
        {
            multipleInstructions = increaseMemory (multipleInstructions, &STANDART_NUM_OF_INSTRUCTION);
        }
        node_t * node = getStatement (tokens);
        multipleInstructions[i]->left = node;
        node->parent = multipleInstructions[i];
        multipleInstructions[i+1] = node;
    }
    (*tokens)++;

    node_t * headNode = createTerminalNode (WHILE_T, condition, nullptr, multipleInstructions[0]);

    free (multipleInstructions);

    return headNode;
}

static node_t * getConditional (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");
    printf ("\033[31m erroneous entry5 \033[0m \n");

    (*tokens)++;
    if ((*tokens)->type != TYPE_O_BRACKET)
    {
        MY_ASSERT (1, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    }
    (*tokens)++;

    node_t * condition = getExpression (tokens);

    if ((*tokens)->type != TYPE_C_BRACKET)
    {
        MY_ASSERT (1, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    }
    (*tokens)++;

    if ((*tokens)->type != TYPE_O_F_BRACKET)
    {
        MY_ASSERT (1, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    }
    (*tokens)++;

    node_t ** multipleInstructions = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));
    multipleInstructions[0] = getStatement (tokens);

    for (size_t i = 0; (*tokens)->type != TYPE_C_F_BRACKET; i++)
    {
        if (i == STANDART_NUM_OF_INSTRUCTION-1)
        {
            multipleInstructions = increaseMemory (multipleInstructions, &STANDART_NUM_OF_INSTRUCTION);
        }
        node_t * node = getStatement (tokens);
        multipleInstructions[i]->left = node;
        node->parent = multipleInstructions[i];
        multipleInstructions[i+1] = node;
    }
    (*tokens)++;

    node_t ** multipleInstructionsElse = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));
    if ((*tokens)->type == TYPE_ELSE)
    {
        (*tokens)++;
        if ((*tokens)->type != TYPE_O_F_BRACKET)
        {
            MY_ASSERT (1, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
        }
        (*tokens)++;        

        multipleInstructionsElse[0] = getStatement (tokens);

        for (size_t i = 0; (*tokens)->type != TYPE_C_F_BRACKET; i++)
        {
            if (i == STANDART_NUM_OF_INSTRUCTION-1)
            {
                multipleInstructionsElse = increaseMemory (multipleInstructionsElse, &STANDART_NUM_OF_INSTRUCTION);
            }
            node_t * node = getStatement (tokens);
            multipleInstructionsElse[i]->left = node;
            node->parent = multipleInstructionsElse[i];
            multipleInstructionsElse[i+1] = node; //TODO: it is necessary to correctly add the left nodes to statement
        }
        (*tokens)++;
    }

    node_t * headNode = createTerminalNode (IF_T, condition, multipleInstructions[0], multipleInstructionsElse[0]);

    free (multipleInstructions);
    free (multipleInstructionsElse);

    graphicDumpTree (headNode);

    return headNode;
}

static node_t * getExpression (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "There access to tokens");

    node_t * node_1 = getBoolean (tokens);

    if ((*tokens)->type == TYPE_LOG_AND)
    {
        printf ("\033[31m erroneous entry6 \033[0m \n");
        (*tokens)++; //to skip a logical symbols
        node_t * node_2 = getBoolean (tokens);
        node_t * headNode = createNodeWithOperation (OP_LOG_AND, node_1, node_2);

        return headNode;
    }
    else if ((*tokens)->type == TYPE_LOG_OR)
    {
        printf ("\033[31m erroneous entry7 \033[0m \n");
        (*tokens)++; //to skip a logical symbols
        node_t * node_2 = getBoolean (tokens);
        node_t * headNode = createNodeWithOperation (OP_LOG_OR, node_1, node_2);
        
        return headNode;
    }
    else
    {
        return node_1;
    }
}

static node_t * getBoolean (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "There access to tokens");

    node_t * node_1 = getArithmetic (tokens);

    if ((*tokens)->type == TYPE_GR_OR_EQ || (*tokens)->type == TYPE_LESS_OR_EQ)
    {
        printf ("\033[31m erroneous entry8 \033[0m \n");
        (*tokens)++; //to skip a comparison symbols

        node_t * node_2 = getArithmetic (tokens);

        node_t * headNode = createNodeWithOperation (TYPE_GR_OR_EQ, node_1, node_2); 
        return headNode;
    }

    return node_1;
}

static node_t * getArithmetic (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "There access to tokens");

    node_t * node = getTerm (tokens); // TODO: A code generation must be here. This is the solution for the problem above
    if ((*tokens)->type == TYPE_ADD)
    {
        printf ("\033[31m erroneous entry9 \033[0m \n");
        (*tokens)++;
        node_t * node_1 = getTerm (tokens);
        node_t * headNode = createNodeWithOperation (OP_ADD, node, node_1);
        return headNode;
    }
    else if ((*tokens)->type == TYPE_SUB)
    {
        printf ("\033[31m erroneous entry10 \033[0m \n");
        (*tokens)++;
        node_t * node_1 = getTerm (tokens);
        node_t * headNode = createNodeWithOperation (OP_SUB, node, node_1);
        return headNode;
    }
    else
    {
        return node;
    }
}

static node_t * getTerm (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "There access to tokens");

    node_t * node = getPrimary (tokens);

    if ((*tokens)->type == TYPE_MUL)
    {
        printf ("\033[31m erroneous entry11 \033[0m \n");
        (*tokens)++;
        node_t * node_1 = getPrimary (tokens);
        node_t * headNode = createNodeWithOperation (OP_MUL, node, node_1);
        return headNode;
    }
    else if ((*tokens)->type == TYPE_DIV)
    {
        printf ("\033[31m erroneous entry12 \033[0m \n");
        (*tokens)++;
        node_t * node_1 = getPrimary (tokens);
        node_t * headNode = createNodeWithOperation (OP_DIV, node, node_1);
        return headNode;
    }
    else
    {
        return node;
    }
}

static node_t * getPrimary (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "There access to tokens");

    if ((*tokens)->type == TYPE_SUB)
    {
        printf ("\033[31m erroneous entry13 \033[0m \n");
        (*tokens)++;
        node_t * node_1 = createNodeWithNum (-1);
        node_t * node_2 = getExpression (tokens);
        node_t * headNode = createNodeWithOperation (OP_MUL, node_1, node_2);
        (*tokens)++;
        return headNode;
    }
    else if ((*tokens)->type == TYPE_O_BRACKET)
    {
        printf ("\033[31m erroneous entry14 \033[0m \n");
        (*tokens)++;
        node_t * node = getExpression (tokens);
        (*tokens)++;
        MY_ASSERT ((*tokens)->type != TYPE_C_BRACKET, "You forgot to close the bracket");
        (*tokens)++;
        return node;
    }
    else if ((*tokens)->type == TYPE_NUM)
    {
        printf ("\033[31m erroneous entry15 \033[0m \n");
        node_t * node = createNodeWithNum ((*tokens)->u1.value);
        (*tokens)++;
        return node;
    }
    else if ((*tokens)->type == TYPE_ID)
    {
        printf ("in getPrimary TYPE_ID\n");
        node_t * node = createNodeWithVariable ((*tokens)->u1.id);
        (*tokens)++;

        // printf ("in getPrimary TYPE_ID\n");
        // node_t * node = nullptr;
        // if (((*tokens)+1)->type != TYPE_O_BRACKET)
        // {
        //     node = createNodeWithVariable ((*tokens)->u1.id);
        //     (*tokens)++;
        // }

        return node;
    }
    else
    {
        printf ("\033[31m erroneous entry17 \033[0m \n");
        MY_ASSERT (1, "Wrong command");
        return nullptr;
    }
}

static node_t * getId (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "There access to tokens");

    node_t * node = createNodeWithVariable ((*tokens)->u1.id);
    (*tokens)++;

    return node;
}

static node_t ** increaseMemory (node_t ** ptr, size_t * size)
{
    *size *= 2;
    ptr = (node_t **) realloc (ptr, (*size) * sizeof(node_t *));
    MY_ASSERT (ptr == nullptr, "Unable to allocate new memory");

    return ptr;
}

static void * allocateMemory (size_t numElem, size_t sizeElem)
{
    void * ptr = calloc (numElem, sizeElem);
    MY_ASSERT (ptr == nullptr, "Unable to allocate new memory");
    return ptr;
}

static void checkSemicolon (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    printf ("(*tokens)->type = %d, (*tokens - 1)->type = %d\n", (*tokens)->type, (*tokens - 1)->type);

    if ((*tokens)->type != TYPE_SEMICOLON)
    {
        MY_ASSERT (1, "The expression must end with a semicolon");
    }
    (*tokens)++; //to skip a ";"
}

static node_t * createParams (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    node_t * identifier = getPrimary (tokens);
    node_t * node = createNodeInstruction (PARAM_T, nullptr, identifier);
    return node;
}