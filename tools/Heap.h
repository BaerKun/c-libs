#ifndef HEAP_H
#define HEAP_H
#define MINUS_INFINITY 0x80000000

typedef int HeapElement;
typedef struct Heap *Heap;

struct Heap{
    int capacity;
    int size;
    HeapElement  *elements;
};

Heap CreateHeap(int capacity);
void HeapInsert(Heap heap, HeapElement element);
HeapElement DeleteMin(Heap);
HeapElement FindMin(Heap);
int IsEmpty(Heap);
int IsFull(Heap);
void MakeEmpty(Heap);
void DeleteHeap(Heap);
void BuildHeap(HeapElement *array, int len);

#endif //HEAP_H
