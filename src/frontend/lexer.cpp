#include "lexer.h"

#define DEF_CMD(nameCmd, value, ...)\
    const int nameCmd = value;

#include "cmd.h"

#undef DEF_CMD

static void setToken(char *word, token_t *tokens, size_t ip);
static char * getWord(char * str);
static char * skipSpace(char *str);
static void dumpLexer (token_t * tokens, size_t numTokens, FILE * logfile);

retLex_t getTokens (char * code, char * nameLogFile)
{
    MY_ASSERT (code == nullptr, "Wrong pointer to the array with your code");

    FILE * logfile = openFile (nameLogFile, "w");

    retLex_t result = {};

    token_t * tokens = (token_t *) allocateMemory (STANDART_SIZE_TOKEN_ARR, sizeof(token_t));
    size_t tmpStandartSize = STANDART_SIZE_TOKEN_ARR;
    size_t numTokens = 0;

    char * word = nullptr;

    for (size_t i = 0; *code != '\0'; i++)
    {
        if (i == (tmpStandartSize - 1))
        {
            tmpStandartSize *= 2;
            tokens = (token_t *)realloc(tokens, tmpStandartSize * sizeof(token_t)); 
        }

        code = skipSpace(code);
        word = getWord(code);
        if (word == nullptr)
        {
            free(word);
            break;
        }
        setToken(word, tokens, i);
        code = code + strlen(word);
        numTokens++;

        if (tokens[i].type != TYPE_ID       && tokens[i].type != TYPE_SIN   &&
            tokens[i].type != TYPE_COS      && tokens[i].type != TYPE_LN    &&
            tokens[i].type != TYPE_PRINT    && tokens[i].type != TYPE_SCAN  &&
            tokens[i].type != TYPE_SQRT)
        {
            free(word);
        }
    }

    numTokens++;
    if (numTokens == (tmpStandartSize - 1))
    {
        tmpStandartSize++;
        tokens = (token_t *) realloc (tokens, tmpStandartSize * sizeof(token_t));
    }
    tokens[numTokens-1].type = TYPE_END_TOKENS;

    result.tokens  = tokens;
    result.nTokens = numTokens;

    dumpLexer (tokens, numTokens, logfile);

    return result;
}

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

    while (*str != ' ' && *str != '\t' && *str != '\n' && *str != '\0' && *str != ',')
    {
        if (*str == '-')
        {
            lengthWord = 1;
            break;
        }
        lengthWord++;
        str++;
    }

    if (lengthWord == 0)
    {
        return nullptr;
    }

    char * word = (char *) allocateMemory (lengthWord + 1, sizeof(char));
    word = (char *) memmove (word, tmp, lengthWord * sizeof(char));
    word[lengthWord] = '\0';

    return word;
}

static void setToken (char *word, token_t *tokens, size_t ip)
{
    MY_ASSERT(word == nullptr, "There is no access to this word");
    MY_ASSERT(word[0] == '_', "Uncorrect symbol: \'_\' should not be at the beginning of the word");
    MY_ASSERT((word[0] >= 48 && word[0] <= 57) && ((word[1] < 48 || word[1] > 57) && (word[1] != '.' && word[1] != ',')) 
                && strlen(word) > 1, "Uncorrect ...");

    #define DEF_CMD(nameCmd, value, terminal)                   \
        if (strcmp (word, terminal) == 0)                       \
        {                                                       \
            tokens[ip].type = nameCmd;                          \
            if (nameCmd == TYPE_SIN || nameCmd == TYPE_COS ||   \
                nameCmd == TYPE_LN ||  nameCmd == TYPE_PRINT || \
                nameCmd == TYPE_SCAN || nameCmd == TYPE_SQRT)   \
            {                                                   \
                tokens[ip].u1.id = word;                        \
            }                                                   \
        }                                                       \
        else

    #include "cmd.h"
    
    if (word[0] >= 48 && word[0] <= 57 && isalpha(word[1]) == 0) 
    {                                                            
        tokens[ip].type = TYPE_NUM;
        tokens[ip].u1.value = atoi(word);
    }
    else
    {
        tokens[ip].type = TYPE_ID;
        tokens[ip].u1.id = word;
    }

    #undef DEF_CMD
}

static void dumpLexer (token_t * tokens, size_t numTokens, FILE * logfile)
{
    for (size_t i = 0; i < numTokens; i++)
    {
        fprintf (logfile, "tokens[%zu]\n", i);
        fprintf (logfile, "----> type = %d\n", tokens[i].type);
        if (tokens[i].type == TYPE_ID)
        {
            fprintf (logfile, "----> id   = %s\n", tokens[i].u1.id);
        }
        else if (tokens[i].type == TYPE_SIN || tokens[i].type == TYPE_COS ||
                tokens[i].type == TYPE_LN ||  tokens[i].type == TYPE_PRINT ||
                tokens[i].type == TYPE_SCAN || tokens[i].type == TYPE_SQRT) 
        {
            fprintf (logfile, "----> id   = %s\n", tokens[i].u1.id);
        }
        else if (tokens[i].type == TYPE_NUM)
        {
            fprintf (logfile, "----> val  = %d\n", tokens[i].u1.value);
        }
        
    }
}



