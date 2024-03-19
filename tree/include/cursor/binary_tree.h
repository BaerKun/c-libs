#ifndef TREE_BINARY_TREE_H
#define TREE_BINARY_TREE_H

#define NO_TREE_NODE (-1)

#define NO_DATA 0

#define COMPARE

typedef int TreeNodeId;
typedef char * DataType;

typedef struct {
    TreeNodeId left;
    TreeNodeId right;
    DataType data;
}BinaryTreeNode, *BinaryTreeNodePtr;

typedef struct {
    TreeNodeId root;
    int capacity;
    int nodeNum;
    BinaryTreeNodePtr nodes;
}BinaryTree, *BinaryTreePtr;

BinaryTreePtr CreateBinaryTree(int capacity);

void InsertNode(BinaryTreePtr pTree, TreeNodeId fatherId, DataType data);

void DeleteBinaryTree(BinaryTreePtr pTree);

TreeNodeId CreateTreeNode(BinaryTreePtr pTree, DataType data);

//void PrintBinaryTree(BinaryTreePtr pTree);

#endif //TREE_BINARY_TREE_H
