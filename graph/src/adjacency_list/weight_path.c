#include "adjacency_list/weight_path.h"
#include "utils/queue.h"
#include "utils/heap.h"

#ifdef WEIGHT_FOR_WEIGHTED_PATH

void DijkstraWeightedPath(GraphPtr pGraph, VertexId source, VertexId target, VertexId *parent) {
    VertexId vertex, adjacentVertex;
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
        vertex = (VertexId) (G_DeleteMin(&heap) - distance);
        if (vertex == target)
            return;
        hasKnown[vertex] = 1;
        for (pEdge = pGraph->vertices[vertex].pOutEdge; pEdge; pEdge = pEdge->next) {
            adjacentVertex = pEdge->target;
            if (hasKnown[adjacentVertex] || distance[adjacentVertex] <= distance[vertex] + pEdge->data.WEIGHT_FOR_WEIGHTED_PATH)
                continue;
            distance[adjacentVertex] = distance[vertex] + pEdge->data.WEIGHT_FOR_WEIGHTED_PATH;
            parent[adjacentVertex] = vertex;
            G_HeapInsert(&heap, distance + adjacentVertex);
        }
    }
}

// 无负值圈
void WeightedPath(GraphPtr pGraph, VertexId source, VertexId *parent) {
    VertexId vertex, adjacentVertex;
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
        for (pEdge = pGraph->vertices[vertex].pOutEdge; pEdge; pEdge = pEdge->next) {
            adjacentVertex = pEdge->target;
            if (distance[adjacentVertex] <= distance[vertex] + pEdge->data.WEIGHT_FOR_WEIGHTED_PATH)
                continue;
            distance[adjacentVertex] = distance[vertex] + pEdge->data.WEIGHT_FOR_WEIGHTED_PATH;
            parent[adjacentVertex] = vertex;
            if (!isInQueue[adjacentVertex]) {
                Enqueue(queue, adjacentVertex);
                isInQueue[adjacentVertex] = 1;
            }
        }
    }
}

#endif