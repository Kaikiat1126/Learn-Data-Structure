// 0424-BPlusTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
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
	void Insert(int key);
	void Delete(int key);
	void Search(int key);
	void Print();
private:
	BPNode* root;
	BPNode* headLeaf;
	void Insert(int key, BPNode* node);
	void Delete(int key, BPNode* node);
	void Search(int key, BPNode* node);
	void Print(BPNode* node);
	void Split(BPNode* node);
	void Merge(BPNode* node);
	void Rotate(BPNode* node, int index);
	void RotateLeft(BPNode* node, int index);
	void RotateRight(BPNode* node, int index);
	void DeleteNode(BPNode* node, int index);

};

BPlusTree::BPlusTree()
{
	root = new BPNode;
	root->isLeaf = true;
	root->keyNum = 0;
	root->parent = NULL;
	root->nextLeaf = NULL;
	headLeaf = root;
}

BPlusTree::~BPlusTree()
{
}

void BPlusTree::Insert(int key)
{
	Insert(key, root);
}

void BPlusTree::Insert(int key, BPNode* node)
{
	if (node->isLeaf)
	{
		int i = node->keyNum;
		while (i > 0 && key < node->key[i])
		{
			node->key[i + 1] = node->key[i];
			i--;
		}
		node->key[i + 1] = key;
		node->keyNum++;
		if (node->keyNum == MAX)
		{
			Split(node);
		}
	}
	else
	{
		int i = 1;
		while (i <= node->keyNum && key > node->key[i])
		{
			i++;
		}
		Insert(key, node->child[i]);
	}
}

void BPlusTree::Delete(int key)
{
	Delete(key, root);
}

void BPlusTree::Delete(int key, BPNode* node)
{
	if (node->isLeaf)
	{
		int i = 1;
		while (i <= node->keyNum && key > node->key[i])
		{
			i++;
		}
		if (i <= node->keyNum && key == node->key[i])
		{
			for (int j = i; j < node->keyNum; j++)
			{
				node->key[j] = node->key[j + 1];
			}
			node->keyNum--;
			if (node->keyNum < MIN)
			{
				Merge(node);
			}
		}
	}
	else
	{
		int i = 1;
		while (i <= node->keyNum && key > node->key[i])
		{
			i++;
		}
		Delete(key, node->child[i]);
	}
}

void BPlusTree::Search(int key)
{
	Search(key, root);
}

void BPlusTree::Search(int key, BPNode* node)
{
	if (node->isLeaf)
	{
		int i = 1;
		while (i <= node->keyNum && key > node->key[i])
		{
			i++;
		}
		if (i <= node->keyNum && key == node->key[i])
		{
			cout << "Found" << endl;
		}
		else
		{
			cout << "Not Found" << endl;
		}
	}
	else
	{
		int i = 1;
		while (i <= node->keyNum && key > node->key[i])
		{
			i++;
		}
		Search(key, node->child[i]);
	}
}

void BPlusTree::Print()
{
	Print(root);
}

void BPlusTree::Print(BPNode* node)
{
	if (node->isLeaf)
	{
		BPNode* p = headLeaf;
		while (p != NULL)
		{
			for (int i = 1; i <= p->keyNum; i++)
			{
				cout << p->key[i] << " ";
			}
			p = p->nextLeaf;
		}
		cout << endl;
	}
	else
	{
		for (int i = 1; i <= node->keyNum; i++)
		{
			Print(node->child[i]);
			cout << node->key[i] << " ";
		}
		Print(node->child[node->keyNum + 1]);
	}
}

