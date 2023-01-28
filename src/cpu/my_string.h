#ifndef MY_STRING_H
#define MY_STRING_H

#include <ctype.h>
#include "MY_ASSERT.h"

int myStrcmp (const char * string1, const char * string2);
int readNum (char * src, int * dst);
void skipSpace (char ** strCode, int countLetters);

#endif