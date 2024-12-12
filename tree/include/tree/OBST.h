#ifndef TREE_OBST_H
#define TREE_OBST_H

#include "binary_search_tree.h"

// 输入需要先进行排序
BSTPtr optimalBST(const DataType data[], const WeightType weight[], int number, void **buffer);

#endif //TREE_OBST_H