void BPlusTree::Split(BPNode* node)
{
	BPNode* newNode = new BPNode;
	newNode->isLeaf = node->isLeaf;
	newNode->keyNum = MIN;
	newNode->parent = node->parent;
	newNode->nextLeaf = node->nextLeaf;
	node->nextLeaf = newNode;
	for (int i = 1; i <= MIN; i++)
	{
		newNode->key[i] = node->key[i + MIN];
	}
	if (!node->isLeaf)
	{
		for (int i = 1; i <= MIN + 1; i++)
		{
			newNode->child[i] = node->child[i + MIN];
			newNode->child[i]->parent = newNode;
		}
	}
	node->keyNum = MIN;
	if (node->parent == NULL)
	{
		BPNode* newRoot = new BPNode;
		newRoot->isLeaf = false;
		newRoot->keyNum = 1;
		newRoot->key[1] = node->key[MIN + 1];
		newRoot->child[1] = node;
		newRoot->child[2] = newNode;
		node->parent = newRoot;
		newNode->parent = newRoot;
		root = newRoot;
	}
	else
	{
		int i = node->parent->keyNum;
		while (i > 0 && node->key[MIN + 1] < node->parent->key[i])
		{
			node->parent->key[i + 1] = node->parent->key[i];
			node->parent->child[i + 2] = node->parent->child[i + 1];
			i--;
		}
		node->parent->key[i + 1] = node->key[MIN + 1];
		node->parent->child[i + 2] = newNode;
		node->parent->keyNum++;
		if (node->parent->keyNum == MAX)
		{
			Split(node->parent);
		}
	}
}

void BPlusTree::Merge(BPNode* node)
{
	if (node->parent == NULL)
	{
		if (node->keyNum == 0)
		{
			root = node->child[1];
			root->parent = NULL;
			delete node;
		}
	}
	else
	{
		int i = 1;
		while (i <= node->parent->keyNum && node->parent->child[i] != node)
		{
			i++;
		}
		if (i > 1 && node->parent->child[i - 1]->keyNum > MIN)
		{
			for (int j = node->keyNum; j > 0; j--)
			{
				node->key[j + 1] = node->key[j];
			}
			node->key[1] = node->parent->key[i];
			node->parent->key[i] = node->parent->child[i - 1]->key[node->parent->child[i - 1]->keyNum];
			node->parent->child[i - 1]->keyNum--;
			node->keyNum++;
			if (!node->isLeaf)
			{
				for (int j = node->keyNum; j > 0; j--)
				{
					node->child[j + 1] = node->child[j];
				}
				node->child[1] = node->parent->child[i - 1]->child[node->parent->child[i - 1]->keyNum + 1];
				node->child[1]->parent = node;
			}
		}
		else if (i < node->parent->keyNum && node->parent->child[i + 1]->keyNum > MIN)
		{
			node->key[node->keyNum + 1] = node->parent->key[i + 1];
			node->parent->key[i + 1] = node->parent->child[i + 1]->key[1];
			for (int j = 1; j < node->parent->child[i + 1]->keyNum; j++)
			{
				node->parent->child[i + 1]->key[j] = node->parent->child[i + 1]->key[j + 1];
			}
			node->parent->child[i + 1]->keyNum--;
			node->keyNum++;
			if (!node->isLeaf)
			{
				node->child[node->keyNum + 1] = node->parent->child[i + 1]->child[1];
				node->child[node->keyNum + 1]->parent = node;
				for (int j = 1; j <= node->parent->child[i + 1]->keyNum + 1; j++)
				{
					node->parent->child[i + 1]->child[j] = node->parent->child[i + 1]->child[j + 1];
				}
			}
		}
		else
		{
			if (i < node->parent->keyNum)
			{
				node->key[node->keyNum + 1] = node->parent->key[i + 1];
				for (int j = 1; j <= node->parent->child[i + 1]->keyNum; j++)
				{
					node->key[node->keyNum + 1 + j] = node->parent->child[i + 1]->key[j];
				}
				if (!node->isLeaf)
				{
					for (int j = 1; j <= node->parent->child[i + 1]->keyNum + 1; j++)
					{
						node->child[node->keyNum + j] = node->parent->child[i + 1]->child[j];
						node->child[node->keyNum + j]->parent = node;
					}
				}
				node->keyNum += node->parent->child[i + 1]->keyNum + 1;
				for (int j = i + 1; j < node->parent->keyNum; j++)
				{
					node->parent->key[j] = node->parent->key[j + 1];
					node->parent->child[j + 1] = node->parent->child[j + 2];
				}
				node->parent->keyNum--;
				delete node->parent->child[i + 1];
				if (node->parent->keyNum == 0)
				{
					Merge(node->parent);
				}
			}
			else
			{
				node->parent->child[i - 1]->key[node->parent->child[i - 1]->keyNum + 1] = node->parent->key[i];
				for (int j = 1; j <= node->keyNum; j++)
				{
					node->parent->child[i - 1]->key[node->parent->child[i - 1]->keyNum + 1 + j] = node->key[j];
				}
				if (!node->isLeaf)
				{
					for (int j = 1; j <= node->keyNum + 1; j++)
					{
						node->parent->child[i - 1]->child[node->parent->child[i - 1]->keyNum + j] = node
							->child[j];
						node->parent->child[i - 1]->child[node->parent->child[i - 1]->keyNum + j]->parent = node->parent->child[i - 1];
					}
				}
				node->parent->child[i - 1]->keyNum += node->keyNum + 1;
				for (int j = i; j < node->parent->keyNum; j++)
				{
					node->parent->key[j] = node->parent->key[j + 1];
					node->parent->child[j] = node->parent->child[j + 1];
				}
				node->parent->keyNum--;
				delete node->parent->child[i];
				if (node->parent->keyNum == 0)
				{
					Merge(node->parent);
				}
			}
		}
	}
}

