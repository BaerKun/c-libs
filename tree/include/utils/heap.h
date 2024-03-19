#ifndef TREE_HEAP_H
#define TREE_HEAP_H

typedef int HeapValue;
typedef int HeapKey;

typedef struct {
    HeapKey key;
    HeapValue value;
} HeapElement;

typedef struct {
    int size;
    HeapElement *elements;
} Heap, *HeapPtr;

HeapPtr CreateHeap(int capacity);

void PercolateDown(HeapPtr pHeap, int index);

void HeapInsert(HeapPtr pHeap, HeapValue element, HeapKey key);

HeapElement DeleteMin(HeapPtr pHeap);

void BuildHeap(HeapPtr pHeap, int size);

#endif //TREE_HEAP_H
