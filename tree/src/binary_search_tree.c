#include "tree/binary_search_tree.h"
#include <stdio.h>
#include <stdlib.h>

BinaryTreeNodePtr bstFindMax(const BSTPtr tree) {
    BinaryTreeNodePtr node;

    for(node = tree->root; node->right != NULL; node = node->right);

    return node;
}

BinaryTreeNodePtr bstUnlinkMax(const BSTPtr tree) {
    BinaryTreeNodePtr parent = NULL;
    for(BinaryTreeNodePtr child = tree->root; child->right != NULL; child = child->right)
        parent = child;

    return btUnlink(tree, parent, 1, NULL);
}

static BinaryTreeNodePtr bstFindParent(const BSTPtr tree, const DataType data, int *isRight) {
    BinaryTreeNodePtr parent = NULL,
        child = tree->root;

    while(child != NULL) {
        if (LESS(data, child->data)) {
            parent = child;
            child = child->left;
            *isRight = 0;
        } else if (GREATER(data, child->data)) {
            parent = child;
            child = child->right;
            *isRight = 1;
        } else {
            break;
        }
    }

    return parent;
}

void bstInsertNode(const BSTPtr tree, const BinaryTreeNodePtr node) {
    int isRight;

    const BinaryTreeNodePtr parent = bstFindParent(tree, node->data, &isRight);

    btInsertNode(tree, parent, node, isRight);
}

void bstInsertData(const BSTPtr tree, const DataType data){
    int isRight;

    const BinaryTreeNodePtr parent = bstFindParent(tree, data, &isRight);

    btInsertData(tree, parent, data, isRight);
}

static BinaryTreeNodePtr bstUnlinkRightMin(const BSTPtr tree, const BinaryTreeNodePtr node) {
    BinaryTreeNodePtr parent = node;
    BinaryTreeNodePtr child = node->right;
    while (child->left != NULL) {
        parent = child;
        child = child->left;
    }

    return btUnlink(tree, parent, parent == node, NULL);
}

static BinaryTreeNodePtr if2childrenCallback(const BinaryTreePtr tree, const BinaryTreeNodePtr parent, const int isRight) {
    BinaryTreeNodePtr *ptr = btGetChild(tree, parent, isRight);
    const BinaryTreeNodePtr node = *ptr;

    const BinaryTreeNodePtr replace = bstUnlinkRightMin(tree, node);
    replace->left = node->left;
    replace->right = node->right;
    replace->next = NULL;
    *ptr = replace;

    tree->nodeNum--;
    return node;
}

BinaryTreeNodePtr bstUnlink(const BSTPtr tree, const BinaryTreeNodePtr node, const int isRight) {
    return btUnlink(tree, node, isRight, if2childrenCallback);
}

BinaryTreeNodePtr bstUnlinkWithData(const BSTPtr tree, const DataType data) {
    int isRight;
    const BinaryTreeNodePtr parent = bstFindParent(tree, data, &isRight);
    return bstUnlink(tree, parent, isRight);
}

BinaryTreeNodePtr bstFind(const BSTPtr tree, const DataType data) {
    BinaryTreeNodePtr node;

    for(node = tree->root; node != NULL;){
        if(LESS(data, node->data))
            node = node->left;
        else if(GREATER(data, node->data))
            node = node->right;
        else
            break;
    }

    return node;
}

BSTPtr buildBST(const DataType data[], const int len, const BinaryTreeNodePtr buffer) {
    const BSTPtr tree = newBinaryTree();

    if (buffer == NULL) {
        for(int i = 0; i < len; i++)
            bstInsertData(tree, data[i]);
    } else {
        for (int i = 0; i < len; i++) {
            buffer[i].data = data[i];
            bstInsertNode(tree, buffer + i);
        }
    }

    return tree;
}