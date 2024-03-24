#ifndef TREE_BINARY_TREE_H
#define TREE_BINARY_TREE_H

#define NO_TREE_NODE (-1)
#define NO_DATA_INDEX (-1)
#define NO_DATA (-1)

typedef int TreeNodeId;
typedef int DataType;
typedef int DataIndex;
//typedef int TreeKey;

typedef struct {
    TreeNodeId left;
    TreeNodeId right;
//    TreeKey key;
    DataIndex dataIdx;
    TreeNodeId nextAvailableNodeId;
}BinaryTreeNode, *BinaryTreeNodePtr;

typedef struct {
    int capacity;
    int nodeNum;
    int shouldFreeDatas;
    TreeNodeId root;
    TreeNodeId nextAvailableNodeId;
    DataIndex nextAvailableDataIdx;
    BinaryTreeNode *nodes;
    DataIndex *nextDataIdx;
    DataType *datas;
}BinaryTree, *BinaryTreePtr;

BinaryTreePtr createBinaryTreeWithoutDatas(int capacity);

BinaryTreePtr CreateBinaryTree(int capacity);

void DeleteBinaryTree(BinaryTreePtr pTree);

DataIndex createData(BinaryTreePtr pTree, DataIndex dataIdx, DataType data);

TreeNodeId createTreeNode(BinaryTreePtr pTree);

DataIndex insertData(BinaryTreePtr pTree, TreeNodeId nodeId, DataIndex dataIdx, DataType data);

TreeNodeId binaryTreeInsert(BinaryTreePtr pTree, TreeNodeId parent, int isRight, DataIndex dataIdx, DataType data);

DataIndex binaryTreeDelete(BinaryTreePtr pTree, TreeNodeId parent, int isRight);

//void PrintBinaryTree(BinaryTreePtr pTree);

#endif //TREE_BINARY_TREE_H
