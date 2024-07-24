#ifndef TREE_BINARY_SEARCH_TREE_H
#define TREE_BINARY_SEARCH_TREE_H

#include "binary_tree.h"

// 输出 x < y
#define GREATER(x, y) (x > y)
#define LESS(x, y) (x < y)
#define EQUAL(x, y) (x == y)

typedef float WeightType;

typedef BinaryTree BST, *BSTPtr;

BinaryTreeNodePtr bstFindMax(BSTPtr tree);

BinaryTreeNodePtr bstDeleteMax(BSTPtr tree);

BinaryTreeNodePtr bstFind(BSTPtr tree, DataType data);

void bstInsert_node(BSTPtr tree, BinaryTreeNodePtr node);

void bstInsert_data(BSTPtr tree, DataType data);

BinaryTreeNodePtr bstDelete_node(BSTPtr tree, BinaryTreeNodePtr node, int isRight);

BinaryTreeNodePtr bstDelete_data(BSTPtr tree, DataType data);

BSTPtr buildBST(DataType data[], int len, int isFixed);

#endif //TREE_BINARY_SEARCH_TREE_H
