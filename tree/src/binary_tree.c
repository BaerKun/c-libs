#include "tree/binary_tree.h"
#include <stdlib.h>
#include <stdio.h>


BinaryTreePtr newBinaryTree() {
    const BinaryTreePtr tree = malloc(sizeof(BinaryTree));
    tree->nodeNum = 0;
    return tree;
}

static void deleteTreeNodeRecursively(const BinaryTreeNodePtr root) {
    if (root->left != NULL)
        deleteTreeNodeRecursively(root->left);
    if (root->right != NULL)
        deleteTreeNodeRecursively(root->right);
    free(root);
}

void btDestroy(const BinaryTreePtr tree) {
    if (tree->root != NULL)
        deleteTreeNodeRecursively(tree->root);
    free(tree);
}

BinaryTreeNodePtr btNewNode(const DataType data) {
    const BinaryTreeNodePtr node = malloc(sizeof(BinaryTreeNode));
    node->data = data;
    node->left = node->right = node->next = NULL;

    return node;
}

BinaryTreeNodePtr *btGetChild(const BinaryTreePtr tree, const BinaryTreeNodePtr parent, const int isRight) {
    if (parent == NULL)
        return &tree->root;
    if (isRight)
        return &parent->right;
    return &parent->left;
}

BinaryTreeNodePtr btUnlink(const BinaryTreePtr tree, const BinaryTreeNodePtr parent, const int isRight,
                            BinaryTreeNodePtr (*if2children)(BinaryTreePtr, BinaryTreeNodePtr, int)) {
    BinaryTreeNodePtr *ptr = btGetChild(tree, parent, isRight);
    const BinaryTreeNodePtr node = *ptr;

    if (node == NULL)
        return NULL;

    if (node->next != NULL) {
        node->next->left = node->left;
        node->next->right = node->right;
        *ptr = node->next;
    } else if (node->left == NULL)
        *ptr = node->right;
    else if (node->right == NULL)
        *ptr = node->left;
    else if (if2children == NULL)
        return NULL;
    else
        return if2children(tree, parent, isRight);

    tree->nodeNum--;
    return node;
}

void btDeleteNode(const BinaryTreePtr tree, const BinaryTreeNodePtr parent, const int isRight,
               BinaryTreeNodePtr (*if2children)(BinaryTreePtr, BinaryTreeNodePtr, int)) {
    const BinaryTreeNodePtr node = btUnlink(tree, parent, isRight, if2children);
    if (node != NULL)
        free(node);
}

void btInsertNode(const BinaryTreePtr tree, const BinaryTreeNodePtr parent, const BinaryTreeNodePtr node,
                   const int isRight) {
    BinaryTreeNodePtr *ptr = btGetChild(tree, parent, isRight);
    const BinaryTreeNodePtr _node = *ptr;

    if (_node == NULL) {
        node->left = node->right = node->next = NULL;
        *ptr = node;
    } else {
        node->left = _node->left;
        node->right = _node->right;
        node->next = _node;
        *ptr = node;
    }
    tree->nodeNum++;
}

void btInsertData(const BinaryTreePtr tree, const BinaryTreeNodePtr parent, const DataType data, const int isRight) {
    const BinaryTreeNodePtr node = btNewNode(data);
    btInsertNode(tree, parent, node, isRight);
}
