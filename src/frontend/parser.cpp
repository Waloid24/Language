#include "parser.h"

#define DEF_CMD(nameCmd, value, ...)\
    const int nameCmd = value;

#include "../common/cmd.h"

#undef DEF_CMD

#define checkSemicolon(ptrTok, line, file)      \
        checkSemic (ptrTok, line, file)

size_t STANDART_NUM_OF_INSTRUCTION = 5;

static node_t * getDefine (token_t ** tokens);
static node_t * getStatement (token_t ** tokens, bool isMain);
static node_t * getAssign (token_t ** tokens);
static node_t * getTerminational (token_t ** tokens, bool isMain);
static node_t * getCycle (token_t ** tokens, bool isMain);
static node_t * getConditional (token_t ** tokens, bool isMain);
static node_t * getExpression (token_t ** tokens);
static node_t * getBoolean (token_t ** tokens);
static node_t * getArithmetic (token_t ** tokens);
static node_t * getTerm (token_t ** tokens);
static node_t * getPrimary (token_t ** tokens);

static void checkSemic (token_t ** tokens, size_t line, const char * nameFile);
static node_t ** increaseMemory (node_t ** ptr, size_t * size);
static node_t * createParams (token_t ** tokens);
static node_t * getId (token_t ** tokens);
static node_t * getFunc (token_t ** tokens);

node_t * getGeneral (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    node_t ** multipleInstructions = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));

    if (((*tokens)+1)->type != TYPE_O_BRACKET)
    {
        node_t * node = getAssign (tokens);
        multipleInstructions[0] = createKeyNode (STATEMENT_T, nullptr, node, "STATEMENT");
    }
    else if (((*tokens)+1)->type == TYPE_O_BRACKET)
    {
        multipleInstructions[0] = getDefine (tokens);
    }
    else
    {
        MY_ASSERT (1, "Wrong command");
        return nullptr;
    }

    size_t i = 0;
    for (; (*tokens)->type != TYPE_END_TOKENS; i++)
    {
        if (i == STANDART_NUM_OF_INSTRUCTION-1)
        {
            multipleInstructions = increaseMemory (multipleInstructions, &STANDART_NUM_OF_INSTRUCTION);
        }
        node_t * node = nullptr;
        if (((*tokens)+1)->type != TYPE_O_BRACKET)
        {
            node_t * supportNode = getAssign (tokens);
            node = createKeyNode (STATEMENT_T, nullptr, supportNode, "STATEMENT");
        }
        else if (((*tokens)+1)->type == TYPE_O_BRACKET)
        {
            node = getDefine (tokens);
        }
        else
        {
            MY_ASSERT (1, "Wrong command");
            return nullptr;
        }
        multipleInstructions[i]->parent = node;
        node->left = multipleInstructions[i];
        multipleInstructions[i+1] = node;
    }

    node_t * finalNode = copyNode(multipleInstructions[i]);

    deleteTree (multipleInstructions[i], false);
    free (multipleInstructions);

    return finalNode;
}

static node_t * getDefine (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    bool isMain = false;

    node_t * funcName = getFunc (tokens);
    if (funcName->key_t == MAIN_T)
    {
        isMain = true;
    }

    MY_ASSERT ((*tokens)->type != TYPE_O_BRACKET, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    (*tokens)++;

    node_t ** multipleParams = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));

    size_t i = 0;
    if ((*tokens)->type != TYPE_C_BRACKET) 
    {
        multipleParams[0] = createParams (tokens);

        for ( ; (*tokens)->type != TYPE_C_BRACKET; i++)
        {
            if (i == STANDART_NUM_OF_INSTRUCTION-1)
            {
                multipleParams = increaseMemory (multipleParams, &STANDART_NUM_OF_INSTRUCTION);
            }
            node_t * node = createParams (tokens);
            multipleParams[i]->parent = node;
            node->left = multipleParams[i];
            multipleParams[i+1] = node;
        }
        (*tokens)++;
    }
    else
    {
        (*tokens)++;
    }

    if ((*tokens)->type != TYPE_O_F_BRACKET)
    {
        MY_ASSERT (1, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    }
    (*tokens)++;

    node_t ** multipleInstructions = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));
    multipleInstructions[0] = getStatement (tokens, isMain);
    size_t j = 0;
    for (; (*tokens)->type != TYPE_C_F_BRACKET; j++)
    {
        if (j == STANDART_NUM_OF_INSTRUCTION-1)
        {
            multipleInstructions = increaseMemory (multipleInstructions, &STANDART_NUM_OF_INSTRUCTION);
        }
        node_t * node = getStatement (tokens, isMain);
        multipleInstructions[j]->parent = node;
        node->left = multipleInstructions[j];
        multipleInstructions[j+1] = node;
    }
    (*tokens)++;

    node_t * funcSignature  = createKeyNode (FUNC_T, funcName, multipleParams[i], "FUNC");
    node_t * headNode       = createKeyNode (DEFINE_T, funcSignature, multipleInstructions[j], "DEFINE");
    node_t * supportNode    = createKeyNode (STATEMENT_T, nullptr, headNode, "STATEMENT");

    free (multipleParams);
    free (multipleInstructions);

    return supportNode;
}

