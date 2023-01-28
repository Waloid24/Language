#include "textTree.h"

#define print(text, ...)\
		fprintf (textDump, text, ##__VA_ARGS__)

#define printId(text, ...)\
		fprintf (textDump, "\"%s\"", text, ##__VA_ARGS__)

static void dumpTree (node_t * node, FILE * textDump);
static FILE * createFile (char * name);

void textTree (node_t * node, char * name)
{
    MY_ASSERT (node == nullptr, "There is no access to the tree");

    FILE * nameFile = createFile (name);

    dumpTree (node, nameFile);

    fclose (nameFile);
}   

static FILE * createFile (char * name)
{
    MY_ASSERT (name == nullptr, "There is no access to the file name");

    FILE * file = fopen (name, "w");
	MY_ASSERT (file == nullptr, "Unable to open the file for graphic dump");
    setbuf (file, NULL);

    return file;
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
        print ("%lf", node->elem);
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

