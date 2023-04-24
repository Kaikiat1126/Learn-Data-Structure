#include <iostream>

using namespace std;

#define MAX 5
#define MIN 2

struct BPNode
{
	bool isLeaf;
	int keyNum;
	int key[MAX + 1];
	BPNode* child[MAX + 1];
	BPNode* parent;
	BPNode* nextLeaf;
};

class BPlusTree
{
public:
	BPlusTree();
	~BPlusTree();
	void insert(int key);
	void remove(int key);
	void printTree();
	void search(int key);
	void traversal();
	void preOrder();
	void inOrder();

private:
	BPNode* root;
	BPNode* headLeaf;
	BPNode* findLeaf(int key);
	BPNode* createNewNode(int key, BPNode* child);
	int getTreeHeight(BPNode* node);
	void printTree(BPNode* node, int height);
	void addValueToNode(int key, BPNode* node, BPNode* child);
	void splitNode(BPNode* node);
	void removeValueFromNode(int key, BPNode* node);
	void mergeNode(BPNode* node);
	void merge(BPNode* node, int index);
	void adjustNode(BPNode* node);
	void adjustRoot();
	void printNode(BPNode* node);
	void searchNode(int key, BPNode* node);
	void removeNode(int key, BPNode* node);
	void removeInternalNode(int key, BPNode* node);
	void rotate(BPNode* node, int index);
	void rotateLeft(BPNode* node, BPNode* leftChild, int index);
	void rotateRight(BPNode* node, BPNode* rightChild, int index);
	void traversal(BPNode* node);
	void preOrder(BPNode* node);
	void inOrder(BPNode* node);
};

BPlusTree::BPlusTree()
{
	root = NULL;
	headLeaf = NULL;
}

BPlusTree::~BPlusTree()
{
}

void BPlusTree::insert(int key)
{
	if (root == NULL)
	{
		root = createNewNode(key, NULL);
		headLeaf = root;
		return;
	}

	BPNode* leaf = findLeaf(key);
	addValueToNode(key, leaf, NULL);
}

void BPlusTree::remove(int key)
{
	if (root == NULL)
	{
		cout << "The tree is empty!" << endl;
		return;
	}
	BPNode* leaf = findLeaf(key);
	if (leaf == NULL)
	{
		cout << "The key is not in the tree!" << endl;
		return;
	}
	removeValueFromNode(key, leaf);
	if (leaf == root)
	{
		adjustRoot();
	}
	else
	{
		adjustNode(leaf);
	}
}

void BPlusTree::printTree()
{
	if (root == NULL)
	{
		cout << "The tree is empty!" << endl;
		return;
	}
	printTree(root, 0);
}

void BPlusTree::printTree(BPNode* node, int height)
{
	if (node == NULL)
	{
		return;
	}
	if (node->isLeaf)
	{
		printNode(node);
	}
	else
	{
		printTree(node->child[0], height + 1);
		for (int i = 0; i < height; i++)
		{
			cout << "    ";
		}
		printNode(node);
		for (int i = 0; i <= node->keyNum; i++)
		{
			printTree(node->child[i], height + 1);
		}
	}
}

void BPlusTree::search(int key)
{
	if (root == NULL)
	{
		cout << "The tree is empty!" << endl;
		return;
	}
	searchNode(key, root);
}

BPNode* BPlusTree::findLeaf(int key)
{
	BPNode* node = root;
	while (node->isLeaf == false)
	{
		int i = 0;
		while (i < node->keyNum)
		{
			if (key >= node->key[i])
			{
				i++;
			}
			else
			{
				break;
			}
		}
		node = node->child[i];
	}
	return node;
}

BPNode* BPlusTree::createNewNode(int key, BPNode* child)
{
	BPNode* newNode = new BPNode();
	newNode->isLeaf = true;
	newNode->keyNum = 1;
	newNode->key[0] = key;
	newNode->child[0] = child;
	newNode->child[MAX] = NULL;
	newNode->parent = NULL;
	newNode->nextLeaf = NULL;
	return newNode;
}

int BPlusTree::getTreeHeight(BPNode* node)
{
	int height = 0;
	while (node != NULL)
	{
		node = node->child[0];
		height++;
	}
	return height;
}