static node_t * getStatement (token_t ** tokens, bool isMain)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    node_t * node = nullptr;
    if ((*tokens)->type == TYPE_IF)
    {
        node = getConditional (tokens, isMain);
    }
    else if ((*tokens)->type == TYPE_WHILE)
    {
        node = getCycle (tokens, isMain);
    }
    else if ((*tokens + 1)->type == TYPE_ASSIGN)
    {
        node = getAssign (tokens);
    }
    else if ((*tokens)->type == TYPE_RETURN)
    {
        node = getTerminational (tokens, isMain);
    }
    else
    {
        node = getExpression (tokens);
        checkSemicolon (tokens, __LINE__, __FILE__);
    }

    node_t * headNode = createKeyNode (STATEMENT_T, nullptr, node, "STATEMENT");

    return headNode;
}

static node_t * getAssign (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    node_t * node = getPrimary (tokens);

    (*tokens)++;
    MY_ASSERT (node->id_t != ID_VAR, "It is necessary to have a variable before the assignment sign");
    
    node_t * node_1 = getExpression (tokens);
    node_t * supportNode = createKeyNode (INITIALIZER_T, nullptr, node_1, "INITIALIZER");
    node_t * headNode = createNodeWithOperation (OP_ASSIGN, node, supportNode, "AS");

    checkSemicolon (tokens, __LINE__, __FILE__);
    return headNode;
}

static node_t * getTerminational (token_t ** tokens, bool isMain)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    (*tokens)++; //skip 'return'

    node_t * node = getExpression (tokens);
    checkSemicolon (tokens, __LINE__, __FILE__);

    if (isMain == true)
    {
        deleteTree (node, false);
        return createKeyNode (HLT_T, nullptr, nullptr, "HLT");
    }
    else
    {
        return createKeyNode (RETURN_T, nullptr, node, "RETURN");
    }
}

static node_t * getCycle (token_t ** tokens, bool isMain)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");
    (*tokens)++;

    MY_ASSERT ((*tokens)->type != TYPE_O_BRACKET, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    (*tokens)++;

    node_t * condition = getExpression (tokens);

    MY_ASSERT ((*tokens)->type != TYPE_C_BRACKET, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    (*tokens)++;

    MY_ASSERT ((*tokens)->type != TYPE_O_F_BRACKET, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    (*tokens)++;

    node_t ** multipleInstructions = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));
    multipleInstructions[0] = getStatement (tokens, isMain);

    size_t i = 0;
    for (; (*tokens)->type != TYPE_C_F_BRACKET; i++)
    {
        if (i == STANDART_NUM_OF_INSTRUCTION-1)
        {
            multipleInstructions = increaseMemory (multipleInstructions, &STANDART_NUM_OF_INSTRUCTION);
        }
        node_t * node = getStatement (tokens, isMain);
        multipleInstructions[i]->parent = node;
        node->left = multipleInstructions[i];
        multipleInstructions[i+1] = node;
    }
    (*tokens)++;

    node_t * headNode = (node_t *) allocateMemory (1, sizeof(node_t));

	headNode->key_t = WHILE_T;
    headNode->supportName = "WHILE";
    headNode->left = condition;
    condition->parent = headNode;
    headNode->right = multipleInstructions[i];
    multipleInstructions[i]->parent = headNode;

    free (multipleInstructions);

    return headNode;
}

