#include "weight_path.h"
#include "utils/queue.h"
#include "utils/heap.h"

void DijkstraWeightedPath(GraphPtr pGraph, Vertex source, Vertex target, Vertex *parent) {
    Vertex vertex, adjacentVertex;
    EdgePtr pEdge;
    char hasKnown[pGraph->vertexNum];
    int distance[pGraph->vertexNum];
    Heap heap;

    InitHeap(heap, pGraph->vertexNum);

    for (vertex = 0; vertex < pGraph->vertexNum; vertex++) {
        hasKnown[vertex] = 0;
        distance[vertex] = INFINITY;
    }
    distance[source] = 0;
    G_HeapInsert(&heap, distance + source);

    while (heap.size) {
        vertex = (Vertex) (G_DeleteMin(&heap) - distance);
        if (vertex == target)
            return;
        hasKnown[vertex] = 1;
        for (pEdge = pGraph->edges[vertex]; pEdge; pEdge = pEdge->next) {
            adjacentVertex = pEdge->target;
            if (hasKnown[adjacentVertex] || distance[adjacentVertex] <= distance[vertex] + pEdge->data.weight)
                continue;
            distance[adjacentVertex] = distance[vertex] + pEdge->data.weight;
            parent[adjacentVertex] = vertex;
            G_HeapInsert(&heap, distance + adjacentVertex);
        }
    }
}

// 无负值圈
void WeightedPath(GraphPtr pGraph, Vertex source, Vertex *parent) {
    Vertex vertex, adjacentVertex;
    EdgePtr pEdge;
    char isInQueue[pGraph->vertexNum];
    int distance[pGraph->vertexNum];
    Queue queue;

    InitQueue(queue, pGraph->vertexNum);
    for (vertex = 0; vertex < pGraph->vertexNum; vertex++) {
        isInQueue[vertex] = 0;
        distance[vertex] = INFINITY;
    }
    Enqueue(queue, source);
    distance[source] = 0;
    isInQueue[source] = 1;

    while (queue.front != queue.rear) {
        vertex = Dequeue(queue);
        isInQueue[vertex] = 0;
        for (pEdge = pGraph->edges[vertex]; pEdge; pEdge = pEdge->next) {
            adjacentVertex = pEdge->target;
            if (distance[adjacentVertex] <= distance[vertex] + pEdge->data.weight)
                continue;
            distance[adjacentVertex] = distance[vertex] + pEdge->data.weight;
            parent[adjacentVertex] = vertex;
            if (!isInQueue[adjacentVertex]) {
                Enqueue(queue, adjacentVertex);
                isInQueue[adjacentVertex] = 1;
            }
        }
    }
}