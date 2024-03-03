#include "QueueForGraph.h"

#define InitQueue(queue, capacity) \
    int elements[capacity];    \
    queue.front = queue.rear = 0;   \
    queue.elements = elements

#define Enqueue(queue, element) \
    queue.elements[queue.rear++] = element

#define Dequeue(queue) queue.elements[queue.front++]