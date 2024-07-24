#include "super_tree/trie_tree.h"
#include "utils/heap.h"
#include <stdio.h>

TrieTreePtr HuffmanCode(DataType *data, const int *cost, int number) {
    HeapPtr pHeap = CreateHeap(number);
    HeapElement minCost;
    int left, right;
    TrieTreePtr tree = createBinaryTree_fixedCapacity(number * 2 - 1);
    TrieTreeNodePtr rootNode = NULL;

    for (int i = 0; i < number; i++) {
        pHeap->elements[i + 1] = (HeapElement) {i, cost[i]};
    }

    BuildHeap(pHeap, number);

    while (pHeap->size > 1) {
        minCost = DeleteMin(pHeap);
        rootNode = createTreeNode_fc(tree, NO_DATA);

        left = minCost.key;
        if(left < 0) {
            rootNode->left = tree->memoryPool - left;
        } else
            binaryTreeInsert_data(tree, rootNode, data[left], 0);

        right = pHeap->elements[1].key;
        if(right < 0) {
            rootNode->right = tree->memoryPool - right;
        } else
            binaryTreeInsert_data(tree, rootNode, data[right], 1);

        pHeap->elements[1].value += minCost.value;
        pHeap->elements[1].key = tree->memoryPool - rootNode;
        PercolateDown(pHeap, 1);
    }
    tree->root = rootNode;

    return tree;
}