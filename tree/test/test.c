#include <stdio.h>
#include <random.h>
#include "Huffman.h"

void print(const TreeNodePtr node) {
    if (node->left)
        print(node->left);
    printf("%d\n", node->data);
    if (node->right)
        print(node->right);
}

int main() {
    int data[26];
    int weight[26];
    pcg32_srandom(123211243, 124214222);
    for (int i = 0; i < 26; ++i) {
        weight[i] = randint(0, 100);
        data[i] = i;
    }
    // BSTPtr tree = optimalBST(data, weight, 26);
    const TreeNodePtr tree1 = HuffmanCode(data, weight, 26);
    print(tree1);
    // BinaryTreePtr tree = newBinaryTree_fixedCapacity(3);
    // BT_insertData(tree, NULL, 1, 0);
    // BT_insertData(tree, tree->root, 2, 0);
    // BT_insertData(tree, tree->root, 3, 1);
    return 0;
}