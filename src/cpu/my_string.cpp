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

int readNum (char * src, int * dst)
{
    MY_ASSERT (src == nullptr, "There is no access to source string");
    MY_ASSERT (dst == nullptr, "There is no access to destination string");
    char sym = 0;
    char * tmp = src;

    while (48 <= *tmp && *tmp <= 57)
    {
        tmp++;
        sym++;
    }
    *dst = atoi(src);
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