void BPlusTree::addValueToNode(int key, BPNode* node, BPNode* child)
{
	int i = node->keyNum - 1;
	while (i >= 0 && key < node->key[i])
	{
		node->key[i + 1] = node->key[i];
		node->child[i + 2] = node->child[i + 1];
		i--;
	}
	node->key[i + 1] = key;
	node->child[i + 2] = child;
	node->keyNum++;
}

void BPlusTree::splitNode(BPNode* node)
{
	int i = (MAX + 1) / 2;
	int j = 0;
	BPNode* newNode = new BPNode();
	newNode->isLeaf = true;
	newNode->nextLeaf = node->nextLeaf;
	node->nextLeaf = newNode;
	while (i < MAX + 1)
	{
		newNode->key[j] = node->key[i];
		newNode->child[j] = node->child[i];
		i++;
		j++;
	}
	newNode->keyNum = j;
	node->keyNum = (MAX + 1) / 2;
	newNode->parent = node->parent;
	addValueToNode(newNode->key[0], node->parent, newNode);
}

void BPlusTree::removeValueFromNode(int key, BPNode* node)
{
	int i = 0;
	while (i < node->keyNum && node->key[i] != key)
	{
		i++;
	}
	if (i == node->keyNum)
	{
		cout << "The key is not in the tree!" << endl;
		return;
	}
	while (i < node->keyNum - 1)
	{
		node->key[i] = node->key[i + 1];
		node->child[i] = node->child[i + 1];
		i++;
	}
	node->keyNum--;
}

void BPlusTree::adjustRoot()
{
	if (root->keyNum == 0)
	{
		BPNode* newRoot = root->child[0];
		delete root;
		root = newRoot;
		root->parent = NULL;
	}
}

/*
void BPlusTree::adjustRoot()
{
	if (root->keyNum == 0)
	{
		BPNode* temp = root;
		root = root->child[0];
		delete temp;
	}
}
*/

void BPlusTree::adjustNode(BPNode* node)
{
	if (node->keyNum >= MIN)
	{
		return;
	}
	if (node->parent->child[0] != node)
	{
		BPNode* leftNode = node->parent->child[0];
		int i = 0;
		while (leftNode->child[i] != node)
		{
			i++;
		}
		if (leftNode->keyNum > MIN)
		{
			int j = node->keyNum;
			while (j > 0)
			{
				node->key[j] = node->key[j - 1];
				node->child[j] = node->child[j - 1];
				j--;
			}
			node->key[0] = leftNode->key[leftNode->keyNum - 1];
			node->child[0] = leftNode->child[leftNode->keyNum];
			node->keyNum++;
			leftNode->keyNum--;
			node->parent->key[i - 1] = node->key[0];
			return;
		}
	}
	if (node->parent->child[node->parent->keyNum] != node)
	{
		BPNode* rightNode = node->parent->child[node->parent->keyNum];
		int i = 0;
		while (node->parent->child[i] != node)
		{
			i++;
		}
		if (rightNode->keyNum > MIN)
		{
			node->key[node->keyNum] = rightNode->key[0];
			node->child[node->keyNum] = rightNode->child[0];
			node->keyNum++;
			int j = 0;
			while (j < rightNode->keyNum - 1)
			{
				rightNode->key[j] = rightNode->key[j + 1];
				rightNode->child[j] = rightNode->child[j + 1];
				j++;
			}
			rightNode->keyNum--;
			node->parent->key[i] = rightNode->key[0];
			return;
		}
	}
	if (node->parent->child[0] != node)
	{
		BPNode* leftNode = node->parent->child[0];
		int i = 0;
		while (leftNode->child[i] != node)
		{
			i++;
		}
		leftNode->key[leftNode->keyNum] = node->parent->key[i - 1];
		leftNode->child[leftNode->keyNum + 1] = node->child[0];
		leftNode->keyNum++;
		int j = 0;
		while (j < node->keyNum)
		{
			leftNode->key[leftNode->keyNum] = node->key[j];
			leftNode->child[leftNode->keyNum + 1] = node->child[j + 1];
			leftNode->keyNum++;
			j++;
		}
		removeValueFromNode(node->parent->key[i - 1], node->parent);
		delete node;
		return;
	}

	if (node->parent->child[node->parent->keyNum] != node)
	{
		BPNode* rightNode = node->parent->child[node->parent->keyNum];
		int i = 0;
		while (node->parent->child[i] != node)
		{
			i++;
		}
		node->key[node->keyNum] = node->parent->key[i];
		node->child[node->keyNum + 1] = rightNode->child[0];
		node->keyNum++;
		int j = 0;
		while (j < rightNode->keyNum)
		{
			node->key[node->keyNum] = rightNode->key[j];
			node->child[node->keyNum + 1] = rightNode->child[j + 1];
			node->keyNum++;
			j++;
		}
		removeValueFromNode(node->parent->key[i], node->parent);
		delete rightNode;
		return;
	}

	if (node->parent == root)
	{
		adjustRoot();
	}
}