void BPlusTree::RotateLeft(BPNode* node, int i)
{
	if (node->parent->child[i - 1]->keyNum > MIN)
	{
		for (int j = node->keyNum; j > 0; j--)
		{
			node->key[j + 1] = node->key[j];
		}
		node->key[1] = node->parent->key[i];
		node->parent->key[i] = node->parent->child[i - 1]->key[node->parent->child[i - 1]->keyNum];
		node->parent->child[i - 1]->keyNum--;
		node->keyNum++;
		if (!node->isLeaf)
		{
			for (int j = node->keyNum; j > 0; j--)
			{
				node->child[j + 1] = node->child[j];
			}
			node->child[1] = node->parent->child[i - 1]->child[node->parent->child[i - 1]->keyNum + 1];
			node->child[1]->parent = node;
		}
	}
	else
	{
		node->parent->child[i - 1]->key[node->parent->child[i - 1]->keyNum + 1] = node->parent->key[i];
		for (int j = 1; j <= node->keyNum; j++)
		{
			node->parent->child[i - 1]->key[node->parent->child[i - 1]->keyNum + 1 + j] = node->key[j];
		}
		if (!node->isLeaf)
		{
			for (int j = 1; j <= node->keyNum + 1; j++)
			{
				node->parent->child[i - 1]->child[node->parent->child[i - 1]->keyNum + j] = node->child[j];
				node->parent->child[i - 1]->child[node->parent->child[i - 1]->keyNum + j]->parent = node->parent->child[i - 1];
			}
		}
		node->parent->child[i - 1]->keyNum += node->keyNum + 1;
		for (int j = i; j < node->parent->keyNum; j++)
		{
			node->parent->key[j] = node->parent->key[j + 1];
			node->parent->child[j] = node->parent->child[j + 1];
		}
		node->parent->keyNum--;
		delete node->parent->child[i];
		if (node->parent->keyNum == 0)
		{
			Merge(node->parent);
		}
	}
}

