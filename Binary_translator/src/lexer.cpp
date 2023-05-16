#include "../include/lexer.hpp"

const int MASK = (1<<4) + (1<<3) + (1<<2) + (1<<1) + 1; //0001|1111

const int RAM = 7; //0000 | 0001  -> 1000 | 0000
const int REG = 6; //0000 | 0001  -> 0100 | 0000
const int NUM = 5; //0000 | 0001  -> 0010 | 0000

const int NUM_REGISTERS = 4;

const size_t MAX_RAM = 100;

#define DEF_CMD(name, numCmd, ...)    \
    CMD_##name = numCmd,

typedef enum {
    #include "cmd.h"
} COMMANDS;

#undef DEF_CMD

#define JUMP_FORM(command)                          \
    commandsArray[numCmds] = {                      \
        .name           = #command,                 \
        .cmd            = CMD_##command,            \
        .nativeSize     = 2,                        \
        .nativeIP       = i-1,                      \
        .type_argument  = LABEL,                    \
        .argument       = byteCode[i]               \
    };

#define BOOL_EXPR(command)                          \
    commandsArray[numCmds] = {                      \
        .name           = #command,                 \
        .cmd            = CMD_##command,            \
        .nativeSize     = 2,                        \
        .nativeIP       = i,                        \
        .type_argument  = LABEL,                    \
        .argument       = byteCode[i]               \
    };

static int checkBit(const int value, const int position);
static int * createArrRegs (size_t numRegs);

cmd_t * createIRArray (code_t code)
{
    cmd_t * array = (cmd_t *) calloc (code.sizeBuf/sizeof(int), sizeof(cmd_t));
    MY_ASSERT (array == nullptr, "Unable to allocate memory");

    return array;
}