void BPlusTree::mergeNode(BPNode* node)
{
	int i = 0;
	while (node->parent->child[i] != node)
	{
		i++;
	}
	if (i == 0)
	{
		BPNode* rightNode = node->parent->child[i + 1];
		node->key[node->keyNum] = node->parent->key[i];
		node->child[node->keyNum] = rightNode->child[0];
		node->keyNum++;
		int j = 0;
		while (j < rightNode->keyNum)
		{
			node->key[node->keyNum] = rightNode->key[j];
			node->child[node->keyNum] = rightNode->child[j + 1];
			node->keyNum++;
			j++;
		}
		node->parent->keyNum--;
		delete rightNode;
	}
	else
	{
		BPNode* leftNode = node->parent->child[i - 1];
		leftNode->key[leftNode->keyNum] = node->parent->key[i - 1];
		leftNode->child[leftNode->keyNum] = node->child[0];
		leftNode->keyNum++;
		int j = 0;
		while (j < node->keyNum)
		{
			leftNode->key[leftNode->keyNum] = node->key[j];
			leftNode->child[leftNode->keyNum] = node->child[j + 1];
			leftNode->keyNum++;
			j++;
		}
		node->parent->keyNum--;
		delete node;
	}
}

void BPlusTree::printNode(BPNode* node)
{
	int i = 0;
	while (i < node->keyNum)
	{
		cout << node->key[i] << " ";
		i++;
	}
	cout << endl;
}

void BPlusTree::searchNode(int key, BPNode* node)
{
	int i = 0;
	while (i < node->keyNum && node->key[i] != key)
	{
		i++;
	}
	if (i < node->keyNum)
	{
		cout << "The key is in the tree!" << endl;
		return;
	}
	else
	{
		if (node->isLeaf == true)
		{
			cout << "The key is not in the tree!" << endl;
			return;
		}
		else
		{
			i = 0;
			while (i < node->keyNum)
			{
				if (key >= node->key[i])
				{
					i++;
				}
				else
				{
					break;
				}
			}
			searchNode(key, node->child[i]);
		}
	}
}

void BPlusTree::removeNode(int key, BPNode* node)
{
	int i = 0;
	while (i < node->keyNum && node->key[i] != key)
	{
		i++;
	}
	if (i < node->keyNum)
	{
		if (node->isLeaf == true)
		{
			removeValueFromNode(key, node);
			return;
		}
		else
		{
			BPNode* leftNode = node->child[i];
			BPNode* rightNode = node->child[i + 1];
			if (leftNode->keyNum > MIN)
			{
				int j = leftNode->keyNum - 1;
				while (j > 0)
				{
					leftNode->key[j] = leftNode->key[j - 1];
					j--;
				}
				leftNode->key[0] = node->key[i];
				node->key[i] = leftNode->key[leftNode->keyNum - 1];
				leftNode->keyNum--;
				removeNode(key, leftNode);
				return;
			}
			else if (rightNode->keyNum > MIN)
			{
				leftNode->key[leftNode->keyNum] = node->key[i];
				leftNode->keyNum++;
				node->key[i] = rightNode->key[0];
				int j = 0;
				while (j < rightNode->keyNum - 1)
				{
					rightNode->key[j] = rightNode->key[j + 1];
					j++;
				}
				rightNode->keyNum--;
				removeNode(key, leftNode);
				return;
			}
			else
			{
				leftNode->key[leftNode->keyNum] = node->key[i];
				leftNode->keyNum++;
				int j = 0;
				while (j < rightNode->keyNum)
				{
					leftNode->key[leftNode->keyNum] = rightNode->key[j];
					leftNode->keyNum++;
					j++;
				}
				removeValueFromNode(node->key[i], node);
				delete rightNode;
				removeNode(key, leftNode);
				return;
			}
		}
	}
	else
	{
		if (node->isLeaf == true)
		{
			cout << "The key is not in the tree!" << endl;
			return;
		}
		else
		{
			i = 0;
			while (i < node->keyNum)
			{
				if (key >= node->key[i])
				{
					i++;
				}
				else
				{
					break;
				}
			}
			removeNode(key, node->child[i]);
		}
	}
}

