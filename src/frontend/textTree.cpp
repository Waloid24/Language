#include "textTree.h"

#define print(text, ...)\
		fprintf (textDump, text, ##__VA_ARGS__)

#define printId(text, ...)\
		fprintf (textDump, "\"%s\"", text, ##__VA_ARGS__)

static void dumpTree (node_t * node, FILE * textDump);

void textTree (node_t * node, char * name)
{
    MY_ASSERT (node == nullptr, "There is no access to the tree");

    // char * path = "./bin/";
    // char * fullname = strcat (path, name);
    FILE * nameFile = openFile (name, "w");

    dumpTree (node, nameFile);

    fclose (nameFile);
}   

static void dumpTree (node_t * node, FILE * textDump)
{
    MY_ASSERT (node == nullptr, "There is no access to the node");
    
    if (node->name == nullptr && node->isNum == false)
    {
        print ("%s",node->supportName);
    }
    else if (node->name != nullptr && node->isNum == false)
    {
        print ("\"%s\"", node->name);
    }
    else if (node->isNum != false)
    {
        print ("%.0lf", node->elem);
    }
    else
    {
        MY_ASSERT (1, "Wrong node");
    }

    print ("(");
    if (node->left != nullptr)
    {
        dumpTree (node->left, textDump);
    }
    print (")");

    print ("(");
    if (node->right != nullptr)
    {
        dumpTree (node->right, textDump);
    }
    print (")");
}

