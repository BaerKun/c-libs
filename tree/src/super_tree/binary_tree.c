#include "super_tree/binary_tree.h"
#include <stdlib.h>
#include <stdio.h>

BinaryTreePtr createBinaryTree_fixedCapacity(int capacity) {
    BinaryTreePtr tree = malloc(sizeof(BinaryTree));
    tree->capacity = capacity;
    tree->nodeNum = 0;
    tree->memoryPool = malloc(capacity * sizeof(BinaryTreeNode));
    tree->root = NULL;

    BinaryTreeNodePtr end = tree->memoryPool + capacity - 1;
    for (BinaryTreeNodePtr node = tree->memoryPool; node < end; ++node) {
        node->isEmpty = 1;
        node->nextEmpty = node + 1;
    }
    end->isEmpty = 1;
    end->nextEmpty = tree->memoryPool;

    return tree;
}

BinaryTreePtr createBinaryTree() {
    BinaryTreePtr tree = malloc(sizeof(BinaryTree));
    tree->capacity = MOST_TREE_NODES;
    tree->nodeNum = 0;
    tree->memoryPool = tree->root = NULL;

    return tree;
}

static void deleteTreeNodeRecursively(BinaryTreeNodePtr root) {
    if(root->left != NULL)
        deleteTreeNodeRecursively(root->left);
    if(root->right != NULL)
        deleteTreeNodeRecursively(root->right);
    free(root);
}

void deleteBinaryTree(BinaryTreePtr tree) {
    if(tree->memoryPool != NULL)
        free(tree->memoryPool);
    else if(tree->root != NULL)
        deleteTreeNodeRecursively(tree->root);
    free(tree);
}

BinaryTreeNodePtr createTreeNode(DataType data) {
    BinaryTreeNodePtr node = malloc(sizeof(BinaryTreeNode));
    node->data = data;
    node->left = node->right = node->next = node->nextEmpty = NULL;
    node->isEmpty = 0;

    return node;
}

static BinaryTreeNodePtr _createTreeNode_fc(BinaryTreePtr tree, DataType data) {
    if(tree->memoryPool->nextEmpty == NULL && !tree->memoryPool->isEmpty) {
        fputs("fixed tree is full!\n", stderr);
        for(int i=0;i<tree->capacity;++i)
            printf("%d ", tree->memoryPool[i].data);
        exit(1);
        return NULL;
    }
    BinaryTreeNodePtr node;
    do {
        node = tree->memoryPool->nextEmpty;
        tree->memoryPool->nextEmpty = node->nextEmpty;
    } while (!node->isEmpty);

    *node = (BinaryTreeNode){NULL, NULL, NULL, NULL, 0, data};

    return node;
}

BinaryTreeNodePtr createTreeNode_fc(BinaryTreePtr tree, DataType data) {
    tree->nodeNum++;
    return _createTreeNode_fc(tree, data);
}

void freeTreeNode(BinaryTreePtr tree, BinaryTreeNodePtr node) {
    if(tree->memoryPool == NULL) {
        free(node);
        return;
    }
    node->isEmpty = 1;
    node->nextEmpty = tree->memoryPool->nextEmpty;
    tree->memoryPool->nextEmpty = node;
}

BinaryTreeNodePtr binaryTreeDelete(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight,
    BinaryTreeNodePtr if2childrenCallback(BinaryTreePtr, BinaryTreeNodePtr, int)) {
    BinaryTreeNodePtr node, *ptr;
    if(parent == NULL) {
        node = tree->root;
        ptr = &tree->root;
    }
    else if(isRight) {
        node = parent->right;
        ptr = &parent->right;
    }
    else {
        node = parent->left;
        ptr = &parent->left;
    }

    if(node == NULL)
        return  NULL;

    if (node->next != NULL) {
        node->next->left = node->left;
        node->next->right = node->right;
        *ptr = node->next;
    }
    else if(node->left == NULL)
        *ptr = node->right;
    else if(node->right == NULL)
        *ptr = node->left;
    else if(if2childrenCallback == NULL)
        return  NULL;
    else
        return  if2childrenCallback(tree, parent, isRight);

    tree->nodeNum--;
    return node;
}

void binaryTreeDeleteAndFree(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight,
    BinaryTreeNodePtr if2childrenCallback(BinaryTreePtr, BinaryTreeNodePtr, int)) {
    BinaryTreeNodePtr node = binaryTreeDelete(tree, parent, isRight, if2childrenCallback);
    if(node != NULL)
        freeTreeNode(tree, node);
}

void binaryTreeInsert_node(BinaryTreePtr tree, BinaryTreeNodePtr parent, BinaryTreeNodePtr node, int isRight) {
    BinaryTreeNodePtr _node, *ptr;
    if(parent == NULL) {
        _node = tree->root;
        ptr = &tree->root;
    }else if(isRight) {
        _node = parent->right;
        ptr = &parent->right;
    } else {
        _node = parent->left;
        ptr = &parent->left;
    }

    if(_node == NULL) {
        *ptr = node;
    } else {
        node->left = _node->left;
        node->right = _node->right;
        node->next = _node;
        *ptr = node;
    }
    tree->nodeNum++;
}

void binaryTreeInsert_data(BinaryTreePtr tree, BinaryTreeNodePtr parent, DataType data, int isRight) {
    BinaryTreeNodePtr node = tree->memoryPool == NULL ?
        createTreeNode(data) : _createTreeNode_fc(tree, data);
    if(node != NULL)
        binaryTreeInsert_node(tree, parent, node, isRight);
}