#include "cursor/trie_tree.h"
#include "utils/heap.h"

void HuffmanCode(TrieTreePtr pTree, DataType *data, const int *cost, int number) {
    HeapPtr pHeap = CreateHeap(number);
    HeapElement minCost;
    TreeNodeId root, left, right;

    for (int i = 0; i < number; i++) {
        pHeap->elements[i + 1] = (HeapElement) {i, cost[i]};
    }

    BuildHeap(pHeap, number);

    while (pHeap->size > 1) {
        minCost = DeleteMin(pHeap);
        root = pTree->nodeNum++;
        if(minCost.key >= number)
            left = minCost.key - number;
        else
            left = CreateTreeNode(pTree, data[minCost.key]);
        if(pHeap->elements[1].key >= number)
            right = pHeap->elements[1].key - number;
        else
            right = CreateTreeNode(pTree, data[pHeap->elements[1].key]);
        pTree->nodes[root] = (BinaryTreeNode){left, right, NO_DATA};
        pHeap->elements[1].value += minCost.value;
        pHeap->elements[1].key = number + root;
        PercolateDown(pHeap, 1);
    }
    pTree->root = root;
}