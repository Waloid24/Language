#ifndef MY_STRING_H
#define MY_STRING_H

#include <ctype.h>
#include "MY_ASSERT.h"

int myStrcmp (const char * string1, const char * string2);
char readNum (char * src, double * doubleNumDst, int * intNumDst);
void skipSpace (char ** strCode, int countLetters);

#endif