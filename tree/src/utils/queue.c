#include "utils/queue.h"
#include <stdlib.h>

QueuePtr CreateQueue(int capacity) {
    QueuePtr pQueue = malloc(sizeof(Queue));
    QueueElementType *elements = malloc(sizeof(QueueElementType) * capacity);
    pQueue->front = pQueue->rear = 0;
    pQueue->elements = elements;
    return pQueue;
}

void Enqueue(QueuePtr pQueue, QueueElementType element) {
    pQueue->elements[pQueue->rear++] = element;
}

QueueElementType Dequeue(QueuePtr pQueue) {
    return pQueue->elements[pQueue->front++];
}

void MakeQueueEmpty(QueuePtr pQueue) {
    pQueue->front = pQueue->rear = 0;
}

void DeleteQueue(QueuePtr pQueue) {
    free(pQueue->elements);
    free(pQueue);
}