void BPlusTree::removeInternalNode(int key, BPNode* node)
{
	int i = 0;
	while (i < node->keyNum && node->key[i] != key)
	{
		i++;
	}
	if (i < node->keyNum)
	{
		if (node->isLeaf == true)
		{
			removeValueFromNode(key, node);
			return;
		}
		else
		{
			BPNode* leftNode = node->child[i];
			BPNode* rightNode = node->child[i + 1];
			if (leftNode->keyNum > MIN)
			{
				int j = leftNode->keyNum - 1;
				while (j > 0)
				{
					leftNode->key[j] = leftNode->key[j - 1];
					j--;
				}
				leftNode->key[0] = node->key[i];
				node->key[i] = leftNode->key[leftNode->keyNum - 1];
				leftNode->keyNum--;
				removeInternalNode(key, leftNode);
				return;
			}
			else if (rightNode->keyNum > MIN)
			{
				leftNode->key[leftNode->keyNum] = node->key[i];
				leftNode->keyNum++;
				node->key[i] = rightNode->key[0];
				int j = 0;
				while (j < rightNode->keyNum - 1)
				{
					rightNode->key[j] = rightNode->key[j + 1];
					j++;
				}
				rightNode->keyNum--;
				removeInternalNode(key, leftNode);
				return;
			}
			else
			{
				leftNode->key[leftNode->keyNum] = node->key[i];
				leftNode->keyNum++;
				int j = 0;
				while (j < rightNode->keyNum)
				{
					leftNode->key[leftNode->keyNum] = rightNode->key[j];
					leftNode->keyNum++;
					j++;
				}
				removeValueFromNode(node->key[i], node);
				delete rightNode;
				removeInternalNode(key, leftNode);
				return;
			}
		}
	}
	else
	{
		if (node->isLeaf == true)
		{
			cout << "The key is not in the tree!" << endl;
			return;
		}
		else
		{
			i = 0;
			while (i < node->keyNum)
			{
				if (key >= node->key[i])
				{
					i++;
				}
				else
				{
					break;
				}
			}
			removeInternalNode(key, node->child[i]);
		}
	}
}

/*

void BPlusTree::rotateLeft(BPNode* node, int index)
{
	BPNode* leftNode = node->child[index];
	BPNode* rightNode = node->child[index + 1];
	leftNode->key[leftNode->keyNum] = node->key[index];
	leftNode->keyNum++;
	node->key[index] = rightNode->key[0];
	int i = 0;
	while (i < rightNode->keyNum - 1)
	{
		rightNode->key[i] = rightNode->key[i + 1];
		i++;
	}
	rightNode->keyNum--;
}

void BPlusTree::rotateRight(BPNode* node, int index)
{
	BPNode* leftNode = node->child[index];
	BPNode* rightNode = node->child[index + 1];
	int i = rightNode->keyNum - 1;
	while (i > 0)
	{
		rightNode->key[i] = rightNode->key[i - 1];
		i--;
	}
	rightNode->key[0] = node->key[index];
	rightNode->keyNum++;
	node->key[index] = leftNode->key[leftNode->keyNum - 1];
	leftNode->keyNum--;
}

*/

void BPlusTree::rotateLeft(BPNode* node, BPNode* leftChild, int index)
{
	BPNode* rightChild = node->child[index + 1];
	leftChild->key[leftChild->keyNum] = node->key[index];
	leftChild->keyNum++;
	node->key[index] = rightChild->key[0];
	int i = 0;
	while (i < rightChild->keyNum - 1)
	{
		rightChild->key[i] = rightChild->key[i + 1];
		i++;
	}
	rightChild->keyNum--;
}

