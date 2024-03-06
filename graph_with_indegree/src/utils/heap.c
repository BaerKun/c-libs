#include "utils/heap.h"

static void PercolateDown(WeightType **heap, int father, int len){
    int child;
    WeightType *theTop;
    theTop = heap[father];

    for( ; (child = father << 1) < len; father = child){
        if(child + 1 != len && *heap[child + 1] < *heap[child])
            child++;
        if(*heap[child] < *theTop)
            heap[father] = heap[child];
        else
            break;
    }
    heap[father] = theTop;
}

void G_HeapInsert(HeapPtr pHeap, WeightType *pWeight) {
    WeightType **elements = pHeap->elements;
    int i, father;

    for (i = ++pHeap->size; (father = i >> 1) && *elements[father] > *pWeight; i = father)
        pHeap->elements[i] = elements[father];

    pHeap->elements[i] = pWeight;
}

WeightType *G_DeleteMin(HeapPtr pHeap) {
    WeightType **elements = pHeap->elements - 1;
    WeightType *output = elements[1];
    elements[1] = elements[pHeap->size];

    PercolateDown(elements, 1, 1 + pHeap->size--);

    return output;
}

void G_BuildHeap(HeapPtr pHeap, int size) {
    WeightType **heap = pHeap->elements - 1;
    pHeap->size = size;
    size++;

    for (int i = size >> 1; i; i--)
        PercolateDown(heap, i, size);
}