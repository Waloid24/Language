#include "asm.h"

#define DEF_CMD(name, numLetters, numCmd, isArg, ...)    \
    CMD_##name = numCmd,

typedef enum {
    #include "cmd.h"
} COMMANDS;

#undef DEF_CMD

const int RAM = 7; //0000 | 0001  -> 1000 | 0000
const int REG = 6; //0000 | 0001  -> 0100 | 0000
const int NUM = 5; //0000 | 0001  -> 0010 | 0000

const int SIGNATURE_SIZE = 4;

int N_FUNC_WITHOUT_ARG = 0;

const int NO_TAGS = -2;

enum {
    NO = 0,
    YES = 1
};

static void scanTag (char * src, char * dst, size_t lengthSrc);
static int findFreePlace (tag_t * tags, int sizeArrTags);
static long int findTag (tag_t * tags, char * argument, int * startArrWithCode, size_t numTags);
static void ram (int ** code, char * firstBracket, int numCmd);
static void no_ram (int ** code, char * strCode, int countLetters, int numCmd);
static void getArg (int ** code, char * str_text_code, int countLetters, int numCmd, FILE * logfile);
static void freeArrTags (tag_t * tags, freeCall_t * calls, size_t sizeArrTags);
static int setbit (int value, int position);
static char readNum (char * src, double * doubleNumDst, int * intNumDst);
static void skipSpace (char ** strCode, int countLetters);

void dumpCode (char ** arrStrs, int * code, size_t numElem)
{
    MY_ASSERT (code == nullptr, "There is no access to this code");
    MY_ASSERT (arrStrs == nullptr, "There is no access to array with strings");

    FILE * logfile = fopen ("logCode.txt", "a");
    fprintf (logfile, "\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");

    for (size_t i = 0; i < numElem; i++)
    {
        fprintf (logfile, "code[%lu] = %d\n", i, code[i]);
    }

    fprintf (logfile, "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");

    fclose (logfile);
}

