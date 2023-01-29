#include "cpu.h"

#define JUMP_FORM(sign)                             \
    i++;                                            \
    int n1 = stack_pop (&stk, logfile);             \
    int n2 = stack_pop (&stk, logfile);             \
    if (n2 sign n1)                                 \
    {                                               \
        int ptrToJmp = code[i];                     \
        i = ((size_t) ptrToJmp) - 1;                \
    }                                               \

#define BOOL_EXPR(sign)                             \
    int n1 = stack_pop (&stk, logfile);             \
    int n2 = stack_pop (&stk, logfile);             \
    if (n2 sign n1)                                 \
    {                                               \
        stack_push (&stk, 1, logfile);              \
    }                                               \
    else                                            \
    {                                               \
        stack_push (&stk, 0, logfile);              \
    }

const int NUM_REGISTERS = 4;
const int SIGNATURE_SIZE = 4;

static int getNum (void);
static int * createArrRegs (size_t numRegs);
static int * createArrCode (size_t nStrs);
static int checkBit(const int value, const int position);
static int * increaseMemSize (int * code, size_t STANDART_SIZE_CODE, FILE * binFile);

FILE * codeFile (char * nameFile)
{
    MY_ASSERT (nameFile == nullptr, "Unable to access the file");
    FILE * binFile = fopen (nameFile, "rb");
    MY_ASSERT (binFile == nullptr, "Unable to open binary file");

    return binFile;
}

size_t readNumStrs (FILE * binFile)
{
    MY_ASSERT (binFile == nullptr, "There is no access to binary file");

    fseek (binFile, SIGNATURE_SIZE*sizeof(char), SEEK_SET);

    int nStrs = -1;
    fread (&nStrs, sizeof(int), 1, binFile);
    MY_ASSERT (nStrs < 0, "The file is empty");

    return (size_t)nStrs;
}

