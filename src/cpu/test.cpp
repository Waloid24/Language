#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MY_ASSERT.h"

const int SIGNATURE = 4;
const char VERSION = 1;

int checkBit(const int value, const int position) {
    return ((value & (1 << position)) != 0);
}

void printBinaryForm (int count)
{
    const int NUM_BITS = 8;
    printf ("count = %d\n", count);
    char * arrDigit = (char *) calloc (NUM_BITS, sizeof(char));
    MY_ASSERT (arrDigit == nullptr, "Unable to allocate new memory");

    for (int i = 0; i < NUM_BITS; i++)
    {
        if (checkBit(count, i))
        {
            arrDigit[i] = 1;
        }
        else
        {
            arrDigit[i] = 0;
        }
    }
    for (int i = NUM_BITS; i > 0; i--)
    {
        if (i == 4)
        {
            printf ("|");
        }
        printf ("%d", arrDigit[i-1]);
    }
    printf ("\n");
}

int main (void)
{
    const int MASK = (1<<4) + (1<<3) + (1<<2) + 1; //0001|1111
    printf ("MASK = %d\n", MASK);

    int num = 5;
    printBinaryForm (num);
}