#include "tree.h"
#include "graphDumpTree.h"
#include "myStrcmp.h"

bool IS_MAIN = false;

//-------------------------------------------------------------building a tree-------------------------------------------------------

node_t * createNodeWithNum (elem_t num)
{
	node_t * node = (node_t *) calloc (1, sizeof(node_t));
	MY_ASSERT (node == nullptr, "Unable to allocate new memory");

	node->isNum = true;
	node->elem = num;

	return node;
}

node_t * createNodeWithOperation (enum operation oper, node_t * nodeL, node_t * nodeR)
{
	MY_ASSERT (nodeL == nullptr, "There is no access to the left node");
	MY_ASSERT (nodeR == nullptr, "There is no access to the right node");

	node_t * node = (node_t *) calloc (1, sizeof(node_t));
	MY_ASSERT (node == nullptr, "Unable to allocate new memory");

	if (oper != OP_SUB			&&
		oper != OP_ADD			&&
		oper != OP_DIV			&&
		oper != OP_MUL			&&
		oper != OP_DEG			&&
		oper != OP_ASSIGN		&&
		oper != OP_LOG_AND		&&
		oper != OP_LOG_OR 		&&
		oper != OP_LESS_OR_EQ 	&&
		oper != OP_GR_OR_EQ		&&
		oper != OP_CELEBRATION	&&
		oper != OP_NOT_EQUAL	&&
		oper != OP_DENIAL)
	{
		MY_ASSERT (1, "Incorrect operation type specified");
	}

	node->left = nodeL;
	node->right = nodeR;
	nodeL->parent = node;
	nodeR->parent = node;
	node->op_t = oper;
	
	return node;
}

node_t * createKeyNode (enum keyword type, node_t * nodeL, node_t * nodeR)
{
	node_t * node = (node_t *) calloc (1, sizeof(node_t));
	MY_ASSERT (node == nullptr, "Unable to allocate new memory");

	node->key_t = type;

	if (nodeR != nullptr)
	{
		node->right = nodeR;
		nodeR->parent = node;
	}

	if (nodeL != nullptr)
	{
		node->left = nodeL;
		nodeL->parent = node;
	}

	return node;
}

node_t * createNodeWithVariable (char * variableName)
{
	MY_ASSERT (variableName == nullptr, "There is no access to the variable name");
	node_t * node = (node_t *) calloc (1, sizeof(node_t));
	MY_ASSERT (node == nullptr, "Unable to allocate new memory");

	node->id_t = ID_VAR;
	node->name = variableName;

	return node;
}

node_t * createNodeWithFunction (char * funcName)
{
	MY_ASSERT (funcName == nullptr, "There is no access to the variable name");
	node_t * node = (node_t *) calloc (1, sizeof(node_t));
	MY_ASSERT (node == nullptr, "Unable to allocate new memory");

	node->id_t = ID_FUNC;
	node->name = funcName;

	if ((strcmp (funcName, "main") == 0))
	{
		MY_ASSERT (IS_MAIN == true, "There should be only one main function");
		node->key_t = MAIN_T;
		IS_MAIN = true;
	}

	return node;
}

node_t * copyNode (node_t * nodeForCopy)
{
	MY_ASSERT (nodeForCopy == nullptr, "There is no access to node");
	node_t * newNode = (node_t *) calloc (1, sizeof (node_t));
	MY_ASSERT (newNode == nullptr, "Unable to allocate new memory");
	*newNode = *nodeForCopy;

	if (nodeForCopy->left != nullptr)
	{
		node_t * leftNode = copyNode (nodeForCopy->left);
		newNode->left = leftNode;
		leftNode->parent = newNode;
	}

	if (nodeForCopy->right != nullptr)
	{
		node_t * rightNode = copyNode (nodeForCopy->right);
		newNode->right = rightNode;
		rightNode->parent = newNode;
	}

	return newNode;
}

void deleteTree (node_t * node)
{
    MY_ASSERT (node == nullptr, "There is no access to this tree");

    if (node->left != nullptr)
    {
        deleteTree (node->left);
    }
    if (node->right != nullptr)
    {
        deleteTree (node->right);
    }

    free(node);
}

void deleteNode (node_t * node)
{
    MY_ASSERT (node == nullptr, "There is no access to the node");
    free (node);
}
//----------------------------------------------------------------------------------------------------------------------------------

