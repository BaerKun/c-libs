#include "tree/trie_tree.h"
#include <stdio.h>

typedef struct HeapElement {
    TrieTreeNodePtr node;
    int value;
} HeapElement;

#define HEAP_ELEMENT_TYPE HeapElement
#define HEAP_LESS_THAN(a, b) (a.value < b.value)
#include "heap.h"

TrieTreePtr HuffmanCode(const DataType *data, const int *cost, const int number) {
    const TrieTreePtr tree = newBinaryTree();
    const HeapPtr heap = newHeap(number);
    TrieTreeNodePtr root = NULL;

    for (int i = 0, j = 1; i < number; i = j++) {
        const TrieTreeNodePtr node = btNewNode(data[i]);
        heap->prev[j] = (HeapElement) {node, cost[i]};
    }

    buildHeap(heap, number);

    while (heap->size > 1) {
        const HeapElement minCost = heapDeleteMin(heap);
        root = btNewNode(NO_DATA);

        root->left = minCost.node;
        root->right = heapDeleteMin(heap).node;

        heap->prev[1].value += minCost.value;
        heap->prev[1].node = root;
        heapPercolateDown(heap->prev, 1, heap->size);
    }

    tree->root = root;
    tree->nodeNum = number;

    heapDestroy(heap);
    return tree;
}