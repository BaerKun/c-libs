#ifndef QUEUEFORGRAPH_H
#define QUEUEFORGRAPH_H

#define InitQueue(queue, capacity, Type) \
    Type elements[capacity];    \
    queue.front = queue.rear = 0;   \
    queue.elements = elements

#define Enqueue(queue, element) \
    queue.elements[queue.rear++] = element

#define Dequeue(queue) queue.elements[queue.front++]

#endif //QUEUEFORGRAPH_H
