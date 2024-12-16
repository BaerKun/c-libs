#include "tree/binary_search_tree.h"
#include <stdio.h>
#include <stdlib.h>

BinaryTreeNodePtr *bstFindMax(const BSTPtr tree) {
    BinaryTreeNodePtr *parent2child = &tree->root;
    BinaryTreeNodePtr child = *parent2child;
    if (child != NULL) {
        while (child->right != NULL) {
            parent2child = &child->right;
            child = *parent2child;
        }
    }

    return parent2child;
}

BinaryTreeNodePtr bstUnlinkMax(const BSTPtr tree) {
    return btUnlink(tree, bstFindMax(tree), NULL);
}

void bstInsertNode(const BSTPtr tree, const BinaryTreeNodePtr node) {
    BinaryTreeNodePtr *parent2child = bstFind(tree, node->data);

    btInsertNode(tree, parent2child, node);
}

void bstInsertData(const BSTPtr tree, const DataType data) {
    BinaryTreeNodePtr *parent2child = bstFind(tree, data);

    btInsertData(tree, parent2child, data);
}

static BinaryTreeNodePtr bstUnlinkRightMin(const BSTPtr tree, const BinaryTreeNodePtr node) {
    BinaryTreeNodePtr *parent2child = &node->right;
    BinaryTreeNodePtr child = *parent2child;
    while (child->left != NULL) {
        parent2child = &child->left;
        child = *parent2child;
    }

    return btUnlink(tree, parent2child, NULL);
}

static BinaryTreeNodePtr if2childrenCallback(const BinaryTreePtr tree, BinaryTreeNodePtr *const parent2child) {
    const BinaryTreeNodePtr node = *parent2child;

    const BinaryTreeNodePtr replace = bstUnlinkRightMin(tree, node);
    replace->left = node->left;
    replace->right = node->right;
    replace->next = NULL;
    *parent2child = replace;

    tree->nodeNum--;
    return node;
}

BinaryTreeNodePtr bstUnlink(const BSTPtr tree, BinaryTreeNodePtr *const parent2child) {
    return btUnlink(tree, parent2child, if2childrenCallback);
}

BinaryTreeNodePtr bstUnlinkWithData(const BSTPtr tree, const DataType data) {
    BinaryTreeNodePtr *const parent2child = bstFind(tree, data);
    return bstUnlink(tree, parent2child);
}

BinaryTreeNodePtr *bstFind(const BSTPtr tree, const DataType data) {
    BinaryTreeNodePtr *parent2child = &tree->root;

    for (BinaryTreeNodePtr node = *parent2child; node != NULL; node = *parent2child) {
        if (LESS(data, node->data))
            parent2child = &node->left;
        else if (GREATER(data, node->data))
            parent2child = &node->right;
        else
            break;
    }

    return parent2child;
}

BSTPtr buildBST(const DataType data[], const int len, const BinaryTreeNodePtr buffer) {
    const BSTPtr tree = newBinaryTree();

    if (buffer == NULL) {
        for (int i = 0; i < len; i++)
            bstInsertData(tree, data[i]);
    } else {
        for (int i = 0; i < len; i++) {
            buffer[i].data = data[i];
            bstInsertNode(tree, buffer + i);
        }
    }

    return tree;
}
