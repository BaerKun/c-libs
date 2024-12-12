#ifndef TREE_BINARY_TREE_H
#define TREE_BINARY_TREE_H

#define MOST_TREE_NODES 0x7FFFFFFF

typedef int DataType;
typedef struct BinaryTreeNode BinaryTreeNode, *BinaryTreeNodePtr;

struct BinaryTreeNode{
    BinaryTreeNodePtr left;
    BinaryTreeNodePtr right;
    BinaryTreeNodePtr next;
    DataType data;
};

typedef struct {
    int nodeNum;
    BinaryTreeNodePtr root;
}BinaryTree, *BinaryTreePtr;

BinaryTreePtr newBinaryTree();

static void deleteTreeNodeRecursively(BinaryTreeNodePtr root);

void btDestroy(BinaryTreePtr tree);

BinaryTreeNodePtr btNewNode(DataType data);

BinaryTreeNodePtr *btGetChild(BinaryTreePtr tree,  BinaryTreeNodePtr parent, int isRight);

BinaryTreeNodePtr btUnlink(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight,
                                BinaryTreeNodePtr (*if2children)(BinaryTreePtr, BinaryTreeNodePtr, int));

void btDeleteNode(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight,
                      BinaryTreeNodePtr (*if2children)(BinaryTreePtr, BinaryTreeNodePtr, int));

void btInsertNode(BinaryTreePtr tree, BinaryTreeNodePtr parent, BinaryTreeNodePtr node, int isRight);

void btInsertData(BinaryTreePtr tree, BinaryTreeNodePtr parent, DataType data, int isRight);

#endif //TREE_BINARY_TREE_H
