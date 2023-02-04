#include "readCode.h"

static size_t size (const char * arg_console);
static void writeTextToBuf (code_t * code, FILE * codeFile);
static size_t correctBuf (char * buf, size_t nElements);
static char ** arrPtrToStrings (size_t nStrs, size_t size);
static void fillArr (char ** arrPtrsToStrings, char * buf, size_t nElem, code_t code);
static void skipSpace (char ** strCode, int countLetters);

code_t readCode (const char * nameFile)
{
    MY_ASSERT (nameFile == nullptr, "You did not pass the file name");
    code_t fileInfo = {};
    fileInfo.sizeFile = size (nameFile);

    FILE * codeFile = openFile (nameFile, "r");

    writeTextToBuf (&fileInfo, codeFile);
    fileInfo.nStrs = correctBuf (fileInfo.buf, fileInfo.sizeFile);

    fileInfo.arrStrs = (char **) allocateMemory (fileInfo.nStrs, sizeof(char *));
    fillArr (fileInfo.arrStrs, fileInfo.buf, fileInfo.sizeFile, fileInfo);

    fclose (codeFile);

    return fileInfo;
}

static size_t size (const char * argConsole)
{
    struct stat code = {};
    stat (argConsole, &code);

    return (size_t) code.st_size;
}

static void writeTextToBuf (code_t * code, FILE * codeFile)
{
    MY_ASSERT (code == nullptr, "There is no access to the struct with the necessary information");
    MY_ASSERT (codeFile == nullptr, "There is no access to the code file");

    code->buf = (char *) calloc (code->sizeFile + 1, sizeof(char));
    MY_ASSERT (code->buf == nullptr, "Unable to allocate new memory");

    fread (code->buf, sizeof(char), code->sizeFile,  codeFile);
    (code->buf)[code->sizeFile] = '\0';
}

static size_t correctBuf (char * buf, size_t nElements) 
{
	size_t nStrings = 0;

	for (size_t i = 0; i < nElements; i++) 
	{   
        if (buf[i] == ';')
        {
            while (buf[i] != '\n')
            {
                buf[i] = '\0';
                i++;
            }
            buf[i] = '\0'; 
            nStrings++; 
        }

		if (buf[i] == '\n')
		{
			buf[i] = '\0';
		}   
	}
	return nStrings;
}

static char ** arrPtrToStrings (size_t nStrs, size_t size)
{
	char ** array = (char **) calloc (nStrs, size);
	MY_ASSERT (array == NULL, "Memory allocation error\n");
	return array;
}

static void fillArr (char ** arrPtrsToStrings, char * buf, size_t nElem, code_t code) 
{
    skipSpace (&buf, 0);
	arrPtrsToStrings[0] = buf;
	for (size_t nSym = 0, j = 1; nSym < nElem && j < code.nStrs && *buf != EOF; nSym++)
	{
		if (*buf == '\0')
		{
            while (*buf == '\0')
            {
                buf++;
            }
            skipSpace (&buf, 0);
			arrPtrsToStrings[j] = buf;
			j++;
		}
		buf++;
	}
}

static void skipSpace (char ** strCode, int countLetters) //TODO: remove clons
{
    *strCode = *strCode + countLetters;
	int i = 0;
    for (; **strCode == ' ' || **strCode == '\t' || **strCode == '\0'; i++)
    {
        (*strCode)++;
    }
}

FILE * openFile(const char * nameFile, const char * format)
{
    MY_ASSERT (nameFile == nullptr, "Wrong pointer to name file with source code");

    FILE *file = fopen (nameFile, format);
    MY_ASSERT (file == nullptr, "Unable to open the source file");
    setbuf (file, NULL);

    return file;
}

void * allocateMemory (size_t numElem, size_t sizeElem)
{
    void * ptr = calloc (numElem, sizeElem);
    MY_ASSERT (ptr == nullptr, "Unable to allocate new memory");
    return ptr;
}

char * getCode (const char * nameFile)
{
    FILE * fileCode = openFile (nameFile, "rb");
    size_t sizeArr = size (nameFile);
    char * code = (char *) allocateMemory (sizeArr + 1, sizeof(char));
    fread (code, sizeof (char), sizeArr, fileCode);
    code[sizeArr] = '\0';

    fclose (fileCode);

    return code;
}