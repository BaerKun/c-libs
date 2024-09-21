#include "../include/tree/binary_search_tree.h"
#include <stdio.h>

BinaryTreeNodePtr BST_findMax(BSTPtr tree) {
    BinaryTreeNodePtr node;

    for(node = tree->root; node->right != NULL; node = node->right);

    return node;
}

BinaryTreeNodePtr BST_deleteMax(BSTPtr tree) {
    BinaryTreeNodePtr parent, child;

    parent = NULL;
    for(child = tree->root; child->right != NULL; child = child->right)
        parent = child;

    return BT_deleteNode(tree, parent, 1, NULL);
}

DataType bstDeleteAndFreeMax(BSTPtr tree) {
    BinaryTreeNodePtr node = BST_deleteMax(tree);
    BT_freeNode(tree, node);
    return node->data;
}

static BinaryTreeNodePtr bstFindParent(BSTPtr tree, DataType data, int *isRight) {
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

void BST_insertNode(BSTPtr tree, BinaryTreeNodePtr node) {
    BinaryTreeNodePtr parent;
    int isRight;

    parent = bstFindParent(tree, node->data, &isRight);

    BT_insertNode(tree, parent, node, isRight);
}

void BST_insertData(BSTPtr tree, DataType data){
    BinaryTreeNodePtr parent;
    int isRight;

    parent = bstFindParent(tree, data, &isRight);

    BT_insertData(tree, parent, data, isRight);
}

static BinaryTreeNodePtr bstDeleteRightMin(BSTPtr tree, BinaryTreeNodePtr node) {
    BinaryTreeNodePtr parent, child;

    parent = node;
    child = node->right;
    while (child->left != NULL) {
        parent = child;
        child = child->left;
    }

    return BT_deleteNode(tree, parent, parent == node, NULL);
}

static BinaryTreeNodePtr if2childrenCallback(BinaryTreePtr tree, BinaryTreeNodePtr parent, int isRight) {
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
    BinaryTreeNodePtr replace = bstDeleteRightMin(tree, node);
    replace->left = node->left;
    replace->right = node->right;
    replace->next = NULL;
    *ptr = replace;

    tree->nodeNum--;

    return node;
}

BinaryTreeNodePtr BST_deleteNode(BSTPtr tree, BinaryTreeNodePtr node, int isRight) {
    return BT_deleteNode(tree, node, isRight, if2childrenCallback);
}

BinaryTreeNodePtr BST_deleteData(BSTPtr tree, DataType data) {
    int isRight;
    BinaryTreeNodePtr parent = bstFindParent(tree, data, &isRight);
    return BST_deleteNode(tree, parent, isRight);
}

BinaryTreeNodePtr BST_find(BSTPtr tree, DataType data) {
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

BSTPtr buildBST(DataType data[], int len, int isFixed) {
    BSTPtr tree = isFixed ? newBinaryTree_fixedCapacity(len) : newBinaryTree();

    for(int i = 0; i < len; i++){
        BST_insertData(tree, data[i]);
    }

    return tree;
}