void BPlusTree::rotateRight(BPNode* node, BPNode* rightChild, int index)
{
	BPNode* leftChild = node->child[index];
	int i = rightChild->keyNum - 1;
	while (i > 0)
	{
		rightChild->key[i] = rightChild->key[i - 1];
		i--;
	}
	rightChild->key[0] = node->key[index];
	rightChild->keyNum++;
	node->key[index] = leftChild->key[leftChild->keyNum - 1];
	leftChild->keyNum--;
}

void BPlusTree::rotate(BPNode* node, int index)
{
	BPNode* leftChild = node->child[index];
	BPNode* rightChild = node->child[index + 1];
	if (leftChild->keyNum > MIN)
	{
		rotateRight(node, rightChild, index);
	}
	else if (rightChild->keyNum > MIN)
	{
		rotateLeft(node, leftChild, index);
	}
	else
	{
		merge(node, index);
	}
}

void BPlusTree::merge(BPNode* node, int index)
{
	BPNode* leftNode = node->child[index];
	BPNode* rightNode = node->child[index + 1];
	leftNode->key[leftNode->keyNum] = node->key[index];
	leftNode->keyNum++;
	int i = 0;
	while (i < rightNode->keyNum)
	{
		leftNode->key[leftNode->keyNum] = rightNode->key[i];
		leftNode->keyNum++;
		i++;
	}
	removeValueFromNode(node->key[index], node);
	delete rightNode;
}

void BPlusTree::traversal(BPNode* node)
{
	if (node == NULL)
	{
		return;
	}
	int i = 0;
	while (i < node->keyNum)
	{
		cout << node->key[i] << " ";
		i++;
	}
	cout << endl;
	if (node->isLeaf == false)
	{
		i = 0;
		while (i <= node->keyNum)
		{
			traversal(node->child[i]);
			i++;
		}
	}
}

void BPlusTree::traversal()
{
	traversal(root);
}

void BPlusTree::preOrder(BPNode* node)
{
	if (node == NULL)
	{
		return;
	}
	int i = 0;
	while (i < node->keyNum)
	{
		cout << node->key[i] << " ";
		i++;
	}
	cout << endl;
	if (node->isLeaf == false)
	{
		i = 0;
		while (i < node->keyNum)
		{
			preOrder(node->child[i]);
			i++;
		}
		preOrder(node->child[i]);
	}
}

void BPlusTree::preOrder()
{
	preOrder(root);
}

void BPlusTree::inOrder(BPNode* node)
{
	if (node == NULL)
	{
		return;
	}
	if (node->isLeaf == false)
	{
		inOrder(node->child[0]);
	}
	int i = 0;
	while (i < node->keyNum)
	{
		cout << node->key[i] << " ";
		i++;
		if (node->isLeaf == false)
		{
			inOrder(node->child[i]);
		}
	}
}

void BPlusTree::inOrder()
{
	inOrder(root);
}

int main() {
	int option;
	int key;
	BPlusTree tree;
	while (true)
	{
		cout << "1. Insert" << endl;
		cout << "2. Delete" << endl;
		cout << "3. Search" << endl;
		cout << "4. Traversal" << endl;
		cout << "5. PreOrder" << endl;
		cout << "6. InOrder" << endl;
		cout << "7. Print Tree" << endl;
		cout << "8. Exit" << endl;
		cout << "Please input your option: ";
		cin >> option;
		switch (option)
		{
		case 1:
			cout << "Please input the key: ";
			cin >> key;
			tree.insert(key);
			break;
		case 2:
			cout << "Please input the key: ";
			cin >> key;
			tree.remove(key);
			break;
		case 3:
			cout << "Please input the key: ";
			cin >> key;
			tree.search(key);
		case 4:
			tree.traversal();
			break;
		case 5:
			tree.preOrder();
			break;
		case 6:
			tree.inOrder();
			break;
		case 7:
			tree.printTree();
			break;
		case 8:
			return 0;
		default:
			break;

		}
	}
}