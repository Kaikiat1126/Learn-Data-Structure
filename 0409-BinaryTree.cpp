// 0409-BinaryTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <iostream>

using namespace std;

struct Node 
{
	char data;
	Node* LeftChild;
	Node* RightChild;
};

Node* createNode(char data) {
	struct Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL) {
		printf("Memory Error");
		return NULL;
	}
	newNode->data = data;
	newNode->LeftChild = NULL;
	newNode->RightChild = NULL;
	return newNode;
}

void insertNode(Node* parent, Node* LeftChild, Node* RightChild) {
	if (parent == NULL) return;
	parent->LeftChild = LeftChild;
	parent->RightChild = RightChild;
}

//遍历方式
//递归遍历
void preOrder(Node* root)
{
	//根左右
	if (root == NULL) return;
	printf("%c ", root->data);
	preOrder(root->LeftChild);
	preOrder(root->RightChild);
}

void midOrder(Node* root)
{
	//左根右
	if (root == NULL) return;
	midOrder(root->LeftChild);
	printf("%c ", root->data);
	midOrder(root->RightChild);
}

void lastOrder(Node* root)
{
	//左右根
	if (root == NULL) return;
	lastOrder(root->LeftChild);
	lastOrder(root->RightChild);
	printf("%c ", root->data);
}

//非递归遍历
void preOrderByStack(Node* root)
{
	/*
		1. 定义应该移动指针，指向根部
	 
		2. 沿着左边走到空的位置 （边走边打印，并且记录指针）入栈
	 
		3. 回退（出栈）判断出栈元素是否在右边有元素，如果有，移动指针到右边，继续走2
	*/
	if (root == NULL) return;
	Node* pmove = root;
	Node* statck[100];
	int top = -1;
	while (pmove != NULL || top != -1) {
		//先走最左边，边走边打印
		while (pmove != NULL) {
			printf("%c ", pmove->data);
			statck[++top] = pmove;
			pmove = pmove->LeftChild;
		}
		if (top != -1) {
			pmove = statck[top--];
			pmove = pmove->RightChild;
		}
	}
}

void midOrderByStack(Node* root)
{
	if (root == NULL) return;
	Node* pmove = root;
	Node* statck[100];
	int top = -1;
	while (pmove != NULL || top != -1) {
		//先走最左边，边走边打印
		while (pmove != NULL) {
			statck[++top] = pmove;
			pmove = pmove->LeftChild;
		}
		if (top != -1) {
			pmove = statck[top--];
			printf("%c ", pmove->data);
			pmove = pmove->RightChild;
		}
	}
}

void lastOrderByStack(Node* root)
{
	if (root == NULL) return;
	Node* pmove = root;
	Node* statck[100];
	int top = -1;
	while (pmove != NULL || top != -1) {
		//先走最左边，边走边打印
		while (pmove != NULL) {
			statck[++top] = pmove;
			pmove = pmove->LeftChild;
		}
		if (top != -1) {
			pmove = statck[top];
			if (pmove->RightChild == NULL) {
				pmove = statck[top--];
				printf("%c ", pmove->data);
				while (top != -1 && statck[top]->RightChild == pmove) {
					pmove = statck[top--];
					printf("%c ", pmove->data);
				}
				pmove = NULL;
			}
			else {
				pmove = pmove->RightChild;
			}
		}
	}
}


//层次遍历 --> 广度优先遍历方式（BFS)
/*
void layerOrderByQueue(Node* root)
{
	Node* pmove = root;
	Node* queue[100];
	int front = 0;
	int tail = 0;
	if (pmove == NULL) return;
	queue[tail++] = pmove;
	while (front != tail) {
		pmove = queue[front++];
		printf("%c ", pmove->data);
		if (pmove->LeftChild != NULL) {
			queue[tail++] = pmove->LeftChild;
		}
		if (pmove->RightChild != NULL) {
			queue[tail++] = pmove->RightChild;
		}
	}
}
*/
void layerOrder(Node* root) {
	if (root == nullptr) {
		return;
	}

	struct LinkedListNode {
		Node* data;
		LinkedListNode* next;
		LinkedListNode(Node* data) : data(data), next(nullptr) {}
	};

	LinkedListNode* head = new LinkedListNode(root);
	LinkedListNode* tail = head;

	while (head != nullptr) {
		Node* curr = head->data;
		printf("%c ", curr->data);

		if (curr->LeftChild != nullptr) {
			tail->next = new LinkedListNode(curr->LeftChild);
			tail = tail->next;
		}

		if (curr->RightChild != nullptr) {
			tail->next = new LinkedListNode(curr->RightChild);
			tail = tail->next;
		}

		LinkedListNode* toDelete = head;
		head = head->next;
		delete toDelete;
	}
}

//递归法创建
void createTree(Node** root)
{
	char userKey = 0;
	scanf_s("%c", &userKey, 1);
	if (userKey == '#') {
		*root = NULL;
	}
	else 
	{
		*root = (Node*)malloc(sizeof(Node));
		if (*root == NULL) return;
		(*root)->data = userKey;
		createTree(&(*root)->LeftChild);
		createTree(&(*root)->RightChild);
	}
}

int main()
{
	//创建
	/*
	Node* A = createNode('A');
	Node* B = createNode('B');
	Node* C = createNode('C');
	Node* D = createNode('D');
	Node* E = createNode('E');
	Node* F = createNode('F');
	Node* G = createNode('G');
	
	//连接
	insertNode(A, B, C);
	insertNode(B, D, E);
	insertNode(C, NULL, F);
	insertNode(F, G, NULL);
	*/

	Node* A = NULL;
	createTree(&A);

	
	
	printf("前序遍历：");
	preOrder(A);
	printf("\n中序遍历：");
	midOrder(A);
	printf("\n后序遍历：");
	lastOrder(A);

	printf("\n前序非递归遍历：");
	preOrderByStack(A);
	printf("\n中序非递归遍历：");
	midOrderByStack(A);
	printf("\n后序非递归遍历：");
	lastOrderByStack(A);

	printf("\n层次遍历：");
	layerOrder(A);

	return 0;
}

