# 0410-BTree
Self Learning of Data Structure &amp; Alogorithm -- B Tree

Just a note taking for my study plan in data structure

## Self Learning Data Structure & Algorithm

### What is B Tree?
A B-tree is a self-balancing tree data structure (multiplexed lookup tree) that stores associative arrays (key-value) and provides fast lookup, insertion, and deletion operations.A B-tree is characterized by the fact that each node can have multiple children (the largest number of children of a node in the tree is called the order of the B-tree pair) and the children are ordered in size among themselves, which enables fast lookups.The height of a B-tree is usually The height of B-trees is usually low, so the time complexity of lookup, insertion and deletion operations is O(log n).

<hr>

### Traversal in B Tree
<b>B-tree traversal</b> usually consists of three types: pre-order traversal, mid-order traversal and post-order traversal

- Pre-order traversal: Visit the root node first, then recursively traverse the left and right subtrees first.
```c++
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
```

- Middle-order traversal: first recursively traverses the left subtree in middle order, then visits the root node, and finally recursively traverses the right subtree in middle order.
```c++
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
```

- Post-order traversal: recursively traverses the left and right subtrees, then accesses the root node.
```c++
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
```
<b>B-tree traversal</b> is different from <b>Binary Tree</b> because B-tree nodes may have multiple children, so B-tree traversal needs to be performed according to specific rules.

For example, for a B-tree node which contains n keywords and n+1 pointers to subtrees. For a non-leaf node, the number of sub-tree pointers is always 1 more than the number of keywords. in the middle-order traversal, we traverse the keywords in the order from smallest to largest, so a non-leaf node needs to be traversed in the following way:
- traversing subtree 0;
- traversing keyword 0;
- traversing subtree 1;
- traversing keyword 1;
- and so on, traversing through all subtrees and keywords.

For leaf nodes, they do not have subtrees, so they only need to be traversed in the order of the keywords.
<hr>

### Insert, Delete and Search in B Tree
- Insert
```c++
void insertValueInBTree(int value)
{
  int flag, i;
  BTreeNode* child;

  flag = setValueIntoNode(value, &i, root, &child);
  if (flag)
    root = createNode(i, child);
}
```

- Delete
```c++
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
```

- Search
```c++
void searchValueInBTree(int value, int* position, BTreeNode* myNode)
{
  if (!myNode) return;

  if (value < myNode->value[1]) *position = 0;
  else
  {
    for (*position = myNode->count;
      (value < myNode->value[*position] && *position > 1);
      (*position)--);

      if (value == myNode->value[*position])
      {
        cout << "The value " << value << " is present in the B-Tree" << endl << endl;
        return;
      }

      cout << "The value " << value << " is not present in the B-Tree" << endl << endl;

  }
  searchValueInBTree(value, position, myNode->child[*position]);
  return;
}
```

### How the nodes/leaves shift
For understaning easily, you may refer the website: [https://www.cs.usfca.edu/~galles/visualization/BTree.html](https://www.cs.usfca.edu/~galles/visualization/BTree.html)
