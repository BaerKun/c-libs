#include "tree/binary_tree.h"
#include <stdlib.h>
#include <stdio.h>

BinaryTreePtr newBinaryTree_fixedCapacity(int capacity) {
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

BinaryTreePtr newBinaryTree() {
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

void BT_destroy(BinaryTreePtr tree) {
    if(tree->memoryPool != NULL)
        free(tree->memoryPool);
    else if(tree->root != NULL)
        deleteTreeNodeRecursively(tree->root);
    free(tree);
}

BinaryTreeNodePtr BT_newNode(DataType data) {
    BinaryTreeNodePtr node = malloc(sizeof(BinaryTreeNode));
    node->data = data;
    node->left = node->right = node->next = node->nextEmpty = NULL;
    node->isEmpty = 0;

    return node;
}

static BinaryTreeNodePtr newNodeHelper_fc(BinaryTreePtr tree, DataType data) {
    if(tree->memoryPool->nextEmpty == NULL && !tree->memoryPool->isEmpty) {
        fputs("fixed tree is full!\n", stderr);
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

BinaryTreeNodePtr BT_newNode_fc(BinaryTreePtr tree, DataType data) {
    tree->nodeNum++;
    return newNodeHelper_fc(tree, data);
}

void BT_freeNode(BinaryTreePtr tree, BinaryTreeNodePtr node) {
    if(tree->memoryPool == NULL) {
        free(node);
        return;
    }
    node->isEmpty = 1;
    node->nextEmpty = tree->memoryPool->nextEmpty;
    tree->memoryPool->nextEmpty = node;
}

BinaryTreeNodePtr BT_deleteNode(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight,
                                BinaryTreeNodePtr if2children(BinaryTreePtr, BinaryTreeNodePtr, int)) {
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
    else if(if2children == NULL)
        return  NULL;
    else
        return  if2children(tree, parent, isRight);

    tree->nodeNum--;
    return node;
}

void BT_deleteAndFree(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight,
                      BinaryTreeNodePtr if2children(BinaryTreePtr, BinaryTreeNodePtr, int)) {
    BinaryTreeNodePtr node = BT_deleteNode(tree, parent, isRight, if2children);
    if(node != NULL)
        BT_freeNode(tree, node);
}

void BT_insertNode(BinaryTreePtr tree, BinaryTreeNodePtr parent, BinaryTreeNodePtr node, int isRight) {
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

void BT_insertData(BinaryTreePtr tree, BinaryTreeNodePtr parent, DataType data, int isRight) {
    BinaryTreeNodePtr node = tree->memoryPool == NULL ?
                             BT_newNode(data) : newNodeHelper_fc(tree, data);
    if(node != NULL)
        BT_insertNode(tree, parent, node, isRight);
}