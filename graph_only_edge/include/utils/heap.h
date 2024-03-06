#ifndef GRAPH_HEAP_H
#define GRAPH_HEAP_H

#define InitHeap(heap, capacity) \
    WeightType *elements[capacity];  \
    heap.size = 0;                  \
    heap.elements = elements

typedef int WeightType;

typedef struct {
    int size;
    WeightType **elements;
} Heap, *HeapPtr;

void G_HeapInsert(HeapPtr pHeap, WeightType *pWeight);

WeightType *G_DeleteMin(HeapPtr pHeap);

void G_BuildHeap(HeapPtr pHeap, int size);

#endif //GRAPH_HEAP_H
