// 0410-BTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

using namespace std;

#define MAX 4
#define MIN 2

struct BTreeNode
{
	int value[MAX + 1];
	int count;
	BTreeNode* child[MAX + 1];
};

class BTree
{
	//BTreeNode* root;
	
public:
	BTreeNode* root;
	
	BTree()
	{
		root = NULL;
	}
	
	BTreeNode* createNode(int value, BTreeNode* child)
	{
		BTreeNode* newnode = new BTreeNode;
		newnode->value[1] = value;
		newnode->count = 1;
		newnode->child[0] = root;
		newnode->child[1] = child;
		return newnode;
	}

	void addValueToNode(int value, int position, BTreeNode* node, BTreeNode* child)
	{
		int j = node->count;
		while (j > position)
		{
			node->value[j + 1] = node->value[j];
			node->child[j + 1] = node->child[j];
			j--;
		}
		node->value[j + 1] = value;
		node->child[j + 1] = child;
		node->count++;
	}

	void splitNode(int value, int* pvalue, int position, 
		BTreeNode* node, BTreeNode* child, BTreeNode** newnode)
	{
		int median, j;

		if (position > MIN)
			median = MIN + 1;
		else
			median = MIN;

		*newnode = new BTreeNode;
		j = median + 1;
		while (j <= MAX)
		{
			(*newnode)->value[j - median] = node->value[j];
			(*newnode)->child[j - median] = node->child[j];
			j++;
		}
		node->count = median;
		(*newnode)->count = MAX - median;

		if (position <= MIN)
		{
			addValueToNode(value, position, node, child);
		}
		else
		{
			addValueToNode(value, position - median, *newnode, child);
		}
		*pvalue = node->value[node->count];
		(*newnode)->child[0] = node->child[node->count];
		node->count--;
	}

	int setValueIntoNode(int value, int* pval, BTreeNode* node, BTreeNode** child)
	{
		int position;
		if (!node)
		{
			*pval = value;
			*child = NULL;
			return 1;
		}

		if (value < node->value[1])
		{
			position = 0;
		}
		else
		{
			for (position = node->count;
				(value < node->value[position] && position > 1); position--);
			if (value == node->value[position])
			{
				cout << "Duplicates not allowed" << endl;
				return 0;
			}
		}
		if (setValueIntoNode(value, pval, node->child[position], child))
		{
			if (node->count < MAX)
			{
				addValueToNode(*pval, position, node, *child);
			}
			else
			{
				splitNode(*pval, pval, position, node, *child, child);
				return 1;
			}
		}
		return 0;
	}

	void insertValueInBTree(int value)
	{
		int flag, i;
		BTreeNode* child;

		flag = setValueIntoNode(value, &i, root, &child);
		if (flag)
			root = createNode(i, child);
	}

	// 要删除的值的复制继承者 copy successor for the value to be deleted
	/*
		B树中删除一个节点时，如果节点既有左子树又有右子树，且无法直接被删除，需要找到该节点的中序遍历的后继节点并用其替换当前节点。
		这段代码的作用是用来寻找后继节点，并将后继节点的值复制到需要删除的节点上，从而实现删除节点的操作
		
		When deleting a node in a B-tree, if the node has both left and right subtrees and cannot be deleted directly, 
		you need to find the successor node of the node in the mid-order traversal and replace the current node with it.
		The purpose of this code is to find the successor node and copy the value of the successor node to the node to be deleted, 
		so as to achieve the operation of deleting the node
	*/
	void copySuccessor(BTreeNode* myNode, int position)
	{
		BTreeNode* dummy;
		dummy = myNode->child[position];

		for (; dummy->child[0] != NULL;)
			dummy = dummy->child[0];
		myNode->value[position] = dummy->value[1];
	}

	void removeValue(BTreeNode* myNode, int position)
	{
		int i = position + 1;
		while (i <= myNode->count)
		{
			myNode->value[i - 1] = myNode->value[i];
			myNode->child[i - 1] = myNode->child[i];
			i++;
		}
		myNode->count--;
	}

