#ifndef TREE_BINARY_SEARCH_TREE_H
#define TREE_BINARY_SEARCH_TREE_H

#include "binary_tree.h"

// 输出 x < y
#define GREATER(x, y) (x > y)
#define LESS(x, y) (x < y)
#define EQUAL(x, y) (x == y)

typedef float WeightType;

typedef BinaryTree BST, *BSTPtr;

DataPtr bstFindMax(BSTPtr pTree);

DataPtr bstDeleteMax(BSTPtr pTree);

TreeNodePtr bstFind(BSTPtr pTree, DataType data);

void bstInsert_ptr(BSTPtr pTree, DataPtr dataPtr);

void bstInsert_val(BSTPtr pTree, DataType data);

DataPtr bstDelete(BSTPtr pTree, DataType data);

BSTPtr buildBST(DataType data[], int len, int capacity, int pleaseCopy);

#endif //TREE_BINARY_SEARCH_TREE_H
