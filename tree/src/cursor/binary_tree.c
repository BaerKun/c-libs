#include "cursor/binary_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include "cursor/macro.h"

BinaryTreePtr createBinaryTreeWithoutData(int capacity) {
    BinaryTreePtr pTree = malloc(sizeof(BinaryTree));

    pTree->capacity = capacity;
    pTree->nodeNum = 0;
    pTree->root = TREE_NODE_NULLPTR;
    pTree->nextAvailableNodePtr = 0;
    pTree->nextAvailableDataPtr = 0;
    pTree->nodes = (BinaryTreeNodePtr) malloc(capacity * sizeof(BinaryTreeNode));
    pTree->nextDataPtr = malloc(capacity * sizeof(DataPtr));
    pTree->data = NULL;

    for (int i = 0; i < capacity; i++) {
        pTree->nodes[i] = (BinaryTreeNode) {TREE_NODE_NULLPTR, TREE_NODE_NULLPTR, DATA_NULLPTR, i + 1};
        pTree->nextDataPtr[i] = i + 1;
    }
    pTree->nodes[capacity - 1].nextAvailableNodePtr = TREE_NODE_NULLPTR;
    pTree->nextDataPtr[capacity - 1] = DATA_NULLPTR;

    return pTree;
}

BinaryTreePtr CreateBinaryTree(int capacity) {
    BinaryTreePtr pTree = createBinaryTreeWithoutData(capacity);

    pTree->shouldFreeData = 1;
    pTree->data = malloc(capacity * sizeof(DataType));

    return pTree;
}

void DeleteBinaryTree(BinaryTreePtr pTree) {
    free(pTree->nodes);
    free(pTree->nextDataPtr);
    if (pTree->shouldFreeData)
        free(pTree->data);
    free(pTree);
}

static int takeData(BinaryTreePtr pTree, DataPtr dataIdx) {
    DataPtr this, next;

    this = pTree->nextAvailableDataPtr;
    if (this == dataIdx) {
        pTree->nextAvailableDataPtr = NEXT(this);
        return 1;
    }
    for (next = NEXT(this); next != dataIdx; this = next, next = NEXT(next)) {
        if (next == DATA_NULLPTR)
            return 0;
    }
    NEXT(this) = NEXT(next);
    return 1;
}

DataPtr createData_ptr(BinaryTreePtr pTree, DataPtr dataPtr){
    if(takeData(pTree, dataPtr)){
        NEXT(dataPtr) = DATA_NULLPTR;
        return dataPtr;
    }
    return DATA_NULLPTR;
}

DataPtr createData_val(BinaryTreePtr pTree, const DataType data){
    if (pTree->nextAvailableDataPtr == DATA_NULLPTR) {
        fputs("createData: No available data index.\n", stderr);
        return DATA_NULLPTR;
    }

    DataPtr thisDataIdx = pTree->nextAvailableDataPtr;
    pTree->nextAvailableDataPtr = NEXT(thisDataIdx);
    DATA(thisDataIdx) = data;

    return thisDataIdx;
}

//DataPtr createData(BinaryTreePtr pTree, DataPtr dataIdx, DataType data) {
//    DataPtr thisDataIdx;
//
//    if (dataIdx != DATA_NULLPTR && takeData(pTree, dataIdx)) {
//        thisDataIdx = dataIdx;
//    }else {
//        if (pTree->nextAvailableDataPtr == DATA_NULLPTR) {
//            fputs("createData: No available data index.\n", stderr);
//            return DATA_NULLPTR;
//        }
//
//        thisDataIdx = pTree->nextAvailableDataPtr;
//        pTree->nextAvailableDataPtr = NEXT(thisDataIdx);
//
//        if (data != NO_DATA) {
//            DATA(thisDataIdx) = data;
//        }
//    }
//
//    NEXT(thisDataIdx) = DATA_NULLPTR;
//
//    return thisDataIdx;
//}

void insertData_ptr(BinaryTreePtr pTree, TreeNodePtr nodePtr, DataPtr dataPtr){
    if(createData_ptr(pTree, dataPtr) == DATA_NULLPTR)
        return;

    NEXT(dataPtr) = DATAPTR(nodePtr);
    DATAPTR(nodePtr) = dataPtr;
}

void insertData_val(BinaryTreePtr pTree, TreeNodePtr nodePtr, const DataType data){
    DataPtr dataPtr = createData_val(pTree, data);

    if(dataPtr == DATA_NULLPTR)
        return;

    NEXT(dataPtr) = DATAPTR(nodePtr);
    DATAPTR(nodePtr) = dataPtr;
}

//DataPtr insertData(BinaryTreePtr pTree, TreeNodePtr nodeId, DataPtr dataIdx, DataType data) {
//    DataPtr inDataIdx;
//
//    if (nodeId == TREE_NODE_NULLPTR) {
//        fputs("insertData: No node to insert.", stderr);
//        return DATA_NULLPTR;
//    }
//
//    inDataIdx = createData(pTree, dataIdx, data);
//
//    NEXT(inDataIdx) = DATAPTR(nodeId);
//    DATAPTR(nodeId) = inDataIdx;
//
//    return inDataIdx;
//}