	// 调整节点
	void rightShift(BTreeNode* myNode, int position)
	{
		BTreeNode* x = myNode->child[position];
		int j = x->count;

		while (j > 0)
		{
			x->value[j + 1] = x->value[j];
			x->child[j + 1] = x->child[j];
		}
		x->value[1] = myNode->value[position];
		x->child[1] = x->child[0];
		x->count++;

		x = myNode->child[position - 1];
		myNode->value[position] = x->value[x->count];
		myNode->child[position]->child[0] = x->child[x->count];
		x->count--;
		return;
	}

	// 向左移动
	void leftShift(BTreeNode* myNode, int position)
	{
		int j = 1;
		BTreeNode* x = myNode->child[position - 1];

		x->count++;
		x->value[x->count] = myNode->value[position];
		x->child[x->count] = myNode->child[position]->child[0];

		x = myNode->child[position];
		myNode->value[position] = x->value[1];
		x->child[0] = x->child[1];
		x->count--;

		while (j <= x->count)
		{
			x->value[j] = x->value[j + 1];
			x->child[j] = x->child[j + 1];
			j++;
		}
		return;
	}

	// 合并节点 merge nodes
	void mergeLeaves(BTreeNode* myNode, int position)
	{
		int j = 1;
		BTreeNode* x1 = myNode->child[position], * x2 = myNode->child[position - 1];

		x2->count++;
		x2->value[x2->count] = myNode->value[position];
		x2->child[x2->count] = x1->child[0];

		while (j <= x1->count)
		{
			x2->count++;
			x2->value[x2->count] = x1->value[j];
			x2->child[x2->count] = x1->child[j];
			j++;
		}

		j = position;
		while (j < myNode->count)
		{
			myNode->value[j] = myNode->value[j + 1];
			myNode->child[j] = myNode->child[j + 1];
			j++;
		}
		myNode->count--;
		free(x1);
	}

	// 调整节点 adjust the node
	void adjustNode(BTreeNode* myNode, int position)
	{
		if (!position)
		{
			if (myNode->child[1]->count > MIN)
			{
				leftShift(myNode, 1);
			}
			else
			{
				mergeLeaves(myNode, 1);
			}
		}
		else
		{
			if (myNode->count != position)
			{
				if (myNode->child[position - 1]->count > MIN)
				{
					rightShift(myNode, position);
				}
				else
				{
					if (myNode->child[position + 1]->count > MIN)
					{
						leftShift(myNode, position + 1);
					}
					else
					{
						mergeLeaves(myNode, position);
					}
				}
			}
			else
			{
				if (myNode->child[position - 1]->count > MIN)
					rightShift(myNode, position);
				else
					mergeLeaves(myNode, position);
			}
		}
	}

	int deleteValueFromNode(int value, BTreeNode* myNode)
	{
		int position, i = 0;
		if (myNode)
		{
			if (value < myNode->value[1])
			{
				position = 0;
				i = 0;
			}
			else
			{
				for (position = myNode->count;
					(value < myNode->value[position] && position > 1);
					position--);
				if (value == myNode->value[position])
				{
					i = 1;
				}
				else {
					i = 0;
				}
			}
			
			if (i)
			{
				if (myNode->child[position - 1])
				{
					copySuccessor(myNode, position);
					i = deleteValueFromNode(myNode->value[position], myNode->child[position]);
					if (!i)
					{
						cout << "The value " << value << " is not present in the B-Tree" << endl;
					}
					else
					{
						if (myNode->child[position]->count < MIN)
							adjustNode(myNode, position);
					}
				}
				else
				{
					removeValue(myNode, position);
				}
			}
			else
			{
				i = deleteValueFromNode(value, myNode->child[position]);
				if (myNode->value[position])
				{
					if (myNode->child[position]->count < MIN)
						adjustNode(myNode, position);
				}
			}
		}
		return i;
	}

