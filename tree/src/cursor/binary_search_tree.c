#include "cursor/binary_search_tree.h"
#include "cursor/macro.h"
#include <stdlib.h>
#include <string.h>

int getIsRight(BSTPtr pTree, TreeNodeId parent, DataType data) {
    if(parent == NO_DATA_INDEX || LESS(data, DATA(DATAIDX(parent))))
        return 0;
    else
        return 1;
}

DataIndex bstFindMax(BSTPtr pTree) {
    TreeNodeId nodeId;

    for(nodeId = pTree->root; RIGHT(nodeId) != NO_TREE_NODE; nodeId = RIGHT(nodeId));

    return DATAIDX(nodeId);
}

DataIndex bstDeleteMax(BSTPtr pTree) {
    TreeNodeId parent, child;

    parent = NO_DATA_INDEX;
    child = pTree->root;
    for(; RIGHT(child) != NO_TREE_NODE; parent = child, child = RIGHT(child));

    return binaryTreeDelete(pTree, parent, 1);
}

TreeNodeId bstFindParent(BSTPtr pTree, DataType data) {
    TreeNodeId nodeId, nextNodeId;

    for (nodeId = NO_TREE_NODE, nextNodeId = pTree->root; nextNodeId != NO_TREE_NODE;) {
        if (LESS(data, DATA(DATAIDX(nextNodeId)))) {
            nodeId = nextNodeId;
            nextNodeId = LEFT(nextNodeId);
        } else if (GREATER(data, DATA(DATAIDX(nextNodeId)))) {
            nodeId = nextNodeId;
            nextNodeId = RIGHT(nextNodeId);
        } else {
            break;
        }
    }

    return nodeId;
}

void bstInsert(BSTPtr pTree, DataIndex dataIdx, DataType data) {
    TreeNodeId parentId;

    if(dataIdx != NO_DATA_INDEX)
        data = DATA(dataIdx);

    parentId = bstFindParent(pTree, data);

    binaryTreeInsert(pTree, parentId, getIsRight(pTree, parentId, data), dataIdx, data);
}

DataIndex bstDeleteRightMin(BSTPtr pTree, TreeNodeId nodeId) {
    TreeNodeId this, next;
    DataIndex min;

    this = nodeId;
    next = RIGHT(nodeId);
    while (LEFT(next) != NO_TREE_NODE) {
        this = next;
        next = LEFT(next);
    }

    min = DATAIDX(next);
    if(this == nodeId)
        binaryTreeDelete(pTree, this, 1);
    else
        binaryTreeDelete(pTree, this, 0);

    return min;
}

int bstDelete(BSTPtr pTree, DataType data) {
    TreeNodeId parentId, nodeId;
    int isRight;

    parentId = bstFindParent(pTree, data);

    if(NO_DATA_INDEX == binaryTreeDelete(pTree, parentId, isRight = getIsRight(pTree, parentId, data)))
        return 0;

    if(isRight)
        nodeId = RIGHT(parentId);
    else if(parentId == NO_TREE_NODE)
        nodeId = pTree->root;
    else
        nodeId = LEFT(parentId);

    if(DATAIDX(nodeId) == NO_DATA_INDEX){
        DATAIDX(nodeId) = createData(pTree, bstDeleteRightMin(pTree, nodeId), NO_DATA);
    }

    return 1;
}

TreeNodeId bstFind(BSTPtr pTree, DataType data) {
    TreeNodeId nodeId;

    for(nodeId = pTree->root; nodeId != NO_TREE_NODE;){
        if(LESS(data, DATA(DATAIDX(nodeId))))
            nodeId = LEFT(nodeId);
        else if(GREATER(data, DATA(DATAIDX(nodeId))))
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

BSTPtr buildBST(DataType datas[], int len, int capacity, int pleaseCopy) {
    BSTPtr pTree = createBinaryTreeWithoutDatas(capacity);

    if(pleaseCopy) {
        pTree->shouldFreeDatas = 1;
        pTree->datas = (DataType *)malloc(capacity * sizeof(DataType));
        memcpy(pTree->datas, datas, len * sizeof(DataType));
    } else{
        pTree->shouldFreeDatas = 0;
        pTree->datas = datas;
    }

    DataIndex middle = median(datas, len);

    bstInsert(pTree, middle, NO_DATA);

    for(int i = 0; i < len; i++){
        if(i != middle)
            bstInsert(pTree, i, NO_DATA);
    }

    return pTree;
}