void BPlusTree::RotateRight(BPNode* node, int i)
{
	if (node->parent->child[i + 1]->keyNum > MIN)
	{
		node->key[node->keyNum + 1] = node->parent->key[i + 1];
		node->parent->key[i + 1] = node->parent->child[i + 1]->key[1];
		for (int j = 1; j < node->parent->child[i + 1]->keyNum; j++)
		{
			node->parent->child[i + 1]->key[j] = node->parent->child[i + 1]->key[j + 1];
		}
		node->keyNum++;
		node->parent->child[i + 1]->keyNum--;
		if (!node->isLeaf)
		{
			node->child[node->keyNum + 1] = node->parent->child[i + 1]->child[1];
			node->child[node->keyNum + 1]->parent = node;
			for (int j = 1; j <= node->parent->child[i + 1]->keyNum; j++)
			{
				node->parent->child[i + 1]->child[j] = node->parent->child[i + 1]->child[j + 1];
			}
		}
	}
	else
	{
		node->key[node->keyNum + 1] = node->parent->key[i + 1];
		for (int j = 1; j <= node->parent->child[i + 1]->keyNum; j++)
		{
			node->key[node->keyNum + 1 + j] = node->parent->child[i + 1]->key[j];
		}
		if (!node->isLeaf)
		{
			for (int j = 1; j <= node->parent->child[i + 1]->keyNum + 1; j++)
			{
				node->child[node->keyNum + j] = node->parent->child[i + 1]->child[j];
				node->child[node->keyNum + j]->parent = node;
			}
		}
		node->keyNum += node->parent->child[i + 1]->keyNum + 1;
		for (int j = i + 1; j < node->parent->keyNum; j++)
		{
			node->parent->key[j] = node->parent->key[j + 1];
			node->parent->child[j] = node->parent->child[j + 1];
		}
		node->parent->keyNum--;
		delete node->parent->child[i + 1];
		if (node->parent->keyNum == 0)
		{
			Merge(node->parent);
		}
	}
}

void BPlusTree::Rotate(BPNode* node, int i)
{
	if (i == 0)
	{
		RotateRight(node, i);
	}
	else if (i == node->parent->keyNum)
	{
		RotateLeft(node, i);
	}
	else
	{
		if (node->parent->child[i - 1]->keyNum > MIN)
		{
			RotateLeft(node, i);
		}
		else if (node->parent->child[i + 1]->keyNum > MIN)
		{
			RotateRight(node, i);
		}
		else
		{
			Merge(node);
		}
	}
}

