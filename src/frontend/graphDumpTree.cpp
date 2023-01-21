#include "graphDumpTree.h"

#define dumplineTree(text, ...)\
		fprintf (graphicDump, text, ##__VA_ARGS__)

//--------------------------------------------graphical tree dump-------------------------------------------------
static void dotFileHeaderForTree (const node_t * node, const char * nameDotFileTree);
static void writeNodeToDotFile (const node_t * node, FILE * graphicDump);
static void writeEdgeToDotFile (const node_t * node, FILE * graphicDump);
static void createDotFileTree (const char * nameDotFile, unsigned int timesCreatePicture);
static void createHtmlFileTree(const char * nameFileDump, unsigned int * timesCreatePicture);

static unsigned int	NUMBER_GRAPHICAL_TREE_DUMPS = 0;
//----------------------------------------------------------------------------------------------------------------

//--------------------------------------------graphical tree dump-------------------------------------------------

void graphicDumpTree (const node_t * node)
{
	MY_ASSERT (node == nullptr, "There is no access to this node");

    const char * dotFileTree = "treeGraphviz.dot";
	const char * htmlFileTree = "treeGraphviz.html";

	dotFileHeaderForTree	(node, dotFileTree);
	createDotFileTree		(dotFileTree, NUMBER_GRAPHICAL_TREE_DUMPS);
	createHtmlFileTree		(htmlFileTree, &NUMBER_GRAPHICAL_TREE_DUMPS);
}

void dotFileHeaderForTree (const node_t * node, const char * nameDotFileTree)
{
	MY_ASSERT (node == nullptr, "There is no access to tree");

	FILE * graphicDump = fopen (nameDotFileTree, "w");
	MY_ASSERT (graphicDump == nullptr, "Unable to open the file for graphic dump");

	dumplineTree ("digraph G{\n");
	dumplineTree ("  graph [dpi = 100];\n\n");
	dumplineTree ("  ranksep = 1.5;\n\n");
	dumplineTree ("  splines = ortho;\n\n");

    dumplineTree ("  node[shape = record];\n\n");

	writeNodeToDotFile (node, graphicDump);
	writeEdgeToDotFile (node, graphicDump);

	dumplineTree ("}\n");

	fflush (graphicDump);
	fclose (graphicDump);
}

void writeNodeToDotFile (const node_t * node, FILE * graphicDump)
{
	MY_ASSERT (node == nullptr, "There is no access to the node");
	MY_ASSERT (graphicDump == nullptr, "There is no access to the file for dump");

	if (node->type == OPER_T)
	{
		if (node->op_t == OP_GR_OR_EQ)
		{
			dumplineTree ("\t node%p [label=\"%s\", shape=oval];\n", node, ">=");
		}
		else if (node->op_t == OP_LESS_OR_EQ)
		{
			dumplineTree ("\t node%p [label=\"%s\", shape=oval];\n", node, "<=");
		}
		else if (node->op_t == OP_LOG_AND)
		{
			dumplineTree ("\t node%p [label=\"%s\", shape=oval];\n", node, "&&");
		}
		else if (node->op_t == OP_LOG_OR)
		{
			dumplineTree ("\t node%p [label=\"%s\", shape=oval];\n", node, "||");
		}
		else
		{
			dumplineTree ("\t node%p [label=\"%c\", shape=oval];\n", node, node->op_t);
		}
	}
	else if (node->type == VAR_T || node->type == CONST_T)
	{
		dumplineTree ("\t node%p [label=\"%s\", penwidth=3, shape=rect, color=darkslategray];\n", node, node->varName);
	}
	else if (node->type == STATEMENT_T)
	{
		dumplineTree ("\t node%p [label=\"%s\", shape=parallelogram, style=filled, fillcolor=silver];\n", node, "statement");
	}
	else if (node->type == DECISION_T)
	{
		dumplineTree ("\t node%p [label=\"%s\", shape=parallelogram, style=filled, fillcolor=silver];\n", node, "decision");
	}
	else if (node->type == WHILE_T)
	{
		dumplineTree ("\t node%p [label=\"%s\", shape=parallelogram, style=filled, fillcolor=silver];\n", node, "while");
	}
	else if (node->type == IF_T)
	{
		dumplineTree ("\t node%p [label=\"%s\", shape=parallelogram, style=filled, fillcolor=silver];\n", node, "if");
	}
	else if (node->type == NUM_T)
	{
		dumplineTree ("\t node%p [label=\"%lf\", penwidth=3, shape=rect, color=darkgreen];\n", node, node->elem);
	}
	else
	{
		dumplineTree ("\t node%p [label=\"{ %s | %p }\"];\n", node, node->nameFunc, node);
	}

	if (node->left != nullptr)
	{
		writeNodeToDotFile (node->left, graphicDump);
	}
	if (node->right != nullptr)
	{
		writeNodeToDotFile (node->right, graphicDump);
	}
}

void writeEdgeToDotFile (const node_t * node, FILE * graphicDump)
{
	MY_ASSERT (node == nullptr, "There is no access to the node");
	MY_ASSERT (graphicDump == nullptr, "There is no access to the file for dump");

	if (node->left != nullptr)
	{
		dumplineTree ("\t node%p -> node%p [color = blueviolet];\n", node, node->left);
		writeEdgeToDotFile (node->left, graphicDump);
	}
	if (node->right != nullptr)
	{
		dumplineTree ("\t node%p -> node%p [color = darkgoldenrod1];\n", node, node->right);
		writeEdgeToDotFile (node->right, graphicDump);
	}
}

void createDotFileTree (const char * nameDotFile, unsigned int timesCreatePicture)
{
	MY_ASSERT (nameDotFile == nullptr, "There is no access to name file for graphic dump");

	char buf[100] = {};
    sprintf(buf, "dot -T png -o graph%u.png %s", timesCreatePicture, nameDotFile);
    system(buf);
}

static void createHtmlFileTree(const char * nameFileDump, unsigned int * timesCreatePicture)
{
	FILE * treeHTML = fopen (nameFileDump, "a");
	MY_ASSERT (treeHTML == nullptr, "Unable to open the file for html-dump");

	char namePicture[100] = {};
    sprintf (namePicture, "graph%u.png", *timesCreatePicture);

	printf ("namePicture = %s\n", namePicture);

	fprintf (treeHTML, "<img src=\"%s\" alt=\"dump РІвЂћвЂ“%u\">\n", namePicture, *timesCreatePicture);
	fprintf (treeHTML, "<hr>\n\n");

    (*timesCreatePicture)++;

    fflush (treeHTML);
    fclose (treeHTML);
}

//---------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------------