#ifndef TREE_BINARY_SEARCH_TREE_H
#define TREE_BINARY_SEARCH_TREE_H

#include "binary_tree.h"

// 输出 x < y
#define GREATER(x, y) (x > y)
#define LESS(x, y) (x < y)
#define EQUAL(x, y) (x == y)

typedef float WeightType;

typedef BinaryTree BST, *BSTPtr;

BinaryTreeNodePtr *bstFindMax(BSTPtr tree);

BinaryTreeNodePtr bstUnlinkMax(BSTPtr tree);

BinaryTreeNodePtr *bstFind(BSTPtr tree, DataType data);

void bstInsertNode(BSTPtr tree, BinaryTreeNodePtr node);

void bstInsertData(BSTPtr tree, DataType data);

BinaryTreeNodePtr bstUnlink(BSTPtr tree, BinaryTreeNodePtr *parent2child);

BinaryTreeNodePtr bstUnlinkWithData(BSTPtr tree, DataType data);

BSTPtr buildBST(const DataType data[], int len, BinaryTreeNodePtr buffer);

#endif //TREE_BINARY_SEARCH_TREE_H