size_t setIR (cmd_t * commandsArray, int * byteCode)
{
    MY_ASSERT (commandsArray == nullptr, "There is no access to the array with code");

    int * regs = createArrRegs (NUM_REGISTERS);
    int cmd = -1;

    size_t numCmds = 0;

    #define DEF_CMD(nameCmd, numCmd, isArg, ...)            \
    if (cmd == CMD_##nameCmd)                               \
        __VA_ARGS__                                         \
    else

    for (size_t i = 0; ; i++, numCmds++)
    {
        cmd = (byteCode[i] & MASK);

        if ( (checkBit(byteCode[i], NUM) == 1) && 
             (checkBit(byteCode[i], REG) == 0) && 
             (checkBit(byteCode[i], RAM) == 0)) //push 7 / pop
        {
            i++;
            if (cmd == CMD_PUSH)
            {
                commandsArray[numCmds] = {
                    .name           = "push_num",
                    .cmd            = CMD_PUSH,
                    .nativeSize     = 2,
                    .nativeIP       = i-1,
                    .type_argument  = NUMBER,
                    .argument       = byteCode[i]
                };
            }
            else if (cmd == CMD_POP)
            {
                i++;
                commandsArray[numCmds] = {
                    .name           = "pop_empty",
                    .cmd            = CMD_POP,
                    .nativeSize     = 2,
                    .nativeIP       = i-1,
                    .type_argument  = NUMBER,
                    .argument       = byteCode[i]
                };
            }
            else
            {
                MY_ASSERT (1, "Wrong command (section pushORpop_num)");
            }
        }
        else if ((checkBit(byteCode[i], NUM) == 0) && 
                 (checkBit(byteCode[i], REG) == 1) && 
                 (checkBit(byteCode[i], RAM) == 0)) //push/pop rax
        {     
            i++;
            int nReg = byteCode[i];    
            MY_ASSERT (nReg > NUM_REGISTERS-1, "You are out of register memory");

            if (cmd == CMD_PUSH)
            {
                commandsArray[numCmds] = {
                    .name           = "push_reg",
                    .cmd            = CMD_PUSH,
                    .nativeSize     = 2,
                    .nativeIP       = i-1,
                    .type_argument  = REGISTER,
                    .argument       = byteCode[i]
                };
            }
            else if (cmd == CMD_POP)
            {
                commandsArray[numCmds] = {
                    .name           = "pop_reg",
                    .cmd            = CMD_POP,
                    .nativeSize     = 2,
                    .nativeIP       = i-1,
                    .type_argument  = REGISTER,
                    .argument       = byteCode[i]
                };
            }
            else
            {
                MY_ASSERT (1, "Wrong command");
            }
        }
        else if ((cmd == CMD_PUSH) && 
                 (checkBit(byteCode[i], NUM) == 1) && 
                 (checkBit(byteCode[i], REG) == 1) && 
                 (checkBit(byteCode[i], RAM) == 0)) //push 5 + rax
        {
            i++;
            int nReg = byteCode[i];
            printf ("nReg = %d, i = %zu\n", nReg, i);
            MY_ASSERT (nReg > NUM_REGISTERS-1, "You are out of register memory");
            i++;
            int tmp = regs[nReg] + byteCode[i];
            commandsArray[numCmds] = {
                .name           = "push_num_reg",
                .cmd            = CMD_PUSH,
                .nativeSize     = 3, 
                .nativeIP       = i-2,
                .type_argument  = NUM_REG,
                .argument       = tmp
            };
        }
        else if ((checkBit(byteCode[i], NUM) == 1) && 
                 (checkBit(byteCode[i], REG) == 0) && 
                 (checkBit(byteCode[i], RAM) == 1)) // push/pop[10]
        {
            i++;
            int ramIndex = byteCode[i];
            MY_ASSERT ((size_t) ramIndex > MAX_RAM-1, "You are out of RAM");
            if (cmd == CMD_PUSH)
            {
                commandsArray[numCmds] = {
                    .name           = "push_num",
                    .cmd            = CMD_PUSH,
                    .nativeSize     = 2,
                    .nativeIP       = i-1,
                    .type_argument  = MEM_NUM,
                    .argument       = ramIndex
                }; 
            }
            else if (cmd == CMD_POP)
            {
                commandsArray[numCmds] = {
                    .name           = "pop_num",
                    .cmd            = CMD_POP,
                    .nativeSize     = 2,
                    .nativeIP       = i-1,
                    .type_argument  = MEM_NUM,
                    .argument       = ramIndex
                }; 
            }
            else 
            {
                MY_ASSERT (1, "Wrong command");
            }
        }
        else if ((checkBit(byteCode[i], NUM) == 0) && 
                 (checkBit(byteCode[i], REG) == 1) && 
                 (checkBit(byteCode[i], RAM) == 1)) //push/pop[rcx]
        {
            i++;
            int nReg = byteCode[i];
            MY_ASSERT (nReg > NUM_REGISTERS-1, "You are out of register memory");
            int ramIndex = regs[nReg];
            MY_ASSERT ((size_t) ramIndex > MAX_RAM-1, "You are out of RAM");

            if (cmd == CMD_PUSH)
            {
                commandsArray[numCmds] = {
                    .name           = "push_reg",
                    .cmd            = CMD_PUSH,
                    .nativeSize     = 2,
                    .nativeIP       = i-1,
                    .type_argument  = MEM_REG,
                    .argument       = ramIndex
                };
            }
            else if (cmd == CMD_POP)
            {
                commandsArray[numCmds]  = {
                    .name           = "pop_reg",
                    .cmd            = CMD_POP,
                    .nativeSize     = 2,
                    .nativeIP       = i-1,
                    .type_argument  = MEM_NUM,
                    .argument       = ramIndex
                }; 
            }
            else 
            {
                MY_ASSERT (1, "Wrong command");
            }
        }
        else if ((checkBit(byteCode[i], NUM) == 1) && 
                (checkBit(byteCode[i], REG) == 1) && 
                (checkBit(byteCode[i], RAM) == 1)) //push/pop [5 + rcx]
        {
            i++;
            int nReg = byteCode[i];
            MY_ASSERT (nReg > NUM_REGISTERS-1, "You are out of register memory");
            i++;
            int value1 = regs[nReg];
            int value2 = byteCode[i];
            int ramIndex = value1 + value2;
            MY_ASSERT ((size_t) ramIndex > MAX_RAM-1, "You are out of RAM");

            if (cmd == CMD_PUSH)
            {
                commandsArray[numCmds] = {
                    .name           = "push_mem_num_reg",
                    .cmd            = CMD_PUSH,
                    .nativeSize     = 3,
                    .nativeIP       = i-2,
                    .type_argument  = MEM_NUM_REG,
                    .argument       = ramIndex
                };
            }
            else if (cmd == CMD_POP)
            {
                commandsArray[numCmds]  = {
                    .name           = "pop_mem_num_reg",
                    .cmd            = CMD_POP,
                    .nativeSize     = 3,
                    .nativeIP       = i-2,
                    .type_argument  = MEM_NUM_REG,
                    .argument       = ramIndex
                };
            }
            else 
            {
                MY_ASSERT (1, "Wrong command");
            }
        }
        else


        #include "cmd.h"
        
    
        {
            printf ("cmd = %d, i = %zu, prevCmd = %s\n", cmd, i, commandsArray[i-2].name);
            break;
        }
    }

    free (regs);

    return numCmds;
}

static int checkBit(const int value, const int position) 
{
    return ((value & (1 << position)) != 0);
}

static int * createArrRegs (size_t numRegs)
{
    int * regs = (int *) calloc (numRegs, sizeof(int));
    MY_ASSERT (regs == nullptr, "Unable to allocate new memory");

    return regs;
}


