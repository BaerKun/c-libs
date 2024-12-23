#ifndef TREE_BINARY_SEARCH_TREE_H
#define TREE_BINARY_SEARCH_TREE_H

#include <tree_struct.h>

// 输出 x < y
#define GREATER(x, y) (x > y)
#define LESS(x, y) (x < y)
#define EQUAL(x, y) (x == y)

TreeNodePtr *bstFindMax(TreeNodePtr *root);

extern inline TreeNodePtr bstUnlinkMax(TreeNodePtr *root);

TreeNodePtr *bstFind(TreeNodePtr *root, DataType data);

extern inline void bstInsertNode(TreeNodePtr *root, TreeNodePtr node);

extern inline void bstInsertData(TreeNodePtr *root, DataType data);

extern inline TreeNodePtr bstUnlink(TreeNodePtr *parent2child);

extern inline TreeNodePtr bstUnlinkWithData(TreeNodePtr *root, DataType data);

TreeNodePtr buildBST(const DataType data[], int len, TreeNodePtr buffer);

#endif //TREE_BINARY_SEARCH_TREE_H
