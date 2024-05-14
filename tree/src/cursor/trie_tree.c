#include "cursor/trie_tree.h"
#include "utils/heap.h"
#include "cursor/macro.h"

void HuffmanCode(TrieTreePtr pTree, DataType *data, const int *cost, int number) {
    HeapPtr pHeap = CreateHeap(number);
    HeapElement minCost;
    TreeNodePtr root, left, right;

    for (int i = 0; i < number; i++) {
        pHeap->elements[i + 1] = (HeapElement) {i, cost[i]};
    }

    BuildHeap(pHeap, number);

    while (pHeap->size > 1) {
        minCost = DeleteMin(pHeap);
        root = createTreeNode(pTree);

        if(minCost.key >= number) {
            left = minCost.key - number;
            LEFT(root) = left;
        }
        else
            binaryTreeInsert_val(pTree, root, 0, data[minCost.key]);
        if(pHeap->elements[1].key >= number) {
            right = pHeap->elements[1].key - number;
            RIGHT(root) = right;
        }
        else
            binaryTreeInsert_val(pTree, root, 1, data[pHeap->elements[1].key]);

        pHeap->elements[1].value += minCost.value;
        pHeap->elements[1].key = number + root;
        PercolateDown(pHeap, 1);
    }
    pTree->root = root;
}