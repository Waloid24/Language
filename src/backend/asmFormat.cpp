#include "asmFormat.h"

static FILE * createFile (char * nameFile);
static void filledFile (node_t * node, FILE * asmFile, var_t ** tableVars);
static void processDefineParams (node_t * node, FILE * asmFile, var_t ** tableVars);
static void processFuncBody (node_t * node, FILE * asmFile, var_t ** tableVars);
static void processExpr (node_t * node, FILE * asmFile, var_t ** tableVars);
static void processCallParams (node_t * node, FILE * asmFile, var_t ** tableVars, bool toStack);
static void processAssign (node_t * node, FILE * asmFile, var_t ** tableVars);

#define printAsm(text, ...)         \
        fprintf (asmFile, text, ##__VA_ARGS__)

size_t NUM_VAR = 0;
size_t STANDART_NUM_VAR = 50;
size_t NUM_WHILE = 0;
size_t NUM_ELSE = 0;

void asmFormat (node_t * node, char * nameFile)
{
    MY_ASSERT (node == nullptr, "There is no access to this node");
    MY_ASSERT (nameFile == nullptr, "There is no access to the file name")
    printf ("nameFile = %s\n", nameFile);
    FILE * asmFile = createFile (nameFile);

    var_t * tableVars = (var_t *) calloc (STANDART_NUM_VAR, sizeof (var_t));
    MY_ASSERT (tableVars == nullptr, "Unable to allocate new memory");

    filledFile (node, asmFile, &tableVars);
    
    free (tableVars);
    fclose (asmFile);
}

static void filledFile (node_t * node, FILE * asmFile, var_t ** tableVars)
{
    MY_ASSERT (node == nullptr, "There is no access to this node");
    MY_ASSERT (asmFile == nullptr, "There is no access to the file");

    if (node->key_t == DEFINE_T)
    {
        if (node->left->left->key_t == MAIN_T)
        {
            processDefineParams (node->left->right, asmFile, tableVars);
            processFuncBody (node->right, asmFile, tableVars);
        }
        else
        {
            printAsm ("jmp end_%s:;\n", node->left->left->name);
            printAsm ("%s:;\n", node->left->left->name);
            processDefineParams (node->left->right, asmFile, tableVars);
            processFuncBody (node->right, asmFile, tableVars);
        }
    }
    if (node->op_t == OP_ASSIGN)
    {
        processAssign (node, asmFile, tableVars);
        return ;
    }

    if (node->left != nullptr)
    {
        filledFile (node->left, asmFile, tableVars);
    }
    if (node->right != nullptr)
    {
        filledFile (node->right, asmFile, tableVars);
    }
    if (node->key_t == DEFINE_T && node->left->left->key_t != MAIN_T)
    printAsm ("end_%s:;\n", node->left->left->name);
}

static FILE * createFile (char * nameFile)
{
    MY_ASSERT (nameFile == nullptr, "Wrong pointer to name file with source code");

    FILE *file = fopen (nameFile, "w");
    MY_ASSERT (file == nullptr, "Unable to open the source file");

    setbuf (file, NULL);

    return file;
}

static void processDefineParams (node_t * node, FILE * asmFile, var_t ** tableVars)
{
    MY_ASSERT (asmFile == nullptr, "No access to the file");

    if (node == nullptr) //The case when the function has no parameters
    {
        printf ("No parameters in functions\n");
        return ;
    }

    if (node->left != nullptr)
    {
        processDefineParams (node->left, asmFile, tableVars);
    }
    if (node->right != nullptr)
    {
        processDefineParams (node->right, asmFile, tableVars);
    }

    if (node->id_t == ID_VAR)
    {
        bool isVarInTable = false;
        if (NUM_VAR == STANDART_NUM_VAR-1)
        {
            STANDART_NUM_VAR *= 2;
            *tableVars = (var_t *) realloc (*tableVars, STANDART_NUM_VAR * sizeof(var_t));
        }
        for (size_t i = 0; i < NUM_VAR; i++)
        {
            if (strcmp (node->name, (*tableVars)[i].name) == 0)
            {
                printAsm ("pop[%zu];\n", (*tableVars)[i].num);
                isVarInTable = true;
                break;
            }
        }
        if (isVarInTable == false) 
        {
            printAsm ("pop[%zu];\n", NUM_VAR);
            (*tableVars)[NUM_VAR].name = node->name;
            (*tableVars)[NUM_VAR].num = NUM_VAR;
            NUM_VAR++;
        }
    }
}

static void processFuncBody (node_t * node, FILE * asmFile, var_t ** tableVars)
{
    MY_ASSERT (asmFile == nullptr, "No access to the file");
    MY_ASSERT (tableVars == nullptr, "No access to the variable table");
    if (node == nullptr) //The case when the function has no parameters
    {
        return ;
    }

    if (node->op_t == OP_ASSIGN)
    {
        processAssign (node, asmFile, tableVars);
        return ; 
    }

    if (node->key_t == RETURN_T)
    {
        processExpr (node->right, asmFile, tableVars);
        return ; 
    }

    if (node->key_t == IF_T)
    {
        processExpr (node->left, asmFile, tableVars);
        printAsm ("push 0;\n");
        size_t tmp = NUM_ELSE;
        if (node->right->right != nullptr)
        {
            printAsm ("je else%zu:;\n", NUM_ELSE);
            NUM_ELSE++;
        }
        processFuncBody (node->right->left, asmFile, tableVars);
        printAsm ("jmp elseExit%zu:;\n", tmp);
        if (node->right->right != nullptr)
        {
            printAsm ("else%zu:;\n", tmp);
            processFuncBody (node->right->right, asmFile, tableVars);
            printAsm ("elseExit%zu:;\n", tmp);
        }
        return ;
    }

    if (node->key_t == WHILE_T)
    {
        printAsm ("while%zu:;\n", NUM_WHILE);
        size_t tmp = NUM_WHILE;
        NUM_WHILE++;
        processExpr (node->left, asmFile, tableVars);
        processFuncBody (node->right, asmFile, tableVars);
        processExpr (node->left, asmFile, tableVars);
        printAsm ("push 0;\n");
        printAsm ("jne while%zu:;\n", tmp);
        return ;
    }

    if (node->key_t == CALL_T)
    {
        if (node->left->b_func_t == PRINT_T)
        {
            processCallParams (node->right, asmFile, tableVars, true);
            printAsm ("out;\n");            
        }
        else if (node->left->b_func_t == SCAN_T)
        {
            if (node->right != nullptr)
            {
                printAsm ("in;\n");
                processCallParams (node->right, asmFile, tableVars, false);
            }
            else
            {
                printAsm ("in;\n");
            }
        }
        else if (node->left->b_func_t == SIN_T)
        {
            MY_ASSERT (node->right == nullptr, "\"sin\" function must have a parameter");
            processCallParams (node->right, asmFile, tableVars, true);
            printAsm ("sin;\n");
        }
        else if (node->left->b_func_t == COS_T)
        {
            MY_ASSERT (node->right == nullptr, "\"cos\" function must have a parameter");
            processCallParams (node->right, asmFile, tableVars, true);
            printAsm ("sin;\n");
        }
        else if (node->left->b_func_t == LN_T)
        {
            MY_ASSERT (node->right == nullptr, "\"ln\" function must have a parameter");
            processCallParams (node->right, asmFile, tableVars, true);
            printAsm ("ln;\n");
        }
        else
        {
            processCallParams (node->right, asmFile, tableVars, true);
            printAsm ("jmp %s:;\n", node->left->name);
        }
        return ;
    }

    if (node->left != nullptr)
    {
        processFuncBody (node->left, asmFile, tableVars);
    }

    if (node->right != nullptr)
    {
        processFuncBody (node->right, asmFile, tableVars);
    }

}

static void processExpr (node_t * node, FILE * asmFile, var_t ** tableVars)
{
    MY_ASSERT (asmFile == nullptr, "No access to the file");
    MY_ASSERT (tableVars == nullptr, "No access to the variable table");

    if (node->left != nullptr)
    {
        processExpr (node->left, asmFile, tableVars);
    }

    if (node->right != nullptr)
    {
        processExpr (node->right, asmFile, tableVars);
    }

    if (node->isNum == true)
    {
        printAsm ("push %.0lf;\n", node->elem);
    }

    if (node->id_t == ID_VAR)
    {
        bool isVarInTable = false;

        for (size_t i = 0; i < NUM_VAR; i++)
        {
            if (strcmp (node->name, (*tableVars)[i].name) == 0)
            {
                printAsm ("push[%zu];\n", (*tableVars)[i].num);
                isVarInTable = true;
                break;
            }
        }
        if (isVarInTable == false)
        {
            printf ("\033[31m VAR: %s \033[0m \n", node->name);
            MY_ASSERT (1, "undeclared variable");
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

static void processCallParams (node_t * node, FILE * asmFile, var_t ** tableVars, bool toStack)
{
    MY_ASSERT (asmFile == nullptr, "No access to the file");

    if (node == nullptr) //The case when the function has no parameters
    {
        printf ("No parameters in functions\n");
        return ;
    }

    if (node->left != nullptr)
    {
        processCallParams (node->left, asmFile, tableVars, toStack);
    }
    if (node->right != nullptr)
    {
        processCallParams (node->right, asmFile, tableVars, toStack);
    }

    if (node->id_t == ID_VAR)
    {
        bool isVarInTable = false;
        for (size_t i = 0; i < NUM_VAR; i++)
        {
            if (strcmp (node->name, (*tableVars)[i].name) == 0)
            {
                if (toStack == true)
                {
                    printAsm ("push[%zu];\n", (*tableVars)[i].num);
                }
                else
                {
                    printAsm ("pop[%zu];\n", (*tableVars)[i].num);
                }
                isVarInTable = true;
                break;
            }
        }
        if (isVarInTable == false)
        {
            printf ("var: node->name = %s\n", node->name);
            MY_ASSERT (1, "This variable is not in the table");
        }
    }
    if (node->isNum == true)
    {
        printAsm ("push %.0lf\n", node->elem);
    }
}

static void processAssign (node_t * node, FILE * asmFile, var_t ** tableVars)
{
    bool isVarInTable = false;

    if (NUM_VAR == STANDART_NUM_VAR-1)
    {
        STANDART_NUM_VAR *= 2;
        *tableVars = (var_t *) realloc (*tableVars, STANDART_NUM_VAR * sizeof(var_t));
    }
    
    processExpr (node->right->right, asmFile, tableVars);
    
    for (size_t i = 0; i < NUM_VAR; i++)
    {
        if (strcmp (node->left->name, (*tableVars)[i].name) == 0)
        {
            printAsm ("pop[%zu];\n", (*tableVars)[i].num);
            isVarInTable = true;
            break;
        }
    }
    if (isVarInTable == false)
    {
        printAsm ("pop[%zu];\n", NUM_VAR);
        (*tableVars)[NUM_VAR].name = node->left->name;
        (*tableVars)[NUM_VAR].num = NUM_VAR;
        NUM_VAR++;
    }
}