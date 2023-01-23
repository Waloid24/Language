#include <stdio.h>
#include "MY_ASSERT.h"
#include "tree.h"

void _nodeInfo_ (node_t * node, int line, const char * file);
void _dumpCode_ (char * code, int line, const char * file);

void _nodeInfo_ (node_t * node, int line, const char * file)
{
    MY_ASSERT (node == nullptr, "There is no access to this node");
    MY_ASSERT (file == nullptr, "There is no access to this file");

    if (node->id_t == ID_FUNC)
    {
        printf ("ID_T: %d (func)\n", node->id_t);
        printf ("NAME: %s\n", node->name);
    }
    if (node->id_t == ID_VAR)
    {
        printf ("ID_T: %d (var)\n", node->id_t);
        printf ("NAME: %s\n", node->name);
    }

    if (node->name == nullptr && node->isNum == false)
    {
        printf ("TYPE: %s\n", node->supportName);
    }
    
    if (node->isNum == true)
    {
        printf ("NUM: %lf\n", node->elem);
    }
    printf ("line: %d, file = %s\n\n", line, file);
}

void _dumpCode_ (char * code, int line, const char * file)
{
    for (int i = 0; *code != '\0' && i < 20; i++, code++)
    {
        printf ("%c", *code);
    }
    printf ("\n");
    printf ("line: %d, file = %s\n\n", line, file);
}