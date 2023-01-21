#include "tree.h"
#include "graphDumpTree.h"
#include "myStrcmp.h"

//-------------------------------------------------------------building a tree-------------------------------------------------------

node_t * createNodeWithNum (elem_t num)
{
	node_t * node = (node_t *) calloc (1, sizeof(node_t));
	MY_ASSERT (node == nullptr, "Unable to allocate new memory");

	node->type = NUM_T;
	node->elem = num;

	return node;
}

node_t * createNodeWithOperation (int operation, node_t * nodeL, node_t * nodeR) //enum operationType operation
{
	MY_ASSERT (nodeL == nullptr, "There is no access to the left node");
	MY_ASSERT (nodeR == nullptr, "There is no access to the right node");

	node_t * node = (node_t *) calloc (1, sizeof(node_t));
	MY_ASSERT (node == nullptr, "Unable to allocate new memory");

	if (operation != OP_SUB 		&&
		operation != OP_ADD 		&&
		operation != OP_DIV 		&&
		operation != OP_MUL 		&&
		operation != OP_DEG 		&&
		operation != OP_ASSIGN 		&&
		operation != OP_LOG_AND 	&&
		operation != OP_LOG_OR 		&&
		operation != OP_LESS_OR_EQ 	&&
		operation != OP_GR_OR_EQ)
	{
		MY_ASSERT (1, "Incorrect operation type specified");
	}

	if (operation == OP_ASSIGN)
	{
		node->type = OPER_T;
		node->op_t = OP_ASSIGN;
		node->left = nodeL;
		nodeL->parent = node;
		node->right = nodeR;
		nodeR->parent = node;
	}
	else
	{
		node->left = nodeL;
		node->right = nodeR;
		node->type = OPER_T;
		node->op_t = (operationType) operation;

		nodeL->parent = node;
		nodeR->parent = node;
	}
	

	return node;
}

node_t * createNodeInstruction (enum nodeType type, node_t * nodeL, node_t * nodeR)
{
	node_t * node = (node_t *) calloc (1, sizeof(node_t));
	MY_ASSERT (node == nullptr, "Unable to allocate new memory");

	node->type = type;

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

node_t * createTerminalNode (enum nodeType type, node_t * condition, node_t * nodeL, node_t * nodeR)
{
	node_t * node = (node_t *) calloc (1, sizeof(node_t));
	MY_ASSERT (node == nullptr, "Unable to allocate new memory");

	node->type = type;

	if (type == IF_T) //TODO: delete copypaste
	{
		MY_ASSERT (condition == nullptr, "There is no access to condition in if");
		node_t * actions = createNodeInstruction (DECISION_T, nodeL, nodeR); 

		node->left = condition;
		condition->parent = node;
		
		node->right = actions;
		actions->parent = node;
	}
	else if (type == WHILE_T)
	{
		MY_ASSERT (condition == nullptr, "There is no access to condition in while");

		node->left = condition;
		condition->parent = node;
		
		node->right = nodeR;
		nodeR->parent = node;
	}

	return node;
}

node_t * createNodeWithVariable (char * variableName)
{
	MY_ASSERT (variableName == nullptr, "There is no access to the variable name");
	node_t * node = (node_t *) calloc (1, sizeof(node_t));
	MY_ASSERT (node == nullptr, "Unable to allocate new memory");

	node->type = VAR_T;
	node->varName = variableName;

	return node;
}

node_t * createNodeWithFunction (const char * nameFunction, node_t * leftDescendant)
{
	if ((myStrcmp (nameFunction, "sin") != 0) && (myStrcmp (nameFunction, "cos") != 0) &&
		(myStrcmp (nameFunction, "tg") != 0) && (myStrcmp (nameFunction, "ctg") != 0) &&
		(myStrcmp (nameFunction, "ln") != 0))
	{
		MY_ASSERT (1, "Incorrect function name");
		return nullptr;
	}

	node_t * node = (node_t *) calloc (1, sizeof(node_t));
	MY_ASSERT (node == nullptr, "Unable to allocate new memory");

	node->type = FUNC_T;
	node->nameFunc = nameFunction;
	node->left = leftDescendant;
	leftDescendant->parent = node;
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

