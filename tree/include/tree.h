#ifndef TREE_H
#define TREE_H

#include <tree_struct.h>

extern inline TreeNodePtr treeNewNode(DataType data);

extern inline void treeInsertNode(TreeNodePtr *node2child, TreeNodePtr node);

extern inline void treeInsertData(TreeNodePtr *node2child, DataType data);

extern inline TreeNodePtr treeUnlink(TreeNodePtr *node2child);

void treeDestroy(TreeNodePtr root);

#endif //TREE_H
