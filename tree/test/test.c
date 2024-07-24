#include <stdio.h>
#include "super_tree/OBST.h"
#include "super_tree/trie_tree.h"

void print(BinaryTreeNodePtr node) {
    if(node->left)
        print(node->left);
    printf("%d\n", node->data);
    if(node->right)
        print(node->right);
}

int main() {
    int data[26];
    float weight[26];
    for(int i=0; i < 26;++i) {
        weight[i] = (i*i%100);
        data[i] = i;
    }
    // BSTPtr tree = optimalBST(data, weight, 26);
    TrieTreePtr tree1 = HuffmanCode(data, weight, 26);
    print(tree1->root);
    // BinaryTreePtr tree = createBinaryTree_fixedCapacity(3);
    // binaryTreeInsert_data(tree, NULL, 1, 0);
    // binaryTreeInsert_data(tree, tree->root, 2, 0);
    // binaryTreeInsert_data(tree, tree->root, 3, 1);
    return 0;
}