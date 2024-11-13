#ifndef TREE_BINARY_SEARCH_TREE_H
#define TREE_BINARY_SEARCH_TREE_H

#include "binary_tree.h"

// 输出 x < y
#define GREATER(x, y) (x > y)
#define LESS(x, y) (x < y)
#define EQUAL(x, y) (x == y)

typedef float WeightType;

typedef BinaryTree BST, *BSTPtr;

BinaryTreeNodePtr BST_findMax(BSTPtr tree);

BinaryTreeNodePtr BST_deleteMax(BSTPtr tree);

BinaryTreeNodePtr BST_find(BSTPtr tree, DataType data);

void BST_insertNode(BSTPtr tree, BinaryTreeNodePtr node);

void BST_insertData(BSTPtr tree, DataType data);

BinaryTreeNodePtr BST_deleteNode(BSTPtr tree, BinaryTreeNodePtr node, int isRight);

BinaryTreeNodePtr BST_deleteData(BSTPtr tree, DataType data);

BSTPtr buildBST(const DataType data[], int len, int isFixed);

#endif //TREE_BINARY_SEARCH_TREE_H
