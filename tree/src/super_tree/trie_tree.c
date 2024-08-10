#include "super_tree/trie_tree.h"
#include <stdio.h>

typedef struct HeapElement {
    int key;
    int value;
} HeapElement;
#define HEAP_ELEMENT_TYPE HeapElement
#define HEAP_LESS_THAN(a, b) (a.key < b.key)
#include "heap.h"

TrieTreePtr HuffmanCode(DataType *data, const int *cost, int number) {
    HeapElement minCost, array[number];
    int left, right;
    TrieTreePtr tree = newBinaryTree_fixedCapacity(number * 2 - 1);
    TrieTreeNodePtr rootNode = NULL;

    for (int i = 0; i < number; i++)
        array[i] = (HeapElement) {i, cost[i]};

    HeapPtr heap = buildHeap(array - 1, number);

    while (heap->size > 1) {
        minCost = heap_deleteMin(heap);
        rootNode = BT_newNode_fc(tree, NO_DATA);

        left = minCost.key;
        if (left < 0) {
            rootNode->left = tree->memoryPool - left;
        } else
            BT_insertData(tree, rootNode, data[left], 0);

        right = heap->prev[1].key;
        if (right < 0) {
            rootNode->right = tree->memoryPool - right;
        } else
            BT_insertData(tree, rootNode, data[right], 1);

        heap->prev[1].value += minCost.value;
        heap->prev[1].key = (int) (tree->memoryPool - rootNode);
        heap_percolateDown(heap->prev, 1, heap->size);
    }
    tree->root = rootNode;

    return tree;
}