static void freeData(BinaryTreePtr pTree, DataPtr dataIdx) {
    NEXT(dataIdx) = pTree->nextAvailableDataPtr;
    pTree->nextAvailableDataPtr = dataIdx;
}

DataPtr deleteData(BinaryTreePtr pTree, TreeNodePtr nodeId) {
    DataPtr dataIdx;

    if (nodeId == TREE_NODE_NULLPTR)
        return DATA_NULLPTR;

    dataIdx = DATAPTR(nodeId);

    if (dataIdx == DATA_NULLPTR)
        return DATA_NULLPTR;

    DATAPTR(nodeId) = NEXT(dataIdx);
    freeData(pTree, dataIdx);

    return dataIdx;
}

TreeNodePtr createTreeNode(BinaryTreePtr pTree) {
    TreeNodePtr nodeId = pTree->nextAvailableNodePtr;

    if (nodeId == TREE_NODE_NULLPTR) {
        fputs("createTreeNode: Tree is full!\n", stderr);
        return TREE_NODE_NULLPTR;
    }
    pTree->nextAvailableNodePtr = NODE(nodeId).nextAvailableNodePtr;

    LEFT(nodeId) = RIGHT(nodeId) = TREE_NODE_NULLPTR;
    DATAPTR(nodeId) = TREE_NODE_NULLPTR;
    pTree->nodeNum++;

    return nodeId;
}

void binaryTreeInsert_ptr(BinaryTreePtr pTree, TreeNodePtr parent, int isRight, DataPtr dataPtr){
    if(parent == TREE_NODE_NULLPTR){
        if(pTree->root == TREE_NODE_NULLPTR)
            pTree->root = createTreeNode(pTree);
        insertData_ptr(pTree, pTree->root, dataPtr);
        return;
    }

    if(isRight){
        if(RIGHT(parent) == TREE_NODE_NULLPTR)
            RIGHT(parent) = createTreeNode(pTree);
        insertData_ptr(pTree, RIGHT(parent), dataPtr);
    } else{
        if(LEFT(parent) == TREE_NODE_NULLPTR)
            LEFT(parent) = createTreeNode(pTree);
        insertData_ptr(pTree, LEFT(parent), dataPtr);
    }
}

void binaryTreeInsert_val(BinaryTreePtr pTree, TreeNodePtr parent, int isRight, const DataType data){
    binaryTreeInsert_ptr(pTree, parent, isRight, createData_val(pTree, data));
}

//TreeNodePtr binaryTreeInsert(BinaryTreePtr pTree, TreeNodePtr parent, int isRight, DataPtr dataIdx, DataType data) {
//    if (parent == TREE_NODE_NULLPTR) {
//        if (pTree->root == TREE_NODE_NULLPTR)
//            pTree->root = createTreeNode(pTree);
//        insertData(pTree, pTree->root, dataIdx, data);
//        return pTree->root;
//    }
//
//    if (isRight) {
//        if (RIGHT(parent) == TREE_NODE_NULLPTR) {
//            RIGHT(parent) = createTreeNode(pTree);
//        }
//        insertData(pTree, RIGHT(parent), dataIdx, data);
//        return RIGHT(parent);
//    }
//
//    if (LEFT(parent) == TREE_NODE_NULLPTR) {
//        LEFT(parent) = createTreeNode(pTree);
//    }
//    insertData(pTree, LEFT(parent), dataIdx, data);
//    return LEFT(parent);
//}

static void freeTreeNode(BinaryTreePtr pTree, TreeNodePtr nodeId) {
    NODE(nodeId).nextAvailableNodePtr = pTree->nextAvailableNodePtr;
    pTree->nextAvailableNodePtr = nodeId;
}

static TreeNodePtr deleteBinaryTreeNode(BinaryTreePtr pTree, TreeNodePtr nodeId) {
    TreeNodePtr outNodeId;

    if (LEFT(nodeId) == TREE_NODE_NULLPTR) {
        outNodeId = RIGHT(nodeId);
        freeTreeNode(pTree, nodeId);
        pTree->nodeNum--;
    } else if (RIGHT(nodeId) == TREE_NODE_NULLPTR) {
        outNodeId = LEFT(nodeId);
        freeTreeNode(pTree, nodeId);
        pTree->nodeNum--;
    } else {
        outNodeId = nodeId;
    }

    return outNodeId;
}

DataPtr binaryTreeDelete(BinaryTreePtr pTree, TreeNodePtr parent, int isRight) {
    TreeNodePtr thisNodeId;
    DataPtr outDataIdx;

    if (parent == TREE_NODE_NULLPTR) {
        thisNodeId = pTree->root;
    } else {
        thisNodeId = (isRight) ? RIGHT(parent) : LEFT(parent);
    }

    if (thisNodeId == TREE_NODE_NULLPTR)
        return DATA_NULLPTR;

    outDataIdx = deleteData(pTree, thisNodeId);
    if (DATAPTR(thisNodeId) == DATA_NULLPTR) {
        thisNodeId = deleteBinaryTreeNode(pTree, thisNodeId);
        if (parent == TREE_NODE_NULLPTR) {
            pTree->root = thisNodeId;
        } else {
            if (isRight)
                RIGHT(parent) = thisNodeId;
            else
                LEFT(parent) = thisNodeId;
        }
    }

    return outDataIdx;
}
