// 0414-AVLTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;

struct AVLNode
{
	int data;
	AVLNode* left;
	AVLNode* right;
	int height;
};

class AVLTree
{
private:
	AVLNode* root;
	int height(AVLNode* node);
	int max(int a, int b);
	AVLNode* createNewNode(int data);
	AVLNode* rightRotate(AVLNode* y);
	AVLNode* leftRotate(AVLNode* x);
	int getBalance(AVLNode* node);
	AVLNode* insert(AVLNode* node, int data);
	void preOrder(AVLNode* node);
	void inOrder(AVLNode* node);
	void postOrder(AVLNode* node);
	void deleteNode(AVLNode* node, int data);
	AVLNode* minValueNode(AVLNode* node);
	AVLNode* deleteNode(AVLNode* root, AVLNode* node);
	AVLNode* searchNode(AVLNode* node, int data);
	void destroy(AVLNode* node);
	
public:
	AVLTree();
	void insert(int data);
	void preOrder();
	void inOrder();
	void postOrder();
	void deleteNode(int data);
	void search(int data);
	void destroy();
};

AVLTree::AVLTree()
{
	root = NULL;
}

int AVLTree::height(AVLNode* node)
{
	if (node != NULL)
		return node->height;
	return 0;
}

int AVLTree::max(int a, int b)
{
	return (a > b) ? a : b;
}

AVLNode* AVLTree::createNewNode(int data)
{
	AVLNode* node = new AVLNode();
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return node;
}

AVLNode* AVLTree::rightRotate(AVLNode* y)
{
	AVLNode* x = y->left;
	AVLNode* T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;

	// Return new root
	return x;
}

AVLNode* AVLTree::leftRotate(AVLNode* x)
{
	AVLNode* y = x->right;
	AVLNode* T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;

	// Return new root
	return y;
}

int AVLTree::getBalance(AVLNode* node)
{
	if (node == NULL)
		return 0;
	return height(node->left) - height(node->right);
}

AVLNode* AVLTree::insert(AVLNode* node, int data)
{
	// 1. Perform the normal BST insertion
	if (node == NULL)
		return createNewNode(data);

	if (data < node->data)
		node->left = insert(node->left, data);
	else if (data > node->data)
		node->right = insert(node->right, data);
	else
		return node;

	// 2. Update height of this ancestor node
	node->height = 1 + max(height(node->left), height(node->right));

	// 3. Get the balance factor of this ancestor node to check whether this node became unbalanced
	int balance = getBalance(node);

	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (balance > 1 && data < node->left->data)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && data > node->right->data)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && data > node->left->data)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && data < node->right->data)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	// return the (unchanged) node pointer
	return node;
}

void AVLTree::insert(int data)
{
	root = insert(root, data);
}

void AVLTree::preOrder(AVLNode* node)
{
	if (node != NULL)
	{
		cout << node->data << " ";
		preOrder(node->left);
		preOrder(node->right);
	}
	cout << endl;
}

void AVLTree::preOrder()
{
	preOrder(root);
}

void AVLTree::inOrder(AVLNode* node)
{
	if (node != NULL)
	{
		inOrder(node->left);
		 cout << node->data << " ";
		inOrder(node->right);
	}
	cout << endl;
}

void AVLTree::inOrder()
{
	inOrder(root);
}

void AVLTree::postOrder(AVLNode* node)
{
	if (node != NULL)
	{
		postOrder(node->left);
		postOrder(node->right);
		cout << node->data << " ";
	}
	cout << endl;
}

void AVLTree::postOrder()
{
	postOrder(root);
}

void AVLTree::deleteNode(int data)
{
	deleteNode(root, data);
}

AVLNode* AVLTree::minValueNode(AVLNode* node)
{
	AVLNode* current = node;

	// loop down to find the leftmost leaf
	while (current->left != NULL)
		current = current->left;

	return current;
}

AVLNode* AVLTree::deleteNode(AVLNode* root, AVLNode* node)
{
	// STEP 1: PERFORM STANDARD BST DELETE
	if (node == NULL)
		return root;

	if (node->data < root->data)
		root->left = deleteNode(root->left, node);
	else if (node->data > root->data)
		root->right = deleteNode(root->right, node);
	else
	{
		// node with only one child or no child
		if ((root->left == NULL) || (root->right == NULL))
		{
			AVLNode* temp = root->left ? root->left : root->right;

			// No child case
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // One child case
				*root = *temp; // Copy the contents of the non-empty child
			delete temp;
		}
		else
		{
			// node with two children: Get the inorder successor (smallest in the right subtree)
			AVLNode* temp = minValueNode(root->right);

			// Copy the inorder successor's data to this node
			root->data = temp->data;

			// Delete the inorder successor
			root->right = deleteNode(root->right, temp);
		}
	}

	// If the tree had only one node then return
	if (root == NULL)
		return root;

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
	root->height = 1 + max(height(root->left), height(root->right));

	// STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether this node became unbalanced)
	int balance = getBalance(root);

	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (balance > 1 && getBalance(root->left) >= 0)
		return rightRotate(root);

	// Left Right Case
	if (balance > 1 && getBalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Right Case
	if (balance < -1 && getBalance(root->right) <= 0)
		return leftRotate(root);

	// Right Left Case
	if (balance < -1 && getBalance(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

void AVLTree::deleteNode(AVLNode* node, int data)
{
	AVLNode* temp = searchNode(node, data);
	if (temp == NULL)
	{
		cout << "Node not found" << endl << endl;
		return;
	}
	root = deleteNode(root, temp);
	cout << "Node with value " << data << " deleted" << endl << endl;
}

AVLNode* AVLTree::searchNode(AVLNode* node, int data)
{
	if (node == NULL || node->data == data)
		return node;

	if (node->data < data)
		return searchNode(node->right, data);

	return searchNode(node->left, data);
}

void AVLTree::search(int data)
{
	AVLNode* temp = searchNode(root, data);
	if (temp == NULL)
		cout << "Node with value " << data << " not found" << endl << endl;
	else
		cout << "Node with value " << data << " found" << endl << endl;
}

void AVLTree::destroy(AVLNode* node)
{
	if (node != NULL)
	{
		destroy(node->left);
		destroy(node->right);
		delete node;
	}
}

void AVLTree::destroy()
{
	destroy(root);
}

int main()
{
	int value, option;
	AVLTree tree;
	do {
		cout << "1. Insert" << endl;
		cout << "2. Delete" <<  endl;
		cout << "3. Search" <<  endl;
		cout << "4. Preorder" <<  endl;
		cout << "5. Inorder" <<  endl;
		cout << "6. Postorder" <<  endl;
		cout << "7. Exit" <<  endl;
		cout << "Enter your option: ";
		cin >> option;
		switch (option)
		{
		case 1:
			cout << "Enter value to insert: ";
			cin >> value;
			cout << endl;
			tree.insert(value);
			break;
		case 2:
			cout << "Enter value to delete: ";
			cin >> value;
			tree.deleteNode(value);
			break;
		case 3:
			cout << "Enter value to search: ";
			cin >> value;
			tree.search(value);
			break;
		case 4:
			cout << "Preorder: ";
			tree.preOrder();
			 cout <<  endl << endl;
			break;
		case 5:
			cout << "Inorder: ";
			tree.inOrder();
			 cout <<  endl << endl;
			break;
		case 6:
			cout << "Postorder: ";
			tree.postOrder();
			cout << endl << endl;
			break;
		case 7:
			tree.destroy();
			break;
		default:
			cout << "Invalid option" <<  endl;
		}
	} while (option != 7);

	return 0;
}

