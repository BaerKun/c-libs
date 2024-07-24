#include "super_tree/binary_search_tree.h"
#include <stdlib.h>
#include <stdio.h>

BinaryTreeNodePtr bstFindMax(BSTPtr tree) {
    BinaryTreeNodePtr node;

    for(node = tree->root; node->right != NULL; node = node->right);

    return node;
}

BinaryTreeNodePtr bstDeleteMax(BSTPtr tree) {
    BinaryTreeNodePtr parent, child;

    parent = NULL;
    for(child = tree->root; child->right != NULL; child = child->right)
        parent = child;

    return binaryTreeDelete(tree, parent, 1, NULL);
}

DataType bstDeleteAndFreeMax(BSTPtr tree) {
    BinaryTreeNodePtr node = bstDeleteMax(tree);
    freeTreeNode(tree, node);
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

void bstInsert_node(BSTPtr tree, BinaryTreeNodePtr node) {
    BinaryTreeNodePtr parent;
    int isRight;

    parent = bstFindParent(tree, node->data, &isRight);

    binaryTreeInsert_node(tree, parent, node, isRight);
}

void bstInsert_data(BSTPtr tree, const DataType data){
    BinaryTreeNodePtr parent;
    int isRight;

    parent = bstFindParent(tree, data, &isRight);

    binaryTreeInsert_data(tree, parent, data, isRight);
}

static BinaryTreeNodePtr bstDeleteRightMin(BSTPtr tree, BinaryTreeNodePtr node) {
    BinaryTreeNodePtr parent, child;

    parent = node;
    child = node->right;
    while (child->left != NULL) {
        parent = child;
        child = child->left;
    }

    return  binaryTreeDelete(tree, parent, parent == node, NULL);
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

BinaryTreeNodePtr bstDelete_node(BSTPtr tree, BinaryTreeNodePtr parent, int isRight) {
    return binaryTreeDelete(tree, parent, isRight, if2childrenCallback);
}

BinaryTreeNodePtr bstDelete_data(BSTPtr tree, DataType data) {
    int isRight;
    BinaryTreeNodePtr parent = bstFindParent(tree, data, &isRight);
    return bstDelete_node(tree, parent, isRight);
}

BinaryTreeNodePtr bstFind(BSTPtr tree, DataType data) {
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
    BSTPtr tree = isFixed ? createBinaryTree_fixedCapacity(len) : createBinaryTree();

    for(int i = 0; i < len; i++){
        bstInsert_data(tree, data[i]);
    }

    return tree;
}