	//从B树删除某个value
	void deleteValueFromBTree(int value, BTreeNode* myNode)
	{
		BTreeNode* temp;
		if (!deleteValueFromNode(value, myNode))
		{
			cout << "The value " << value << " is not present in the B-Tree" << endl;
			return;
		}
		else
		{
			if (myNode->count == 0)
			{
				temp = myNode;
				myNode = myNode->child[0];
				free(temp);
			}
			root = myNode;
		}

	}

	void searchValueInBTree(int value, int* position, BTreeNode* myNode)
	{
		if (!myNode)
		{
			return;
		}

		if (value < myNode->value[1])
		{
			*position = 0;
		}
		else
		{
			for (*position = myNode->count;
				(value < myNode->value[*position] && *position > 1);
				(*position)--);

				if (value == myNode->value[*position])
				{
					cout << "The value " << value << " is present in the B-Tree" << endl;
					cout << endl;
					return;
				}
				
				cout << "The value " << value << " is not present in the B-Tree" << endl << endl;

		}
		searchValueInBTree(value, position, myNode->child[*position]);
		return;
	}

	// 遍历B树 Traversal == 中序遍历 Midorder traversal
	void traversal(BTreeNode* myNode)
	{
		int i;
		if (myNode)
		{
			for (i = 0; i < myNode->count; i++)
			{
				traversal(myNode->child[i]);
				cout << myNode->value[i + 1] << " ";
			}
			traversal(myNode->child[i]);
		}
	}

	// 先序遍历 Preorder traversal
	void preorder(BTreeNode* myNode)
	{
		int i;
		if (myNode)
		{
			for (i = 0; i < myNode->count; i++)
			{
				cout << myNode->value[i + 1] << " ";
				preorder(myNode->child[i]);
			}
			preorder(myNode->child[i]);
		}
	}

	// 后序遍历 Postorder traversal
	void postorder(BTreeNode* myNode)
	{
		int i;
		if (myNode)
		{
			for (i = 0; i < myNode->count; i++)
			{
				postorder(myNode->child[i]);
			}
			postorder(myNode->child[i]);
			for (i = 0; i < myNode->count; i++)
			{
				cout << myNode->value[i + 1] << " ";
			}
		}
	}
};

int main()
{
	int value, option;
	BTree myBTree;
	while (true)
	{
		cout << "Enter your choice" << endl;
		cout << "1. Insert" << endl;
		cout << "2. Delete" << endl;
		cout << "3. Search" << endl;
		cout << "4. Traversal" << endl;
		cout << "5. Preorder Traversal" << endl;
		cout << "6. Postorder Traversal" << endl;
		cout << "7. Exit" << endl;
		cout << string(30, '=') << endl << "Your choice: ";
		cin >> option;
		cout << endl;
		switch (option)
		{
		case 1:
			cout << "Enter the value to be inserted: ";
			cin >> value;
			cout << endl;
			myBTree.insertValueInBTree(value);
			break;
		case 2:
			cout << "Enter the value to be deleted: ";
			cin >> value;
			cout << endl;
			myBTree.deleteValueFromBTree(value, myBTree.root);
			break;
		case 3:
			cout << "Enter the value to be searched: " ;
			cin >> value;
			cout << endl;
			myBTree.searchValueInBTree(value, &option, myBTree.root);
			break;
		case 4:
			cout << "Traversal of the B-Tree is: ";
			myBTree.traversal(myBTree.root);
			cout << endl << endl;
			break;
		case 5:
			cout << "Preorder Traversal of the B-Tree is: ";
			myBTree.preorder(myBTree.root);
			cout << endl << endl;
			break;
		case 6:
			cout << "Postorder Traversal of the B-Tree is: ";
			myBTree.postorder(myBTree.root);
			cout << endl << endl;
			break;
		default:
			exit(1);
		}
	}
}

