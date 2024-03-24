#include "cursor/binary_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include "cursor/macro.h"

BinaryTreePtr createBinaryTreeWithoutDatas(int capacity) {
    BinaryTreePtr pTree = malloc(sizeof(BinaryTree));

    pTree->capacity = capacity;
    pTree->nodeNum = 0;
    pTree->root = NO_TREE_NODE;
    pTree->nextAvailableNodeId = 0;
    pTree->nextAvailableDataIdx = 0;
    pTree->nodes = (BinaryTreeNodePtr) malloc(capacity * sizeof(BinaryTreeNode));
    pTree->nextDataIdx = malloc(capacity * sizeof(DataIndex));
    pTree->datas = NULL;

    for (int i = 0; i < capacity; i++) {
        pTree->nodes[i] = (BinaryTreeNode) {NO_TREE_NODE, NO_TREE_NODE, NO_DATA_INDEX, i + 1};
        pTree->nextDataIdx[i] = i + 1;
    }
    pTree->nodes[capacity - 1].nextAvailableNodeId = NO_TREE_NODE;
    pTree->nextDataIdx[capacity - 1] = NO_DATA_INDEX;

    return pTree;
}

BinaryTreePtr CreateBinaryTree(int capacity) {
    BinaryTreePtr pTree = createBinaryTreeWithoutDatas(capacity);

    pTree->shouldFreeDatas = 1;
    pTree->datas = malloc(capacity * sizeof(DataType));

    return pTree;
}

void DeleteBinaryTree(BinaryTreePtr pTree) {
    free(pTree->nodes);
    free(pTree->nextDataIdx);
    if (pTree->shouldFreeDatas)
        free(pTree->datas);
    free(pTree);
}

int takeData(BinaryTreePtr pTree, DataIndex dataIdx) {
    DataIndex this, next;

    this = pTree->nextAvailableDataIdx;
    if (this == dataIdx) {
        pTree->nextAvailableDataIdx = NEXT(this);
        return 1;
    }
    for (next = NEXT(this); next != dataIdx; this = next, next = NEXT(next)) {
        if (next == NO_DATA_INDEX)
            return 0;
    }
    NEXT(this) = NEXT(next);
    return 1;
}

DataIndex createData(BinaryTreePtr pTree, DataIndex dataIdx, DataType data) {
    DataIndex thisDataIdx;

    if (dataIdx != NO_DATA_INDEX && takeData(pTree, dataIdx)) {
        thisDataIdx = dataIdx;
    }else {
        if (pTree->nextAvailableDataIdx == NO_DATA_INDEX) {
            fputs("createData: No available data index.\n", stderr);
            return NO_DATA_INDEX;
        }

        thisDataIdx = pTree->nextAvailableDataIdx;
        pTree->nextAvailableDataIdx = NEXT(thisDataIdx);

        if (data != NO_DATA) {
            DATA(thisDataIdx) = data;
        }
    }

    NEXT(thisDataIdx) = NO_DATA_INDEX;

    return thisDataIdx;
}

DataIndex insertData(BinaryTreePtr pTree, TreeNodeId nodeId, DataIndex dataIdx, DataType data) {
    DataIndex inDataIdx;

    if (nodeId == NO_TREE_NODE)
        return NO_DATA_INDEX;

    inDataIdx = createData(pTree, dataIdx, data);

    NEXT(inDataIdx) = DATAIDX(nodeId);
    DATAIDX(nodeId) = inDataIdx;

    return inDataIdx;
}

void freeData(BinaryTreePtr pTree, DataIndex dataIdx) {
    NEXT(dataIdx) = pTree->nextAvailableDataIdx;
    pTree->nextAvailableDataIdx = dataIdx;
}

DataIndex deleteData(BinaryTreePtr pTree, TreeNodeId nodeId) {
    DataIndex dataIdx;

    if (nodeId == NO_TREE_NODE)
        return NO_DATA_INDEX;

    dataIdx = DATAIDX(nodeId);

    if (dataIdx == NO_DATA_INDEX)
        return NO_DATA_INDEX;

    DATAIDX(nodeId) = NEXT(dataIdx);
    freeData(pTree, dataIdx);

    return dataIdx;
}

TreeNodeId createTreeNode(BinaryTreePtr pTree) {
    TreeNodeId nodeId = pTree->nextAvailableNodeId;

    if (nodeId == NO_TREE_NODE) {
        fputs("createTreeNode: Tree is full!\n", stderr);
        return NO_TREE_NODE;
    }
    pTree->nextAvailableNodeId = NODE(nodeId).nextAvailableNodeId;

    LEFT(nodeId) = RIGHT(nodeId) = NO_TREE_NODE;
    DATAIDX(nodeId) = NO_TREE_NODE;
    pTree->nodeNum++;

    return nodeId;
}

TreeNodeId binaryTreeInsert(BinaryTreePtr pTree, TreeNodeId parent, int isRight, DataIndex dataIdx, DataType data) {
    if (parent == NO_TREE_NODE) {
        if (pTree->root == NO_TREE_NODE)
            pTree->root = createTreeNode(pTree);
        insertData(pTree, pTree->root, dataIdx, data);
        return pTree->root;
    }

    if (isRight) {
        if (RIGHT(parent) == NO_TREE_NODE) {
            RIGHT(parent) = createTreeNode(pTree);
        }
        insertData(pTree, RIGHT(parent), dataIdx, data);
        return RIGHT(parent);
    }

    if (LEFT(parent) == NO_TREE_NODE) {
        LEFT(parent) = createTreeNode(pTree);
    }
    insertData(pTree, LEFT(parent), dataIdx, data);
    return LEFT(parent);
}

void freeTreeNode(BinaryTreePtr pTree, TreeNodeId nodeId) {
    NODE(nodeId).nextAvailableNodeId = pTree->nextAvailableNodeId;
    pTree->nextAvailableNodeId = nodeId;
}

TreeNodeId deleteBinaryTreeNode(BinaryTreePtr pTree, TreeNodeId nodeId) {
    TreeNodeId outNodeId;

    if (LEFT(nodeId) == NO_TREE_NODE) {
        outNodeId = RIGHT(nodeId);
        freeTreeNode(pTree, nodeId);
        pTree->nodeNum--;
    } else if (RIGHT(nodeId) == NO_TREE_NODE) {
        outNodeId = LEFT(nodeId);
        freeTreeNode(pTree, nodeId);
        pTree->nodeNum--;
    } else {
        outNodeId = nodeId;
    }

    return outNodeId;
}

DataIndex binaryTreeDelete(BinaryTreePtr pTree, TreeNodeId parent, int isRight) {
    TreeNodeId thisNodeId;
    DataIndex outDataIdx;

    if (parent == NO_TREE_NODE) {
        thisNodeId = pTree->root;
    } else {
        thisNodeId = (isRight) ? RIGHT(parent) : LEFT(parent);
    }

    if (thisNodeId == NO_TREE_NODE)
        return NO_DATA_INDEX;

    outDataIdx = deleteData(pTree, thisNodeId);
    if (DATAIDX(thisNodeId) == NO_DATA_INDEX) {
        thisNodeId = deleteBinaryTreeNode(pTree, thisNodeId);
        if (parent == NO_TREE_NODE) {
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