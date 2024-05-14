#ifndef TREE_BINARY_TREE_H
#define TREE_BINARY_TREE_H

#define TREE_NODE_NULLPTR (-1)
#define DATA_NULLPTR (-1)
#define NO_DATA (-1)

typedef int TreeNodePtr;
typedef int DataType;
typedef int DataPtr;
//typedef int TreeKey;

typedef struct {
    TreeNodePtr left;
    TreeNodePtr right;
//    TreeKey key;
    DataPtr dataPtr;
    TreeNodePtr nextAvailableNodePtr;
}BinaryTreeNode, *BinaryTreeNodePtr;

typedef struct {
    int capacity;
    int nodeNum;
    int shouldFreeData;
    TreeNodePtr root;
    TreeNodePtr nextAvailableNodePtr;
    DataPtr nextAvailableDataPtr;
    BinaryTreeNode *nodes;
    DataPtr *nextDataPtr;
    DataType *data;
}BinaryTree, *BinaryTreePtr;

BinaryTreePtr createBinaryTreeWithoutData(int capacity);

BinaryTreePtr CreateBinaryTree(int capacity);

void DeleteBinaryTree(BinaryTreePtr pTree);

DataPtr createData_ptr(BinaryTreePtr pTree, DataPtr dataPtr);

DataPtr createData_val(BinaryTreePtr pTree, DataType data);

TreeNodePtr createTreeNode(BinaryTreePtr pTree);

void insertData_ptr(BinaryTreePtr pTree, TreeNodePtr nodeId, DataPtr dataPtr);

void insertData_val(BinaryTreePtr pTree, TreeNodePtr nodePtr, DataType data);

DataPtr deleteData(BinaryTreePtr pTree, TreeNodePtr nodeId);

void binaryTreeInsert_ptr(BinaryTreePtr pTree, TreeNodePtr parent, int isRight, DataPtr dataPtr);

void binaryTreeInsert_val(BinaryTreePtr pTree, TreeNodePtr parent, int isRight, DataType data);

DataPtr binaryTreeDelete(BinaryTreePtr pTree, TreeNodePtr parent, int isRight);

//void PrintBinaryTree(BinaryTreePtr pTree);

#endif //TREE_BINARY_TREE_H