void dumpCode (int * code, size_t numElem)
{
    FILE * logfile = fopen ("logCodeCPU.txt", "a");
    MY_ASSERT (code == nullptr, "There is no access to this code");

    fprintf (logfile, "\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");

    for (size_t i = 0; i < numElem; i++)
    {
        fprintf (logfile, "code[%zu] = %d\n", i, code[i]);
    }

    fprintf (logfile, "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    fclose (logfile);
}

int * getCode (size_t nStrs, FILE * binFile)
{
    int * code = createArrCode (nStrs);
    fread (code, sizeof(int), nStrs * 3, binFile);

    return code;
}

static int * increaseMemSize (int * code, size_t STANDART_SIZE_CODE, FILE * binFile)
{
    MY_ASSERT (binFile == nullptr, "There is no access to binary file");
    int * newPlace = (int *) realloc (code, STANDART_SIZE_CODE*3*sizeof(elem_t));

    if (newPlace == nullptr)
    {
        newPlace = (int *) realloc (code, STANDART_SIZE_CODE*2*sizeof(elem_t));
        if (newPlace == nullptr)
        {
            MY_ASSERT (1, "There is not enough memory for the program to work");
        }
        STANDART_SIZE_CODE = STANDART_SIZE_CODE*2;
    }
    else
    {
        STANDART_SIZE_CODE = STANDART_SIZE_CODE*3;
    }

    fseek (binFile, SIGNATURE_SIZE*sizeof(char), SEEK_SET);
    fread (newPlace, sizeof(int), STANDART_SIZE_CODE, binFile);
    return newPlace;
}

void cpu (int * code, size_t nStrs, size_t numTags, FILE * binFile)
{
    MY_ASSERT (code == nullptr, "Unable to access to array with code");

    FILE * logfile = open_logfile ("stack_log.txt");
    FILE * logCallStack = open_logfile ("callStack.txt");

    size_t STANDART_SIZE_CODE = nStrs * 3; //это не размер кода, а количество элементов

    dumpCode (code, nStrs * 3);

    stack_t callStack = {};
    stk_ctor (&callStack, numTags, "callStk");

    int ram[MAX_RAM] = {};

    stack_t stk = {};
    stk_ctor (&stk, STANDART_SIZE_CODE, "cpu");

    int * regs = createArrRegs (NUM_REGISTERS);
    int cmd = -1;

    #define DEF_CMD(nameCmd, length, numCmd, isArg, ...)    \
    if (cmd == CMD_##nameCmd)                               \
        __VA_ARGS__                                         \
    else

    for (size_t i = 0; ; i++)
    {
        if (i >= STANDART_SIZE_CODE)
        {
            code = increaseMemSize (code, STANDART_SIZE_CODE, binFile);
        }
        cmd = (code[i] & MASK);
        if ((checkBit(code[i], NUM) == 1) && 
            (checkBit(code[i], REG) == 0) && 
            (checkBit(code[i], RAM) == 0)) //push 7 / pop
        {
            i++;
            if (cmd == CMD_PUSH)
            {
                // printf ("code[%zu] = %d\n", i, code[i]);
                stack_push (&stk, code[i], logfile);
            }
            else if (cmd == CMD_POP)
            {
                stack_pop (&stk, logfile);
            }
            else 
            {
                MY_ASSERT (1, "Wrong command");
            }
        }
        else if ((checkBit(code[i], NUM) == 0) && 
                (checkBit(code[i], REG) == 1) && 
                (checkBit(code[i], RAM) == 0)) //push/pop rax
        {     
            i++;
            int nReg = code[i];    
            MY_ASSERT (nReg > NUM_REGISTERS-1, "You are out of register memory");

            if (cmd == CMD_PUSH)
            {
                stack_push (&stk, regs[nReg], logfile);
            }
            else if (cmd == CMD_POP)
            {
                regs[nReg] = stack_pop (&stk, logfile);
            }
            else 
            {
                MY_ASSERT (1, "Wrong command");
            }
        }
        else if ((cmd == CMD_PUSH) && 
                (checkBit(code[i], NUM) == 1) && 
                (checkBit(code[i], REG) == 1) && 
                (checkBit(code[i], RAM) == 0)) //push 5 + rax
        {
            i++;
            int nReg = code[i];
            MY_ASSERT (nReg > NUM_REGISTERS-1, "You are out of register memory");
            i++;    
            int tmp = regs[nReg] + code[i];
            stack_push (&stk, tmp, logfile);
        }
        else if ((checkBit(code[i], NUM) == 1) && 
                (checkBit(code[i], REG) == 0) && 
                (checkBit(code[i], RAM) == 1)) // push/pop[10]
        {
            i++;
            int ramIndex = code[i];
            MY_ASSERT ((size_t) ramIndex > MAX_RAM-1, "You are out of RAM");
            if (cmd == CMD_PUSH)
            {
                stack_push (&stk, ram[ramIndex], logfile);
            }
            else if (cmd == CMD_POP)
            {
                ram[ramIndex] = stack_pop (&stk, logfile);
            }
            else 
            {
                MY_ASSERT (1, "Wrong command");
            }
        }
        else if ((checkBit(code[i], NUM) == 0) && 
                (checkBit(code[i], REG) == 1) && 
                (checkBit(code[i], RAM) == 1)) //push/pop[rcx]
        {
            i++;
            int nReg = code[i];
            MY_ASSERT (nReg > NUM_REGISTERS-1, "You are out of register memory");
            int ramIndex = regs[nReg];
            MY_ASSERT ((size_t) ramIndex > MAX_RAM-1, "You are out of RAM");

            if (cmd == CMD_PUSH)
            {
                stack_push (&stk, ram[ramIndex], logfile);
            }
            else if (cmd == CMD_POP)
            {
                ram[ramIndex] = stack_pop (&stk, logfile);
            }
            else 
            {
                MY_ASSERT (1, "Wrong command");
            }
        }
        else if ((checkBit(code[i], NUM) == 1) && 
                (checkBit(code[i], REG) == 1) && 
                (checkBit(code[i], RAM) == 1)) //push/pop [5 + rcx]
        {
            i++;
            int nReg = code[i];
            MY_ASSERT (nReg > NUM_REGISTERS-1, "You are out of register memory");
            i++;
            int value1 = regs[nReg];
            int value2 = code[i];
            int ramIndex = value1 + value2;
            MY_ASSERT ((size_t) ramIndex > MAX_RAM-1, "You are out of RAM");

            if (cmd == CMD_PUSH)
            {
                stack_push (&stk, ram[ramIndex], logfile);
            }
            else if (cmd == CMD_POP)
            {
                ram[ramIndex] = stack_pop (&stk, logfile);
            }
            else 
            {
                MY_ASSERT (1, "Wrong command");
            }
        }
        else


        #include "cmd.h"
        
    
        {
            break;
        }
    }

    stack_dtor (&stk);
    stack_dtor (&callStack);

    free (regs);
    free (code);
    fclose(logfile);
}

static int getNum (void)
{
    int degreeOfNum = 0;
    int enterSymbols = scanf (" %d", &degreeOfNum);
    while (getchar() != '\n');

    if (enterSymbols == 0)
    {
        printf ("Please, enter a number\n");
        return getNum ();
    }

    return degreeOfNum;
}

static int * createArrRegs (size_t numRegs)
{
    int * regs = (int *) calloc (numRegs, sizeof(int));
    MY_ASSERT (regs == nullptr, "Unable to allocate new memory");

    return regs;
}

static int * createArrCode (size_t nStrs)
{
    int * code = (int *) calloc (nStrs * 3, sizeof (int));
    MY_ASSERT (code == nullptr, "Unable to allocate new memory");
    return code;
}

static int checkBit(const int value, const int position) 
{
    return ((value & (1 << position)) != 0);
}