void createBinFile (char ** arrStrs, code_t * prog, char * nameBinFile, size_t numTags)
{
    MY_ASSERT (arrStrs == nullptr, "There is no access to array of strings");
    MY_ASSERT (prog == nullptr, "There is no access to struct with information about code file");

    FILE * binFile = fopen (nameBinFile, "a+b");
    MY_ASSERT (binFile == nullptr, "There is no access to bin file");
    setbuf (binFile, NULL);

    FILE * logfile = fopen ("logfile.txt", "a");
    MY_ASSERT (logfile == nullptr, "There is no access to bin file");
    setbuf (logfile, NULL);

    tag_t * tags = (tag_t *) calloc (numTags, sizeof(tag_t));
    MY_ASSERT (tags == nullptr, "Unable to allocate new memory");

    int * code = (int *) calloc (prog->nStrs * 3, sizeof(int));
    int * tmp = code;

    long int tagCallOrJmps = 0;

    unsigned long int numCmdAndArgs = 0;

    freeCall_t * missingTags = (freeCall_t *) calloc (numTags, sizeof(freeCall_t));
    MY_ASSERT (missingTags == nullptr, "Unable to allocate new memory");
    freeCall_t * saveMissingTags = missingTags;
    freeCall_t * saveMissingTags_1 = missingTags;

    #define DEF_CMD(nameCmd, countLetters, numCmd, isArg, ...)                  \
        if (myStrcmp (cmd, #nameCmd) == 0)                                      \
        {                                                                       \
            int * tmp1 = code;                                                  \
            if (isArg)                                                          \
            {                                                                   \
                countArgs++;                                                    \
                getArg (&code, arrStrs[i], countLetters, numCmd, logfile);      \
                numCmdAndArgs++;\
                code++;                                                         \
            }                                                                   \
            else                                                                \
            {                                                                   \
                *code = CMD_##nameCmd;                                          \
                numCmdAndArgs++;                                                \
                code++;                                                         \
            }                                                                   \
            fprintf (logfile, "code[%zu] = %d (%s)\n", numCmdAndArgs, *tmp1, arrStrs[i]);    \
            numCmdAndArgs++;                                                    \
            tmp1++;                                                             \
            for (; tmp1 != code; tmp1++, numCmdAndArgs++)                       \
            {                                                                   \
                fprintf (logfile, "code[%zu] = %d\n", numCmdAndArgs, *tmp1);    \
            }                                                                   \
            numCmdAndArgs--;                                                    \
        }                                                                       \
        else 

    #define CALLS_AND_JMPS(nameJmp, length)                                     \
        if (myStrcmp (cmd, #nameJmp) == 0)                                      \
        {                                                                       \
            countArgs++;                                                        \
            *code = CMD_##nameJmp;                                              \
            fprintf (logfile, "code[%zu] = %d (%s)\n", numCmdAndArgs, *code, arrStrs[i]);   \
            numCmdAndArgs++;\
            code++;                                                             \
                                                                                \
            skipSpace (&(arrStrs[i]), length);                                  \
                                                                                \
            size_t lengthTag = strlen (arrStrs[i]);                             \
                                                                                \
            char * nameTag = (char *) calloc (lengthTag, sizeof(char));         \
            scanTag (arrStrs[i], nameTag, lengthTag);                           \
                                                                                \
            tagCallOrJmps = findTag (tags, nameTag, tmp, numTags);              \
            if (tagCallOrJmps == NO_TAGS)                                       \
            {                                                                   \
                missingTags->ptrToArrWithCode = code;                           \
                missingTags->tag = nameTag;                                     \
                N_FUNC_WITHOUT_ARG++;                                           \
                code++;                                                         \
                missingTags++;                                                  \
            }                                                                   \
            else                                                                \
            {                                                                   \
                *code = (int) tagCallOrJmps;                                    \
                fprintf (logfile, "code[%zu] = %d\n", numCmdAndArgs, *code);    \
                code++;                                                         \
                free (nameTag);                                                 \
            }                                                                   \
        }                                                                       \
        else

    char * cmd = (char *) calloc (STANDART_SIZE, sizeof(char));
    MY_ASSERT (cmd == nullptr, "Unable to allocate new memory");

    int countArgs = 0;
    for (size_t i = 0; i < prog->nStrs; i++)
    {
        sscanf (arrStrs[i], "%s", cmd);
        printf ("cmd = %s\n", cmd);
        if (strchr (cmd, ':') != nullptr)
        {
            int freeTag = findFreePlace (tags, (int) numTags);
            MY_ASSERT (freeTag == -1, "There are no free cells in the tag array");

            size_t lengthSrc = strlen (cmd);

            tags[freeTag].nameTag = (char *) calloc (lengthSrc, sizeof(char));
            MY_ASSERT (tags[freeTag].nameTag == nullptr, "Unable to allocate new memory");
            scanTag (cmd, tags[freeTag].nameTag, lengthSrc);

            tags[freeTag].ptr = code;
        }
        else


        #include "jmps.h"
        #include "cmd.h"


        {
            fprintf (stderr, "command undifined is \"%s\" (arrStrs[%lu])\n", arrStrs[i], i);
            MY_ASSERT (1, "This command is not defined.\n");
        }
    }
    for (int i = 0; i < N_FUNC_WITHOUT_ARG; i++)
    {
        tagCallOrJmps = findTag (tags, saveMissingTags->tag, tmp, numTags);
        MY_ASSERT (tagCallOrJmps == NO_TAGS, "This tag does not exist");
        *(saveMissingTags->ptrToArrWithCode) = (int) tagCallOrJmps;
        saveMissingTags++;
    }

    dumpCode (arrStrs, tmp, prog->nStrs * 3);

    fwrite (tmp, sizeof(int), prog->nStrs * 3, binFile);

    fclose (binFile);
    freeArrTags (tags, saveMissingTags_1, numTags);
    free (saveMissingTags_1);
    free (tmp);
    free (tags);
    free (cmd);
    #undef DEF_CMD
    #undef CALLS_AND_JUMPS
}

static int findFreePlace (tag_t * tags, int sizeArrTags)
{
    for (int i = 0; i < sizeArrTags; i++)
    {
        if (tags[i].ptr == nullptr) 
        {
            return i;
        }
    }
    return -1;
}

static void freeArrTags (tag_t * tags, freeCall_t * calls, size_t sizeArrTags)
{
    MY_ASSERT (tags == nullptr, "No access to the tag array");
    for (size_t i = 0; i < sizeArrTags; i++)
    {
        if (tags[i].ptr != nullptr)
        {
            free (tags[i].nameTag);
        }
    }
    for (size_t i = 0; i < sizeArrTags && calls->tag != nullptr; i++)
    {
        free (calls->tag);
        calls++;
    }
}

static void scanTag (char * src, char * dst, size_t lengthSrc)
{
    for (size_t i = 0; i < lengthSrc; i++)
    {
        if (*src == ':' || *src == ';')
        {
            *dst = '\0';
            break;
        }
        if (isalnum(*src) || ispunct(*src))
        {
            *dst = *src;
            dst++;
            src++;
        }
        else
        {
            MY_ASSERT (1, "Incorrect tag: missing \":\" or there are invalid characters");
        }
    }
}

static void getArg (int ** code, char * str_text_code, int countLetters, int numCmd, FILE * logfile)
{
    MY_ASSERT (logfile == nullptr, "There is no access to logfile");
	char * firstBracket = nullptr;
    if ((firstBracket = strchr (str_text_code, '[')) != nullptr)
    {
        ram(code, firstBracket, numCmd);
    }
    else
	{
        no_ram(code, str_text_code, countLetters, numCmd);
	}
}

static void no_ram (int ** code, char * strCode, int countLetters, int numCmd) // tag_t * tags
{
    skipSpace (&strCode, countLetters);
    char * ptrToArg = strCode;

    char * reg  = (char *) calloc (4, sizeof(char));
    MY_ASSERT (reg == nullptr, "It's impossible to read the argument");

    char * ptrToReg = nullptr;

    char * placeOfPlus = nullptr;

    if (strchr(ptrToArg, '-') != nullptr)
    {
        MY_ASSERT (numCmd != CMD_PUSH, "the minus can only be present in the \"push\" command"); 
        **code = setbit (**code, NUM);
        **code = **code | CMD_PUSH;
        (*code)++;
        **code = atoi(strCode);
    }
    else if ((ptrToReg = strchr(ptrToArg, 'r')) != nullptr)
    {
        reg = (char *) memcpy (reg, ptrToReg, 3 * sizeof(char));
        reg[3] = '\0';

        char count_reg = 0;
        if      (strcmp (reg, "rax") == 0) count_reg = (char) RAX;
        else if (strcmp (reg, "rbx") == 0) count_reg = (char) RBX;
        else if (strcmp (reg, "rcx") == 0) count_reg = (char) RCX;
        else if (strcmp (reg, "rdx") == 0) count_reg = (char) RDX;
        else    MY_ASSERT (1, "The case is specified incorrectly");

        if ((placeOfPlus = strchr(ptrToArg, '+')) != nullptr)
        {
            placeOfPlus++;
            **code = setbit (**code, REG);
            **code = setbit (**code, NUM);
            if (numCmd == CMD_PUSH) //0110 | 0001
            {
                **code = **code | CMD_PUSH;
                
            }
            else if (numCmd == CMD_POP) //0110 | 0010
            {
                **code = **code | CMD_POP;
            }   
            else
            {
                MY_ASSERT (1, "Incorrect expression with \"+\"");
            }

            (*code)++;

            skipSpace(&placeOfPlus, 0);
            int num = 0;
            int nSymbols = readNum (ptrToArg, nullptr, &num);
            if (nSymbols == 0)
            {
                readNum (placeOfPlus, nullptr, &num);
            }
            **code = count_reg;
            (*code)++;
            **code = num;
        }
        else
        {
            **code = setbit (**code, REG);
            if (numCmd == CMD_PUSH)
            {
                **code = **code | CMD_PUSH;
            }
            else if (numCmd == CMD_POP)
            {
                **code = **code | CMD_POP;
            }
            else
            {
                MY_ASSERT (1, "Incorrect expression with a case without \"+\"");
            }
            (*code)++;
            **code = count_reg;
        }
    }
    else
    {
        double num = 0;
        int nSymbols = readNum (ptrToArg, &num, nullptr);
        **code = setbit (**code, NUM);
        if (numCmd == CMD_PUSH)
        {
            MY_ASSERT (nSymbols == 0, "Push without a numeric argument\n");
            **code = **code | CMD_PUSH;
        }
        else if (numCmd == CMD_POP)
        {
            MY_ASSERT (nSymbols != 0, "Pop with a numeric argument\n");
            **code = **code | CMD_POP;
        }
        else
        {
            MY_ASSERT (1, "Incorrect push or pop");
        }
        (*code)++;
        **code = (int) num; //?
    }

    free (reg);
}

static void ram (int ** code, char * firstBracket, int numCmd)
{
    char * reg  = (char *) calloc (4, sizeof(char));
    MY_ASSERT (reg == nullptr, "It's impossible to read the argument");
    char * trash = (char *) calloc (8, sizeof(char)); 
    MY_ASSERT (trash == nullptr, "func GetArgument: it's impossible to read other symbols");

	int num = -1;
    firstBracket++;
    if (strcmp (firstBracket, "r") >= 0)
    {
        memmove (reg, firstBracket, 3*sizeof(char));
        reg[3] = '\0';
        firstBracket = firstBracket + 3;
        skipSpace (&firstBracket, 0);
        sscanf (firstBracket, "%[+]", trash);
        firstBracket = firstBracket + 1;
        skipSpace (&firstBracket, 0);
        sscanf (firstBracket, "%d", &num);
    }
    else 
    {
        int nDigit = readNum (firstBracket, nullptr, &num);
        firstBracket = firstBracket + nDigit;
        skipSpace (&firstBracket, 0);      
        sscanf (firstBracket, "%[+]", trash);
        firstBracket = firstBracket + 1;
        skipSpace (&firstBracket, 0);
        sscanf (firstBracket, "%[rabcdx]", reg);
    }

    MY_ASSERT (num == -1 && *reg == 0, "Your argument in square brackets are incorrect");

    if (num == -1 && *reg != 0)
    {
        char count_reg = 0;
        if      (strcmp (reg, "rax") == 0) count_reg = (char) RAX;
        else if (strcmp (reg, "rbx") == 0) count_reg = (char) RBX;
        else if (strcmp (reg, "rcx") == 0) count_reg = (char) RCX;
        else if (strcmp (reg, "rdx") == 0) count_reg = (char) RDX;
        else    MY_ASSERT (1, "The case is specified incorrectly");

        **code = setbit (**code, RAM);
        **code = setbit (**code, REG);
        if (numCmd == CMD_PUSH)
        {
            **code = **code | CMD_PUSH;
        }
        else
        {
            **code = **code | CMD_POP;
        }
        (*code)++;
        **code = count_reg;

    }
    
    else if (*reg == 0 && num != -1) //push [5] pop[5]
    {
        **code = setbit (**code, RAM);
        **code = setbit (**code, NUM);
        if (numCmd == CMD_PUSH)
        {
            **code = **code | CMD_PUSH;
        }
        else
        {
            **code = **code | CMD_POP;
        }
        (*code)++;
        **code = num;
    }
    else 
    {
		char count_reg = 0;
        if      (strcmp (reg, "rax") == 0) count_reg = (char) RAX;
        else if (strcmp (reg, "rbx") == 0) count_reg = (char) RBX;            
        else if (strcmp (reg, "rcx") == 0) count_reg = (char) RCX;
        else if (strcmp (reg, "rdx") == 0) count_reg = (char) RDX;
        else    MY_ASSERT (1, "The case is specified incorrectly");

        **code = setbit (**code, REG);
        **code = setbit (**code, RAM);
        **code = setbit (**code, NUM);

        if (numCmd == CMD_PUSH)
        {
            **code = **code | CMD_PUSH;
        }
        else
        {
            **code = **code | CMD_POP;
        }

        (*code)++;
        **code = count_reg;
        (*code)++;
        **code = num;
    }
    free (reg);
    free (trash);
}

void pushSignature (char * nameBinFile, code_t codeFile)
{
    FILE * dst = fopen (nameBinFile, "w");
	MY_ASSERT (dst == nullptr, "null pointer to file");

    char * nameProg = (char *) calloc (SIGNATURE_SIZE, sizeof(char));
    char * signature = (char *) calloc (1, SIGNATURE_SIZE*sizeof(char) + sizeof(int));
    char * savePtrName = nameProg;
    char * savePtrSignature = signature;
    char version = 1;
	*nameProg = 'A';
    nameProg++;
    *nameProg = 'S';
    nameProg++;
    *nameProg = 'M';
    nameProg++;
    *nameProg = version; 

    memcpy (signature, savePtrName, SIGNATURE_SIZE*sizeof(char));
    signature = signature + SIGNATURE_SIZE*sizeof(char);

    size_t nStrings = codeFile.nStrs;
    memcpy (signature, &nStrings, sizeof(int));

	fwrite (savePtrSignature, SIGNATURE_SIZE*sizeof(char) + sizeof(int), 1, dst);

    free (savePtrName);
    free (savePtrSignature);
    fclose (dst);
}

static long int findTag (tag_t * tags, char * argument, int * startTagWithCode, size_t numTags)
{
    MY_ASSERT (tags == nullptr, "There is no access to array with tags");
    for (size_t i = 0; i < numTags; i++)
    {
        if ((tags[i].nameTag != nullptr) && (strcmp (argument, tags[i].nameTag) == 0))
        {
            return tags[i].ptr - startTagWithCode;
        }
    }
    return NO_TAGS;
}

static int setbit (int value, int position)
{
    return (value | (1 << position));
}

static char readNum (char * src, double * doubleNumDst, int * intNumDst)
{
    MY_ASSERT (src == nullptr, "There is no access to source string");
    char sym = 0;
    char * tmp = src;

	if (doubleNumDst == nullptr && intNumDst != nullptr)
	{
		while (48 <= *tmp && *tmp <= 57)
		{
			tmp++;
			sym++;
		}
		*doubleNumDst = atoi(src);
	}
	else if (doubleNumDst != nullptr && intNumDst == nullptr)
    {
		while ((48 <= *tmp && *tmp <= 57) || *tmp == '.' || *tmp == ',')
		{
			tmp++;
			sym++;
		}
		*doubleNumDst = atoi(src);
	}
    
    return sym;
}

static void skipSpace (char ** strCode, int countLetters) //TODO: remove clons
{
    *strCode = *strCode + countLetters;
	int i = 0;
    for (; **strCode == ' ' || **strCode == '\t'; i++)
    {
        (*strCode)++;
    }
}