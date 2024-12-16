#include "tree/binary_tree.h"

#include <node.h>
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

BinaryTreeNodePtr btUnlink(const BinaryTreePtr tree, BinaryTreeNodePtr *const parent2child,
                            BinaryTreeNodePtr (*if2children)(BinaryTreePtr, BinaryTreeNodePtr *)) {
    const BinaryTreeNodePtr node = *parent2child;

    if (node == NULL)
        return NULL;

    if (node->next != NULL) {
        node->next->left = node->left;
        node->next->right = node->right;
        *parent2child = node->next;
    } else if (node->left == NULL)
        *parent2child = node->right;
    else if (node->right == NULL)
        *parent2child = node->left;
    else if (if2children == NULL)
        return NULL;
    else
        return if2children(tree, parent2child);

    tree->nodeNum--;
    return node;
}

void btDeleteNode(const BinaryTreePtr tree, BinaryTreeNodePtr *const parent2child,
               BinaryTreeNodePtr (*if2children)(BinaryTreePtr, BinaryTreeNodePtr *)) {
    const BinaryTreeNodePtr node = btUnlink(tree, parent2child, if2children);
    if (node != NULL)
        free(node);
}

void btInsertNode(const BinaryTreePtr tree, BinaryTreeNodePtr *const parent2child, const BinaryTreeNodePtr node) {
    const BinaryTreeNodePtr _node = *parent2child;

    if (_node == NULL) {
        node->left = node->right = node->next = NULL;
        *parent2child = node;
    } else {
        node->left = _node->left;
        node->right = _node->right;
        node->next = _node;
        *parent2child = node;
    }
    tree->nodeNum++;
}

void btInsertData(const BinaryTreePtr tree, BinaryTreeNodePtr *const parent2child, const DataType data) {
    const BinaryTreeNodePtr node = btNewNode(data);
    btInsertNode(tree, parent2child, node);
}
