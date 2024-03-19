#ifndef GRAPH_QUEUE_H
#define GRAPH_QUEUE_H

typedef int QueueElementType;

typedef struct {
    int front;
    int rear;
    QueueElementType *elements;
}Queue, *QueuePtr;

QueuePtr CreateQueue(int capacity);

void Enqueue(QueuePtr pQueue, QueueElementType element);

QueueElementType Dequeue(QueuePtr pQueue);

void MakeQueueEmpty(QueuePtr pQueue);

void DeleteQueue(QueuePtr pQueue);

#endif //GRAPH_QUEUE_H
