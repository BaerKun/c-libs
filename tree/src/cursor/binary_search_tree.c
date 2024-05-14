#include "cursor/binary_search_tree.h"
#include "cursor/macro.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int getIsRight(BSTPtr pTree, TreeNodePtr parent, const DataType data) {
    if(parent == DATA_NULLPTR || LESS(data, DATA(DATAPTR(parent))))
        return 0;
    else
        return 1;
}

DataPtr bstFindMax(BSTPtr pTree) {
    TreeNodePtr nodeId;

    for(nodeId = pTree->root; RIGHT(nodeId) != TREE_NODE_NULLPTR; nodeId = RIGHT(nodeId));

    return DATAPTR(nodeId);
}

DataPtr bstDeleteMax(BSTPtr pTree) {
    TreeNodePtr parent, child;

    parent = DATA_NULLPTR;
    child = pTree->root;
    for(; RIGHT(child) != TREE_NODE_NULLPTR; parent = child, child = RIGHT(child));

    return binaryTreeDelete(pTree, parent, 1);
}

static TreeNodePtr bstFindParent(BSTPtr pTree, DataType data) {
    TreeNodePtr nodeId, nextNodeId;

    for (nodeId = TREE_NODE_NULLPTR, nextNodeId = pTree->root; nextNodeId != TREE_NODE_NULLPTR;) {
        if (LESS(data, DATA(DATAPTR(nextNodeId)))) {
            nodeId = nextNodeId;
            nextNodeId = LEFT(nextNodeId);
        } else if (GREATER(data, DATA(DATAPTR(nextNodeId)))) {
            nodeId = nextNodeId;
            nextNodeId = RIGHT(nextNodeId);
        } else {
            break;
        }
    }

    return nodeId;
}

void bstInsert_ptr(BSTPtr pTree, DataPtr dataPtr) {
    TreeNodePtr parent;

    DataType data = DATA(dataPtr);

    parent = bstFindParent(pTree, data);

    binaryTreeInsert_ptr(pTree, parent, getIsRight(pTree, parent, data), dataPtr);
}

void bstInsert_val(BSTPtr pTree, const DataType data){
    TreeNodePtr parent;

    parent = bstFindParent(pTree, data);

    binaryTreeInsert_val(pTree, parent, getIsRight(pTree, parent, data), data);
}

static DataPtr bstDeleteRightMin(BSTPtr pTree, TreeNodePtr nodeId) {
    TreeNodePtr this, next;
    DataPtr min;

    this = nodeId;
    next = RIGHT(nodeId);
    while (LEFT(next) != TREE_NODE_NULLPTR) {
        this = next;
        next = LEFT(next);
    }

    min = DATAPTR(next);
    if(this == nodeId)
        binaryTreeDelete(pTree, this, 1);
    else
        binaryTreeDelete(pTree, this, 0);

    return min;
}

DataPtr bstDelete(BSTPtr pTree, DataType data) {
    TreeNodePtr parentId, nodeId;
    DataPtr dataPtr;
    int isRight;

    parentId = bstFindParent(pTree, data);

    dataPtr = binaryTreeDelete(pTree, parentId, isRight = getIsRight(pTree, parentId, data));

    if(dataPtr == DATA_NULLPTR)
        return DATA_NULLPTR;

    if(parentId == TREE_NODE_NULLPTR)
        nodeId = pTree->root;
    else if(isRight)
        nodeId = RIGHT(parentId);
    else
        nodeId = LEFT(parentId);

    if(nodeId != DATA_NULLPTR && DATAPTR(nodeId) == DATA_NULLPTR){
        DATAPTR(nodeId) = createData_ptr(pTree, bstDeleteRightMin(pTree, nodeId));
    }

    return dataPtr;
}

TreeNodePtr bstFind(BSTPtr pTree, DataType data) {
    TreeNodePtr nodeId;

    for(nodeId = pTree->root; nodeId != TREE_NODE_NULLPTR;){
        if(LESS(data, DATA(DATAPTR(nodeId))))
            nodeId = LEFT(nodeId);
        else if(GREATER(data, DATA(DATAPTR(nodeId))))
            nodeId = RIGHT(nodeId);
        else
            break;
    }

    return nodeId;
}

static int median(const DataType *array, int len) {
    int b[5] = {0, len / 4, len / 2, len * 3 / 4, len - 1};
    int i, j, tmp;
    for(i = 0; i < 4; i++) {
        for (j = 3; j >= i; j--){
            if(LESS(array[b[j]], array[b[j + 1]])){
                tmp = b[j];
                b[j] = b[j + 1];
                b[j + 1] = tmp;
            }
        }
    }

    return b[2];
}

BSTPtr buildBST(DataType data[], int len, int capacity, int pleaseCopy) {
    BSTPtr pTree = createBinaryTreeWithoutData(capacity);

    if(pleaseCopy) {
        pTree->shouldFreeData = 1;
        pTree->data = (DataType *)malloc(capacity * sizeof(DataType));
        memcpy(pTree->data, data, len * sizeof(DataType));
    } else{
        pTree->shouldFreeData = 0;
        pTree->data = data;
    }

    DataPtr middle = median(data, len);

    bstInsert_ptr(pTree, middle);

    for(int i = 0; i < len; i++){
        if(i != middle)
            bstInsert_ptr(pTree, i);
    }

    return pTree;
}