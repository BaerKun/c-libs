#ifndef TREE_BINARY_SEARCH_TREE_H
#define TREE_BINARY_SEARCH_TREE_H

#include "binary_tree.h"

// 输出 x < y
#define GREATER(x, y) (x > y)
#define LESS(x, y) (x < y)
#define EQUAL(x, y) (x == y)

typedef float WeightType;

typedef BinaryTree BST, *BSTPtr;

DataIndex bstFindMax(BSTPtr pTree);

DataIndex bstDeleteMax(BSTPtr pTree);

TreeNodeId bstFind(BSTPtr pTree, DataType data);

void bstInsert(BSTPtr pTree, DataIndex dataIdx, DataType data);

int bstDelete(BSTPtr pTree, DataType data);

BSTPtr buildBST(DataType datas[],int len, int capacity, int pleaseCopy);

#endif //TREE_BINARY_SEARCH_TREE_H
