#ifndef TREE_BINARY_TREE_H
#define TREE_BINARY_TREE_H

#define MOST_TREE_NODES 0x7FFFFFFF

typedef int DataType;
typedef struct BinaryTreeNode BinaryTreeNode, *BinaryTreeNodePtr;

struct BinaryTreeNode {
    BinaryTreeNodePtr left;
    BinaryTreeNodePtr right;
    BinaryTreeNodePtr next;
    DataType data;
};

typedef struct {
    int nodeNum;
    BinaryTreeNodePtr root;
} BinaryTree, *BinaryTreePtr;

BinaryTreePtr newBinaryTree();

static void deleteTreeNodeRecursively(BinaryTreeNodePtr root);

void btDestroy(BinaryTreePtr tree);

BinaryTreeNodePtr btNewNode(DataType data);

BinaryTreeNodePtr btUnlink(BinaryTreePtr tree, BinaryTreeNodePtr *parent2child,
                           BinaryTreeNodePtr (*if2children)(BinaryTreePtr, BinaryTreeNodePtr *));

void btDeleteNode(BinaryTreePtr tree, BinaryTreeNodePtr *parent2child,
                  BinaryTreeNodePtr (*if2children)(BinaryTreePtr, BinaryTreeNodePtr *));

void btInsertNode(BinaryTreePtr tree, BinaryTreeNodePtr *parent2child, BinaryTreeNodePtr node);

void btInsertData(BinaryTreePtr tree, BinaryTreeNodePtr *parent2child, DataType data);

#endif //TREE_BINARY_TREE_H
