#include "backend.h"
#include "debug.h"

static char * deleteQuotMarks (char * word);
static node_t * createTree (char ** code);

char * getCode (const char * nameFile);

//------------------------------------------------------------get code--------------------------------------------------------
static FILE * openFile(const char * nameFile)
{
    MY_ASSERT (nameFile == nullptr, "Wrong pointer to name file with source code");

    FILE *file = fopen (nameFile, "rb");
    MY_ASSERT (file == nullptr, "Unable to open the source file");

    return file;
}

static size_t sizeFile (const char * argConsole)
{
    struct stat code = {};
    stat (argConsole, &code);

    return (size_t)code.st_size;
}

static void * createArr (size_t size, size_t sizeElem)
{
    void * ptrToArr = calloc (size, sizeElem);
    MY_ASSERT (ptrToArr == nullptr, "Unable to allocate new memory");

    return ptrToArr;
}

static void copyFromFile (FILE * src, void * dst, size_t sizeFile)
{
    MY_ASSERT (src == nullptr, "Wrong pointer to the file");

    fread (dst, sizeof(char), sizeFile, src);
}

char * getCode (const char * nameFile)
{
    FILE * fileCode = openFile (nameFile);
    size_t sizeArr = sizeFile (nameFile);
    char * code = (char *) createArr(sizeArr + 1, sizeof(char));
    copyFromFile (fileCode, code, sizeArr);
    code[sizeArr] = '\0';

    fclose (fileCode);

    return code;
}
//-------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------create tree---------------------------------------------------------------

static char * skipSpace(char *str)
{
    MY_ASSERT(str == nullptr, "the pointer to the string is null");

    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == ',')
    {
        if (str[0] == '_')
        {
            MY_ASSERT(1, "The word should not start with an underscore");
        }
        str++;
    }

    return str;
}

static char * getWord(char * str)
{
    MY_ASSERT(str == nullptr, "There is no access to this string");

    char * tmp = str;
    size_t lengthWord = 0;

    while (*str != ' ' && *str != '\t' && *str != '\n' && *str != '\0' && *str != ')' && *str != '(')
    {
        lengthWord++;
        str++;
    }

    if (lengthWord == 0)
    {
        return nullptr;
    }

    char * word = (char *) calloc (lengthWord + 1, sizeof(char));
    MY_ASSERT(word == nullptr, "Unable to allocate new memory");

    word = (char *) memmove (word, tmp, lengthWord * sizeof(char));
    word[lengthWord] = '\0';

    return word;
}

//----------------------------------------------------------------------------------------------------------------------

int main (int argc, char * argv[])
{
    char * code = getCode (argv[1]);

    node_t * node = createTree (&code);

    graphicDumpTree (node);

    deleteTree (node);

    return 0;
}

static node_t * createTree (char ** code)
{
    MY_ASSERT (code == nullptr, "There is no access to the code");

    $code (*code)

    char * word = getWord (*code);
    char * tmpWord = word;
    node_t * node = nullptr;
    bool isQuotMarks = false;

    #define TYPES(cmd, funcArg)                                                     \
            if (strcmp (word, #cmd) == 0)                                           \
            {                                                                       \
                node = createKeyNode (funcArg, nullptr, nullptr, #cmd);             \
            }                                                                       \
            else

    #define OPER(oper, funcArg)                                                     \
            if (strcmp (word, #oper) == 0)                                           \
            {                                                                       \
                node = createNodeWithOperation (funcArg, nullptr, nullptr, #oper);  \
            }                                                                       \
            else

    if (strchr(word, '"') != nullptr)
    {
        word = deleteQuotMarks (word);
        node = createNodeWithVariable (word);
        isQuotMarks = true;
        free (tmpWord);
    }
    else if (word[0] >= 48 && word[0] <= 57 && isalpha(word[1]) == 0)
    {
        node = createNodeWithNum (atoi(word));
    }
    else

    #include "types.h"
    #include "operations.h"

    {
        MY_ASSERT (1, "Something error");
    }

    #undef TYPES
    #undef OPER

    size_t lengthWord = strlen (word);
    if (isQuotMarks == true)
    {
        lengthWord = lengthWord + 2;
    }

    *code = *code + lengthWord;

    $str ("lengthWord = %zu", lengthWord)
    $node (node)
    $code (*code)

    MY_ASSERT (**code != '(' && **code != ')', "Error in reading the code");

    node_t * nodeL = nullptr;
    if (**code != ')')
    {
        (*code)++;
        if (**code == ')')
        {
            nodeL = nullptr;
        }
        else
        {
            nodeL = createTree (code);
        }
    }

    $code (*code)
    MY_ASSERT (**code != ')', "Error in reading the code");
    (*code)++;
    MY_ASSERT (**code != '(', "Error in reading the code");

    node_t * nodeR = nullptr;
    if (**code != ')')
    {
        (*code)++;
        if (**code == ')')
        {
            nodeR = nullptr;
        }
        else
        {
            nodeR = createTree (code);
        }
    }

    (*code)++;
    
    node->left = nodeL;
    node->right = nodeR;
    if (nodeL != nullptr)
    {
        nodeL->parent = node;
    }
    if (nodeR != nullptr)
    {
        nodeR->parent = node;
    }

    if (node->id_t != 1 && node->id_t != 2)
    {
        free (tmpWord);
    }

    if (node->key_t == FUNC_T || node->key_t == CALL_T)
    {
        char * name = node->left->name;
        deleteNode (node->left);
        node->left = createNodeWithFunction (name);
    }

    return node;
}

static char * deleteQuotMarks (char * word)
{
    MY_ASSERT (word == nullptr, "The function has no arguments");

    size_t lengthWord = strlen (word);
    lengthWord = lengthWord - 2;
    word++;

    char * newWord = (char *) calloc (lengthWord+1, sizeof(char));

    newWord = (char *) memmove (newWord, word, lengthWord);
    newWord[lengthWord] = '\0';

    return newWord;
}

