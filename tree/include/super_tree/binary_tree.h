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

BinaryTreePtr createBinaryTree_fixedCapacity(int capacity);

BinaryTreePtr createBinaryTree();

void deleteBinaryTree(BinaryTreePtr tree);

BinaryTreeNodePtr createTreeNode_fc(BinaryTreePtr tree, DataType data);

BinaryTreeNodePtr createTreeNode(DataType data);

void freeTreeNode(BinaryTreePtr tree, BinaryTreeNodePtr node);

void binaryTreeInsert_node(BinaryTreePtr tree, BinaryTreeNodePtr parent, BinaryTreeNodePtr node, int isRight);

void binaryTreeInsert_data(BinaryTreePtr tree, BinaryTreeNodePtr parent, DataType data, int isRight);

BinaryTreeNodePtr binaryTreeDelete(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight,
    BinaryTreeNodePtr if2children(BinaryTreePtr _tree, BinaryTreeNodePtr _parent, int _isRight));

void binaryTreeDeleteAndFree(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight,
    BinaryTreeNodePtr if2children(BinaryTreePtr _tree, BinaryTreeNodePtr _parent, int _isRight));

//void PrintBinaryTree(BinaryTreePtr pTree);

#endif //TREE_BINARY_TREE_H
