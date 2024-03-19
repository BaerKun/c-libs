#include "utils/heap.h"
#include <stdlib.h>

HeapPtr CreateHeap(int capacity) {
    HeapPtr pHeap = malloc(sizeof(Heap));
    pHeap->size = 0;
    pHeap->elements = malloc((1 + capacity) * sizeof(HeapElement));
    return pHeap;
}

void PercolateDown(HeapPtr pHeap, int index) {
    int child;
    HeapElement topVar = pHeap->elements[index];

    for (; (child = index << 1) <= pHeap->size; index = child) {
        if (child != pHeap->size && pHeap->elements[child + 1].value < pHeap->elements[child].value)
            child++;
        if (pHeap->elements[child].value < topVar.value)
            pHeap->elements[index] = pHeap->elements[child];
        else
            break;
    }
    pHeap->elements[index] = topVar;
}

void HeapInsert(HeapPtr pHeap, HeapValue value, HeapKey key) {
    int i, father;

    for (i = ++pHeap->size; (father = i >> 1) && pHeap->elements[father].value > value; i = father)
        pHeap->elements[i] = pHeap->elements[father];

    pHeap->elements[i] = (HeapElement){key, value};
}

HeapElement DeleteMin(HeapPtr pHeap) {
    HeapElement element = pHeap->elements[1];
    pHeap->elements[1] = pHeap->elements[pHeap->size--];

    PercolateDown(pHeap, 1);

    return element;
}

void BuildHeap(HeapPtr pHeap, int size) {
    pHeap->size = size;

    for (int i = size >> 1; i; i--) {
        PercolateDown(pHeap, i);
    }
}