static node_t * getConditional (token_t ** tokens, bool isMain)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");
    (*tokens)++;

    MY_ASSERT ((*tokens)->type != TYPE_O_BRACKET, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    (*tokens)++;

    node_t * condition = getExpression (tokens);

    MY_ASSERT ((*tokens)->type != TYPE_C_BRACKET, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    (*tokens)++;

    MY_ASSERT ((*tokens)->type != TYPE_O_F_BRACKET, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
    (*tokens)++;

    node_t ** multipleInstructions = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));
    multipleInstructions[0] = getStatement (tokens, isMain);

    size_t i = 0;
    for (; (*tokens)->type != TYPE_C_F_BRACKET; i++)
    {
        if (i == STANDART_NUM_OF_INSTRUCTION-1)
        {
            multipleInstructions = increaseMemory (multipleInstructions, &STANDART_NUM_OF_INSTRUCTION);
        }
        node_t * node = getStatement (tokens, isMain);
        multipleInstructions[i]->parent = node;
        node->left = multipleInstructions[i];
        multipleInstructions[i+1] = node;
    }
    (*tokens)++;

    node_t ** multipleInstructionsElse = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));
    size_t j = 0;
    if ((*tokens)->type == TYPE_ELSE)
    {
        (*tokens)++;
        MY_ASSERT ((*tokens)->type != TYPE_O_F_BRACKET, "\033[1;31m The condition in if must always be surrounded by parentheses \033[0m");
        (*tokens)++;        

        multipleInstructionsElse[0] = getStatement (tokens, isMain);

        for (; (*tokens)->type != TYPE_C_F_BRACKET; j++)
        {
            if (j == STANDART_NUM_OF_INSTRUCTION-1)
            {
                multipleInstructionsElse = increaseMemory (multipleInstructionsElse, &STANDART_NUM_OF_INSTRUCTION);
            }
            node_t * node = getStatement (tokens, isMain);
            multipleInstructionsElse[j]->parent = node;
            node->left = multipleInstructionsElse[j];
            multipleInstructionsElse[j+1] = node;
        }
        (*tokens)++;
    }

    node_t * headNode = (node_t *) calloc (1, sizeof(node_t));
	MY_ASSERT (headNode == nullptr, "Unable to allocate new memory");

	headNode->key_t = IF_T;
    headNode->supportName = "IF";

    node_t * actions = createKeyNode (DECISION_T, multipleInstructions[i], multipleInstructionsElse[j], "DECISION"); 
    headNode->left = condition;
    condition->parent = headNode;
    
    headNode->right = actions;
    actions->parent = headNode;

    free (multipleInstructions);
    free (multipleInstructionsElse);

    return headNode;
}

static node_t * getExpression (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "There access to tokens");

    node_t * node_1 = getBoolean (tokens);

    if ((*tokens)->type == TYPE_LOG_AND)
    {
        (*tokens)++;
        node_t * node_2 = getBoolean (tokens);
        node_t * headNode = createNodeWithOperation (OP_LOG_AND, node_1, node_2, "AND");

        return headNode;
    }
    else if ((*tokens)->type == TYPE_LOG_OR)
    {
        (*tokens)++;
        node_t * node_2 = getBoolean (tokens);
        node_t * headNode = createNodeWithOperation (OP_LOG_OR, node_1, node_2, "OR");
        
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

    if ((*tokens)->type == TYPE_GR_OR_EQ || (*tokens)->type == TYPE_LESS_OR_EQ || 
        (*tokens)->type == TYPE_LESS || (*tokens)->type == TYPE_GR ||
        (*tokens)->type == TYPE_IDENTITY || (*tokens)->type == TYPE_NOT_EQUAL)
    {
        int nodeType = (*tokens)->type;
        (*tokens)++;

        node_t * node_2 = getArithmetic (tokens);
        node_t * headNode = nullptr;

        #define BOOL_CMDS(cmd)                                              \
            if (nodeType == TYPE_##cmd)                                     \
            {                                                               \
                headNode = createNodeWithOperation (OP_##cmd, node_1, node_2, #cmd);  \
            }                                                               \
            else

        #include "boolCmds.h"
        {
            return nullptr;
        }
        return headNode;
    }
    return node_1;
}

static node_t * getArithmetic (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "There access to tokens");    

    node_t * node = getTerm (tokens);
    if ((*tokens)->type == TYPE_ADD || (*tokens)->type == TYPE_SUB)
    {
        node_t ** multipleInstructions = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));
        multipleInstructions[0] = node;
        size_t i = 0;
        for (; (*tokens)->type == TYPE_ADD || (*tokens)->type == TYPE_SUB; i++)
        {
            int operation = (*tokens)->type;
            (*tokens)++;
            if (i == STANDART_NUM_OF_INSTRUCTION-1)
            {
                multipleInstructions = increaseMemory (multipleInstructions, &STANDART_NUM_OF_INSTRUCTION);
            }

            node_t * node_1 = getTerm (tokens);
            node_t * node_2 = nullptr;

            if (operation == TYPE_ADD)
            {
                node_2 = createNodeWithOperation (OP_ADD, multipleInstructions[i], node_1, "ADD");
            }
            else
            {
                node_2 = createNodeWithOperation (OP_SUB, multipleInstructions[i], node_1, "SUB");

            }
            multipleInstructions[i+1] = node_2;
        }

        node_t * finalNode = copyNode (multipleInstructions[i]);

        deleteTree (multipleInstructions[i], false);
        free (multipleInstructions);

        return finalNode;
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

    if (((*tokens)->type == TYPE_MUL) || ((*tokens)->type == TYPE_DIV))
    {        
        node_t ** multipleInstructions = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));
        multipleInstructions[0] = node;
        size_t i = 0;
        for (; ((*tokens)->type == TYPE_MUL) || ((*tokens)->type == TYPE_DIV); i++)
        {
            int operation = (*tokens)->type;
            (*tokens)++;
            if (i == STANDART_NUM_OF_INSTRUCTION-1)
            {
                multipleInstructions = increaseMemory (multipleInstructions, &STANDART_NUM_OF_INSTRUCTION);
            }

            node_t * node_1 = getPrimary (tokens);
            node_t * node_2 = nullptr;
            
            if (operation == TYPE_MUL)
            {
                node_2 = createNodeWithOperation (OP_MUL, multipleInstructions[i], node_1, "MUL");
            }
            else
            {
                node_2 = createNodeWithOperation (OP_DIV, multipleInstructions[i], node_1, "DIV");

            }
            multipleInstructions[i+1] = node_2;
        }

        node_t * finalNode = copyNode (multipleInstructions[i]);

        deleteTree (multipleInstructions[i], false);
        free (multipleInstructions);

        return finalNode;
    }
    else
    {
        return node;
    }
}

