#include "generator.h"

static char * deleteQuotMarks (char * word);
static char * getWord(char * str);

node_t * createTree (char ** code)
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
            if (strcmp (word, #oper) == 0)                                          \
            {                                                                       \
                node = createNodeWithOperation (funcArg, nullptr, nullptr, #oper);  \
            }                                                                       \
            else
        
    #define BASE_FUNC(name, type)                                                   \
            if (strcmp (word, name) == 0)                                           \
            {                                                                       \
                node = createNodeWithFunction (name);                               \
            }                                                                       \
            else

    if (strchr(word, '"') != nullptr)
    {
        word = deleteQuotMarks (word);
        node = createNodeWithVariable (word);
        isQuotMarks = true;
        free (tmpWord);
    }
    else if ((word[0] >= 48 && word[0] <= 57 && isalpha(word[1]) == 0) ||
            (word[0] == '-' && word[1] >= 48 && word[1] <= 57))
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
    #undef BASE_FUNC

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
        graphicDumpTree (node->left);
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