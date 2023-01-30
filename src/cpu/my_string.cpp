#include "my_string.h"

int myStrcmp (const char * string1, const char * string2)
{
	int i = 0, j = 0;
	for (; string1[i] != '\0' && string2[j] != '\0'; i++, j++)
	{
		while (!isalpha(string1[i]) && string1[i] != '\0')
			i++;
		while (!isalpha(string2[j]) && string2[j] != '\0')
			j++;
		if (tolower(string1[i]) == tolower(string2[j]))
			continue;
		return (tolower(string1[i]) - tolower(string2[j]));
	}

	while (!isalpha(string1[i]) && string1[i] != '\0')
			i++;

	while (!isalpha(string2[j]) && string2[j] != '\0')
			j++;

	return (tolower(string1[i]) - tolower(string2[j]));
}

char readNum (char * src, double * doubleNumDst, int * intNumDst)
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

void skipSpace (char ** strCode, int countLetters)
{
    *strCode = *strCode + countLetters;
	int i = 0;
    for (; **strCode == ' ' || **strCode == '\t'; i++)
    {
        (*strCode)++;
    }
}