static node_t * getPrimary (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "There access to tokens");
    if (((*tokens)->type == TYPE_ID) && (strcmp ((*tokens)->u1.id, "-") == 0))
    {
        (*tokens)++;
        node_t * node_1 = createNodeWithNum (-1000);
        node_t * node_2 = getPrimary (tokens);
        node_t * headNode = createNodeWithOperation (OP_MUL, node_1, node_2, "MUL");
        return headNode;
    }
    else if ((*tokens)->type == TYPE_O_BRACKET)
    {
        (*tokens)++;
        node_t * node = getExpression (tokens);
        MY_ASSERT ((*tokens)->type != TYPE_C_BRACKET, "You forgot to close the bracket");
        (*tokens)++;
        return node;
    }
    else if ((*tokens)->type == TYPE_NUM)
    {
        node_t * node = createNodeWithNum ((*tokens)->u1.value);
        (*tokens)++;
        return node;
    }
    else if ((((*tokens)->type == TYPE_ID) || ((*tokens)->type == TYPE_SIN) ||
            ((*tokens)->type == TYPE_COS) || ((*tokens)->type == TYPE_LN) ||
            ((*tokens)->type == TYPE_PRINT) || ((*tokens)->type == TYPE_SCAN) || 
            ((*tokens)->type == TYPE_SQRT)) && ((*tokens + 1)->type == TYPE_O_BRACKET))
    {
        node_t * funcName = getFunc (tokens);
        MY_ASSERT ((*tokens)->type != TYPE_O_BRACKET, "The condition in if must always be surrounded by parentheses");
        (*tokens)++;
        node_t ** multipleInstructions = (node_t **) allocateMemory (STANDART_NUM_OF_INSTRUCTION, sizeof(node_t *));
        node_t * expr_1 = getExpression (tokens);
        
        multipleInstructions[0] = createKeyNode (PARAM_T, nullptr, expr_1, "PARAM");

        size_t i = 0;
        for (; (*tokens)->type != TYPE_C_BRACKET; i++)
        {
            if (i == STANDART_NUM_OF_INSTRUCTION-1)
            {
                multipleInstructions = increaseMemory (multipleInstructions, &STANDART_NUM_OF_INSTRUCTION);
            }
            node_t * expr_2 = getExpression (tokens);
            node_t * node = createKeyNode (PARAM_T, nullptr, expr_2, "PARAM");
            multipleInstructions[i]->parent = node;
            node->left = multipleInstructions[i];
            multipleInstructions[i+1] = node;
        }
        (*tokens)++;
        
        node_t * headNode = createKeyNode (CALL_T, funcName, multipleInstructions[i], "CALL");

        free (multipleInstructions);
        return headNode;
    }
    else if ((*tokens)->type == TYPE_ID)
    {
        return getId(tokens);
    }
    else
    {
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

static node_t * getFunc (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "There access to tokens");

    node_t * node = createNodeWithFunction ((*tokens)->u1.id);
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

static void checkSemic (token_t ** tokens, size_t line, const char * nameFile)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");
    MY_ASSERT (nameFile == nullptr, "There is no access to the file");

    if ((*tokens)->type != TYPE_SEMICOLON)
    {
        fprintf (stderr, "Semicolon omitted\n");
        fprintf (stderr, "file = %s, line: %zu\n", nameFile, line);
        MY_ASSERT (1, "n");
    }
    (*tokens)++;
}

static node_t * createParams (token_t ** tokens)
{
    MY_ASSERT (tokens == nullptr, "No access to tokens");

    node_t * identifier = getId (tokens);
    node_t * node = createKeyNode (PARAM_T, nullptr, identifier, "PARAM");
    
    return node;
}