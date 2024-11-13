#include "tree/binary_search_tree.h"
#include <stdio.h>

BinaryTreeNodePtr BST_findMax(const BSTPtr tree) {
    BinaryTreeNodePtr node;

    for(node = tree->root; node->right != NULL; node = node->right);

    return node;
}

BinaryTreeNodePtr BST_deleteMax(const BSTPtr tree) {
    BinaryTreeNodePtr parent = NULL;
    for(BinaryTreeNodePtr child = tree->root; child->right != NULL; child = child->right)
        parent = child;

    return BT_deleteNode(tree, parent, 1, NULL);
}

DataType bstDeleteAndFreeMax(const BSTPtr tree) {
    const BinaryTreeNodePtr node = BST_deleteMax(tree);
    BT_freeNode(tree, node);
    return node->data;
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

void BST_insertNode(const BSTPtr tree, const BinaryTreeNodePtr node) {
    int isRight;

    const BinaryTreeNodePtr parent = bstFindParent(tree, node->data, &isRight);

    BT_insertNode(tree, parent, node, isRight);
}

void BST_insertData(const BSTPtr tree, const DataType data){
    int isRight;

    const BinaryTreeNodePtr parent = bstFindParent(tree, data, &isRight);

    BT_insertData(tree, parent, data, isRight);
}

static BinaryTreeNodePtr bstDeleteRightMin(const BSTPtr tree, const BinaryTreeNodePtr node) {
    BinaryTreeNodePtr parent = node;
    BinaryTreeNodePtr child = node->right;
    while (child->left != NULL) {
        parent = child;
        child = child->left;
    }

    return BT_deleteNode(tree, parent, parent == node, NULL);
}

static BinaryTreeNodePtr if2childrenCallback(const BinaryTreePtr tree, const BinaryTreeNodePtr parent, const int isRight) {
    BinaryTreeNodePtr node, *ptr;
    if(parent == NULL) {
        node = tree->root;
        ptr = &tree->root;
    }else if(isRight) {
        node = parent->right;
        ptr = &parent->right;
    } else {
        node = parent->left;
        ptr = &parent->left;
    }
    const BinaryTreeNodePtr replace = bstDeleteRightMin(tree, node);
    replace->left = node->left;
    replace->right = node->right;
    replace->next = NULL;
    *ptr = replace;

    tree->nodeNum--;

    return node;
}

BinaryTreeNodePtr BST_deleteNode(const BSTPtr tree, const BinaryTreeNodePtr node, const int isRight) {
    return BT_deleteNode(tree, node, isRight, if2childrenCallback);
}

BinaryTreeNodePtr BST_deleteData(const BSTPtr tree, const DataType data) {
    int isRight;
    const BinaryTreeNodePtr parent = bstFindParent(tree, data, &isRight);
    return BST_deleteNode(tree, parent, isRight);
}

BinaryTreeNodePtr BST_find(const BSTPtr tree, const DataType data) {
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

BSTPtr buildBST(const DataType data[], const int len, const int isFixed) {
    const BSTPtr tree = isFixed ? newBinaryTree_fixedCapacity(len) : newBinaryTree();

    for(int i = 0; i < len; i++){
        BST_insertData(tree, data[i]);
    }

    return tree;
}