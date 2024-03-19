#include "cursor/binary_tree.h"
#include <stdlib.h>
#include <stdio.h>

BinaryTreePtr CreateBinaryTree(int capacity) {
    BinaryTreePtr pTree = (BinaryTreePtr) malloc(sizeof(BinaryTree));
    pTree->capacity = capacity;
    pTree->nodeNum = 0;
    pTree->root = NO_TREE_NODE;
    pTree->nodes = (BinaryTreeNodePtr) calloc(capacity, sizeof(BinaryTreeNode));
    for(int i = 0; i < capacity; i++)
        pTree->nodes[i] = (BinaryTreeNode){NO_TREE_NODE, NO_TREE_NODE, NO_DATA};
    return pTree;
}

void DeleteBinaryTree(BinaryTreePtr pTree) {
    free(pTree->nodes);
    free(pTree);
}

void InsertNode(BinaryTreePtr pTree, TreeNodeId fatherId, DataType data) {
    if (pTree->nodeNum == pTree->capacity) {
        puts("InserNode: Tree is full!");
        return;
    }

    if (fatherId < 0 || fatherId >= pTree->nodeNum) {
        puts("InsertNode: father is out of range!");
        return;
    }

    TreeNodeId thisNodeId = pTree->nodeNum++;

    if (pTree->nodes[fatherId].left != NO_TREE_NODE) {
        if (pTree->nodes[fatherId].right != NO_TREE_NODE) {
            puts("InsertNode: father has two children!");
            return;
        }
        pTree->nodes[fatherId].right = thisNodeId;
    } else {
        pTree->nodes[fatherId].left = thisNodeId;
    }

    pTree->nodes[thisNodeId].data = data;
}

TreeNodeId CreateTreeNode(BinaryTreePtr pTree, DataType data) {
    pTree->nodes[pTree->nodeNum].data = data;
    return pTree->nodeNum++;
}


