#ifndef TREE_BINARY_TREE_H
#define TREE_BINARY_TREE_H

#define MOST_TREE_NODES 0x7FFFFFFF

typedef int DataType;
typedef struct BinaryTreeNode BinaryTreeNode, *BinaryTreeNodePtr;

struct BinaryTreeNode{
    BinaryTreeNodePtr left;
    BinaryTreeNodePtr right;
    BinaryTreeNodePtr next;
    BinaryTreeNodePtr nextEmpty;
    int isEmpty;
    DataType data;
};

typedef struct {
    int capacity;
    int nodeNum;
    BinaryTreeNodePtr memoryPool;
    BinaryTreeNodePtr root;
}BinaryTree, *BinaryTreePtr;

BinaryTreePtr newBinaryTree_fixedCapacity(int capacity);

BinaryTreePtr newBinaryTree();

void BT_destroy(BinaryTreePtr tree);

BinaryTreeNodePtr BT_newNode_fc(BinaryTreePtr tree, DataType data);

BinaryTreeNodePtr BT_newNode(DataType data);

void BT_freeNode(BinaryTreePtr tree, BinaryTreeNodePtr node);

void BT_insertNode(BinaryTreePtr tree, BinaryTreeNodePtr parent, BinaryTreeNodePtr node, int isRight);

void BT_insertData(BinaryTreePtr tree, BinaryTreeNodePtr parent, DataType data, int isRight);

BinaryTreeNodePtr BT_deleteNode(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight,
                                BinaryTreeNodePtr if2children(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight));

void BT_deleteAndFree(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight,
                      BinaryTreeNodePtr if2children(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight));

//void PrintBinaryTree(BinaryTreePtr pTree);

#endif //TREE_BINARY_TREE_H
