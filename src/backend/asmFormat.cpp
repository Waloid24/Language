#include "asmFormat.h"

static void filledFile (node_t * node, FILE * asmFile, var_t ** tableGlobVars);
static void processDefineParams (node_t * node, FILE * asmFile, var_t ** tableGlobVars, var_t ** tableLocalVars, size_t * numVars);
static void processFuncBody (node_t * node, FILE * asmFile, var_t ** tableGlobVars, var_t ** tableLocalVars, size_t * numVars);
static void processExpr (node_t * node, FILE * asmFile, var_t ** tableGlobVars, var_t ** tableLocalVars, size_t * numVars, bool toStack);
static void processAssign (node_t * node, FILE * asmFile, var_t ** tableGlobVars, var_t ** tableLocalVars, size_t * numVars);

#define printAsm(text, ...)         \
        fprintf (asmFile, text, ##__VA_ARGS__)

size_t NUM_WHILE        = 0;
size_t NUM_ELSE         = 0;
size_t NUM_GLOB_VARS    = 0;
size_t NUM_IF           = 0;

void asmFormat (node_t * node, char * nameFile)
{
    MY_ASSERT (node == nullptr, "There is no access to this node");
    MY_ASSERT (nameFile == nullptr, "There is no access to the file name");
    FILE * asmFile = openFile (nameFile, "w");

    var_t * tableGlobVars = (var_t *) allocateMemory (MAX_NUM_GLOB_VARS, sizeof (var_t));

    printAsm ("push %zu;\n", PLACE_GLOB_VARS);
    printAsm ("pop rax;\n");
    printAsm ("push %zu;\n", MAX_NUM_GLOB_VARS);
    printAsm ("pop rbx;\n");

    filledFile (node, asmFile, &tableGlobVars);
    
    free (tableGlobVars);
    fclose (asmFile);
}

static void filledFile (node_t * node, FILE * asmFile, var_t ** tableGlobVars)
{
    MY_ASSERT (node == nullptr, "There is no access to this node");
    MY_ASSERT (asmFile == nullptr, "There is no access to the file");

    if (node->key_t == DEFINE_T)
    {
        var_t * tableLocalVars = (var_t *) allocateMemory (MAX_NUM_VARS, sizeof (var_t));

        size_t numVars = 0;

        if (node->left->left->key_t != MAIN_T)
        {
            printAsm ("jmp end_%s:;\n", node->left->left->name);
            printAsm ("%s:;\n", node->left->left->name);
        }

        processDefineParams (node->left->right, asmFile, tableGlobVars, &tableLocalVars, &numVars);
        processFuncBody (node->right, asmFile, tableGlobVars, &tableLocalVars, &numVars);

        if (node->left->left->key_t != MAIN_T)
        {
            printAsm ("end_%s:;\n", node->left->left->name);
        }

        free (tableLocalVars);

        return ;
    }
    if (node->op_t == OP_ASSIGN)
    {
        bool isVarInTable = false;

        if (NUM_GLOB_VARS == MAX_NUM_GLOB_VARS-1)
        {
            MY_ASSERT (1, "The limit on the number of global variables has been reached. Buy more RAM");
        }
        
        processExpr (node->right->right, asmFile, tableGlobVars, nullptr, nullptr, true);
        
        for (size_t i = 0; i < MAX_NUM_GLOB_VARS; i++)
        {
            if ((*tableGlobVars)[i].name != nullptr && strcmp (node->left->name, (*tableGlobVars)[i].name) == 0)
            {
                printAsm ("pop [rax + %zu];\n", (*tableGlobVars)[i].num);
                isVarInTable = true;
                break;
            }
        }
        if (isVarInTable == false)
        {
            printAsm ("pop [rax + %zu];\n", NUM_GLOB_VARS);
            (*tableGlobVars)[NUM_GLOB_VARS].name = node->left->name;
            (*tableGlobVars)[NUM_GLOB_VARS].num = NUM_GLOB_VARS;
            NUM_GLOB_VARS++;
        }
        return ;
    }

    if (node->left != nullptr)
    {
        filledFile (node->left, asmFile, tableGlobVars);
    }
    if (node->right != nullptr)
    {
        filledFile (node->right, asmFile, tableGlobVars);
    }
    
}

static void processDefineParams (node_t * node, FILE * asmFile, var_t ** tableGlobVars, var_t ** tableLocalVars, size_t * numVars)
{
    MY_ASSERT (asmFile == nullptr, "No access to the file");

    if (node == nullptr)
    {
        return ;
    }

    if (node->left != nullptr)
    {
        processDefineParams (node->left, asmFile, tableGlobVars, tableLocalVars, numVars);
    }
    if (node->right != nullptr)
    {
        processDefineParams (node->right, asmFile, tableGlobVars, tableLocalVars, numVars);
    }

    if (node->id_t == ID_VAR)
    {
        bool isVarInTable = false;
        if (*numVars == MAX_NUM_VARS-1)
        {
            MY_ASSERT (1, "The limit on the number of variables has been reached. Buy more RAM");
        }
        for (size_t i = 0; i < *numVars; i++)
        {
            if (strcmp (node->name, (*tableLocalVars)[i].name) == 0)
            {
                printAsm ("pop [rbx + %zu];\n", (*tableLocalVars)[i].num);
                isVarInTable = true;
                break;
            }
        }
        if (isVarInTable == false)
        {
            for (size_t i = 0; i < NUM_GLOB_VARS; i++)
            {
                if (strcmp (node->name, (*tableGlobVars)[i].name) == 0)
                {
                    printAsm ("pop [rax + %zu];\n", (*tableGlobVars)[i].num);
                    isVarInTable = true;
                    break;
                }
            }
        }
        if (isVarInTable == false) 
        {
            printAsm ("pop [rbx + %zu];\n", *numVars);
            (*tableLocalVars)[*numVars].name = node->name;
            (*tableLocalVars)[*numVars].num = *numVars;
            (*numVars)++;
        }
    }
}

static void processFuncBody (node_t * node, FILE * asmFile, var_t ** tableGlobVars, var_t ** tableLocalVars, size_t * numVars)
{
    MY_ASSERT (asmFile == nullptr, "No access to the file");
    MY_ASSERT (tableGlobVars == nullptr, "No access to the variable table");
    MY_ASSERT (tableLocalVars == nullptr, "No access to the variable table");
    MY_ASSERT (numVars == nullptr, "There is no counter for the number of variables");

    if (node == nullptr)
    {
        return ;
    }
    
    if (node->op_t == OP_ASSIGN)
    {
        processAssign (node, asmFile, tableGlobVars, tableLocalVars, numVars);
        return ; 
    }

    if (node->key_t == HLT_T)
    {
        printAsm ("hlt;\n");
    }

    if (node->key_t == RETURN_T)
    {
        processExpr (node->right, asmFile, tableGlobVars, tableLocalVars, numVars, true);
        printAsm ("ret;\n");
        return ; 
    }

    if (node->key_t == IF_T)
    {
        processExpr (node->left, asmFile, tableGlobVars, tableLocalVars, numVars, true);
        printAsm ("push 0;\n");
        size_t tmpElse = NUM_ELSE;
        if (node->right->right != nullptr)
        {
            printAsm ("je else%zu:;\n", NUM_ELSE);
            NUM_ELSE++;
        }
        if (node->right->right == nullptr)
        {
            printAsm ("je afterIf%zu:;\n", NUM_IF);
        }
        size_t tmpIf = NUM_IF;
        NUM_IF++;
        processFuncBody (node->right->left, asmFile, tableGlobVars, tableLocalVars, numVars);

        if (node->right->right != nullptr)
        {
            printAsm ("jmp elseExit%zu:;\n", tmpElse);
            printAsm ("else%zu:;\n", tmpElse);
            processFuncBody (node->right->right, asmFile, tableGlobVars, tableLocalVars, numVars);
            printAsm ("elseExit%zu:;\n", tmpElse);
        }
        if (node->right->right == nullptr)
        {
            printAsm ("afterIf%zu:;\n", tmpIf);
        }
        return ;
    }

    if (node->key_t == WHILE_T)
    {
        printAsm ("while%zu:;\n", NUM_WHILE);
        size_t tmp = NUM_WHILE;
        NUM_WHILE++;
        processExpr (node->left, asmFile, tableGlobVars, tableLocalVars, numVars, true);
        printAsm ("push 0;\n");
        printAsm ("je endWhile%zu:;\n", tmp);
        processFuncBody (node->right, asmFile, tableGlobVars, tableLocalVars, numVars);
        processExpr (node->left, asmFile, tableGlobVars, tableLocalVars, numVars, true);
        printAsm ("push 0;\n");
        printAsm ("jne while%zu:;\n", tmp);
        printAsm ("endWhile%zu:;\n", tmp);
        return ;
    }

    #define CALLS_BASE_FUNC(nameFunc, type)                                                                 \
        if (node->left->b_func_t == type)                                                                   \
        {                                                                                                   \
            MY_ASSERT (node->right == nullptr, "This function must have a parameter");                      \
            processExpr (node->right, asmFile, tableGlobVars, tableLocalVars, numVars, true);               \
            printAsm ("%s;\n", #nameFunc);                                                                  \
        }                                                                                                   \
        else

    if (node->key_t == CALL_T)
    {
        if (node->left->b_func_t == PRINT_T)
        {
            processExpr (node->right, asmFile, tableGlobVars, tableLocalVars, numVars, true);
            printAsm ("out;\n");
        }
        else if (node->left->b_func_t == SCAN_T)
        {
            if (node->right != nullptr)
            {
                printAsm ("in;\n");
                processExpr (node->right, asmFile, tableGlobVars, tableLocalVars, numVars, false);
            }
            else
            {
                printAsm ("in;\n");
            }
        }
        else 


        #include "callBaseFunc.h"


        {
            processExpr (node->right, asmFile, tableGlobVars, tableLocalVars, numVars, true);
            printAsm ("call %s:;\n", node->left->name);
        }
        return ;
    }

    if (node->left != nullptr)
    {
        processFuncBody (node->left, asmFile, tableGlobVars, tableLocalVars, numVars);
    }

    if (node->right != nullptr)
    {
        processFuncBody (node->right, asmFile, tableGlobVars, tableLocalVars, numVars);
    }

}

static void processExpr (node_t * node, FILE * asmFile, var_t ** tableGlobVars, var_t ** tableLocalVars, size_t * numVars, bool toStack)
{
    MY_ASSERT (asmFile == nullptr, "No access to the file");
    MY_ASSERT (tableGlobVars == nullptr, "No access to the variable table");

    if (node->key_t == CALL_T)
    {


        #include "callBaseFunc.h"


        {
            processExpr (node->right, asmFile, tableGlobVars, tableLocalVars, numVars, toStack);
            printAsm ("call %s:;\n", node->left->name);
        }
        #undef CALLS_BASE_FUNC
        return ;
    }

    if (node->left != nullptr)
    {
        processExpr (node->left, asmFile, tableGlobVars, tableLocalVars, numVars, toStack);
    }

    if (node->right != nullptr)
    {
        processExpr (node->right, asmFile, tableGlobVars, tableLocalVars, numVars, toStack);
    }

    if (node->isNum == true)
    {
        printAsm ("push %.0lf;\n", node->elem);
    }

    if (node->id_t == ID_VAR)
    {
        bool isVarInTable = false;
        for (size_t i = 0; i < *numVars; i++)
        {
            if (strcmp (node->name, (*tableLocalVars)[i].name) == 0)
            {
                if (toStack == true)
                {
                    printAsm ("push [rbx + %zu];\n", (*tableLocalVars)[i].num);
                }
                else
                {
                    printAsm ("pop [rbx + %zu];\n", (*tableLocalVars)[i].num);
                }
                isVarInTable = true;
                break;
            }
        }
        if (isVarInTable == false)
        {
            for (size_t i = 0; i < NUM_GLOB_VARS; i++)
            {
                if (strcmp (node->name, (*tableGlobVars)[i].name) == 0)
                {
                    if (toStack == true)
                    {
                        printAsm ("push [rax + %zu];\n", (*tableGlobVars)[i].num);
                    }
                    else
                    {
                        printAsm ("pop [rax + %zu];\n", (*tableGlobVars)[i].num);
                    }
                    isVarInTable = true;
                    break;
                }
            }
        }
        if (isVarInTable == false)
        {
            MY_ASSERT (1, "This variable is not in the table");
        }
    }

    #define OPERS(oper, cmdAsm)             \
        if (node->op_t == oper)             \
        {                                   \
            printAsm ("%s;\n", #cmdAsm);    \
        }               
    
    #include "GEN_OPER_ASM.h"

    #undef OPERS

}

static void processAssign (node_t * node, FILE * asmFile, var_t ** tableGlobVars, var_t ** tableLocalVars, size_t * numVars)
{
    bool isVarInTable = false;

    if (*numVars == MAX_NUM_VARS-1)
    {
        MY_ASSERT (1, "The limit on the number of variables has been reached. Buy more RAM");
    }
    
    processExpr (node->right->right, asmFile, tableGlobVars, tableLocalVars, numVars, true);
    
    for (size_t i = 0; i < *numVars; i++)
    {
        if (strcmp (node->left->name, (*tableLocalVars)[i].name) == 0)
        {
            printAsm ("pop [rbx + %zu];\n", (*tableLocalVars)[i].num);
            isVarInTable = true;
            break;
        }
    }
    if (isVarInTable == false)
    {
        for (size_t i = 0; i < NUM_GLOB_VARS; i++)
        {
            if (strcmp (node->left->name, (*tableGlobVars)[i].name) == 0)
            {
                printAsm ("pop [rax + %zu];\n", (*tableGlobVars)[i].num);
                isVarInTable = true;
                break;
            }
        }
    }
    if (isVarInTable == false)
    {
        printAsm ("pop [rbx + %zu];\n", *numVars);
        (*tableLocalVars)[*numVars].name = node->left->name;
        (*tableLocalVars)[*numVars].num = *numVars;
        (*numVars)++;
    }
}