void BPlusTree::DeleteNode(BPNode* node, int key)
{
	int i = 1;
	while (i <= node->keyNum && key > node->key[i])
	{
		i++;
	}
	if (i <= node->keyNum && key == node->key[i])
	{
		if (node->isLeaf)
		{
			for (int j = i; j < node->keyNum; j++)
			{
				node->key[j] = node->key[j + 1];
			}
			node->keyNum--;
			if (node->keyNum < MIN)
			{
				if (node->parent == NULL)
				{
					if (node->keyNum == 0)
					{
						root = NULL;
						delete node;
					}
				}
				else
				{
					Merge(node);
				}
			}
		}
		else
		{
			BPNode* temp = node->child[i];
			while (!temp->isLeaf)
			{
				temp = temp->child[temp->keyNum];
			}
			node->key[i] = temp->key[temp->keyNum];
			DeleteNode(temp, temp->key[temp->keyNum]);
		}
	}
	else
	{
		if (node->isLeaf)
		{
			return;
		}
		else
		{
			if (node->child[i]->keyNum > MIN)
			{
				DeleteNode(node->child[i], key);
			}
			else
			{
				if (i > 1 && node->child[i - 1]->keyNum > MIN)
				{
					for (int j = node->child[i]->keyNum; j > 0; j--)
					{
						node->child[i]->key[j + 1] = node->child[i]->key[j];
					}
					node->child[i]->key[1] = node->key[i];
					node->key[i] = node->child[i - 1]->key[node->child[i - 1]->keyNum];
					if (!node->child[i]->isLeaf)
					{
						for (int j = node->child[i]->keyNum; j > 0; j--)
						{
							node->child[i]->child[j + 1] = node->child[i]->child[j];
						}
						node->child[i]->child[1] = node->child[i - 1]->child[node->child[i - 1]->keyNum + 1];
						node->child[i]->child[1]->parent = node->child[i];
					}
					node->child[i]->keyNum++;
					node->child[i - 1]->keyNum--;
					DeleteNode(node->child[i], key);
				}
				else if (i < node->keyNum && node->child[i + 1]->keyNum > MIN)
				{
					node->child[i]->key[node->child[i]->keyNum + 1] = node->key[i + 1];
					node->key[i + 1] = node->child[i + 1]->key[1];
					if (!node->child[i]->isLeaf)
					{
						node->child[i]->child[node->child[i]->keyNum + 2] = node->child[i + 1]->child[1];
						node->child[i]->child[node->child[i]->keyNum + 2]->parent = node->child[i];
					}
					node->child[i]->keyNum++;
					for (int j = 1; j < node->child[i + 1]->keyNum; j++)
					{
						node->child[i + 1]->key[j] = node->child[i + 1]->key[j + 1];
					}
					if (!node->child[i + 1]->isLeaf)
					{
						for (int j = 1; j <= node->child[i + 1]->keyNum; j++)
						{
							node->child[i + 1]->child[j] = node->child[i + 1]->child[j + 1];
						}
					}
					node->child[i + 1]->keyNum--;
					DeleteNode(node->child[i], key);
				}
				else
				{
					if (i < node->keyNum)
					{
						node->child[i]->key[node->child[i]->keyNum + 1] = node->key[i + 1];
						for (int j = 1; j <= node->child[i + 1]->keyNum; j++)
						{
							node->child[i]->key[node->child[i]->keyNum + 1 + j] = node->child[i + 1]->key[j];
						}
						if (!node->child[i]->isLeaf)
						{
							for (int j = 1; j <= node->child[i + 1]->keyNum + 1; j++)
							{
								node->child[i]->child[node->child[i]->keyNum + 1 + j] = node->child[i + 1]->child[j];
								node->child[i]->child[node->child[i]->keyNum + 1 + j]->parent = node->child[i];
							}
						}
						node->child[i]->keyNum += node->child[i + 1]->keyNum + 1;
						for (int j = i + 1; j < node->keyNum; j++)
						{
							node->key[j] = node->key[j + 1];
						}
						for (int j = i + 2; j <= node->keyNum; j++)
						{
							node->child[j] = node->child[j + 1];
						}
						node->keyNum--;
						DeleteNode(node->child[i], key);
					}
					else
					{
						node->child[i]->key[node->child[i]->keyNum + 1] = node->key[i];
						for (int j = 1; j <= node->child[i - 1]->keyNum; j++)
						{
							node->child[i]->key[node->child[i]->keyNum + 1 + j] = node->child[i - 1]->key[j];
						}
						if (!node->child[i]->isLeaf)
						{
							for (int j = 1; j <= node->child[i - 1]->keyNum + 1; j++)
							{
								node->child[i]->child[node->child[i]->keyNum + 1 + j] = node->child[i - 1]->child[j];
								node->child[i]->child[node->child[i]->keyNum + 1 + j]->parent = node->child[i];
							}
						}
						node->child[i]->keyNum += node->child[i - 1]->keyNum + 1;
						for (int j = i; j < node->keyNum; j++)
						{
							node->key[j] = node->key[j + 1];
						}
						for (int j = i + 1; j <= node->keyNum; j++)
						{
							node->child[j] = node->child[j + 1];
						}
						node->keyNum--;
						DeleteNode(node->child[i], key);
					}
				}
			}
		}
	}
}

int main()
{
	int option;
	int key;
	BPlusTree tree;
	while (true)
	{
		cout << "1. Insert" << endl;
		cout << "2. Delete" << endl;
		cout << "3. Search" << endl;
		cout << "4. Print" << endl;
		cout << "5. Exit" << endl;
		cout << "Please enter your option: ";
		cin >> option;
		switch (option)
		{
		case 1:
			cout << "Please enter the key: ";
			cin >> key;
			tree.Insert(key);
			break;
		case 2:
			cout << "Please enter the key: ";
			cin >> key;
			tree.Delete(key);
			break;
		case 3:
			cout << "Please enter the key: ";
			cin >> key;
			tree.Search(key);
			break;
		case 4:
			tree.Print();
			break;
		case 5:
			return 0;
		default:
			cout << "Invalid option!" << endl;
			break;
		}
	}

}

*/