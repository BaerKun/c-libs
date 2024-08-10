#include "adjacency_list/weight_path.h"
#include "utils/queue.h"
#include "utils/heap.h"

#ifdef WEIGHT_FOR_WEIGHTED_PATH

void DijkstraWeightedPath(GraphPtr graph, VertexId source, VertexId target, VertexId *parent) {
    VertexId vertex, adjacentVertex;
    EdgePtr edge;
    char hasKnown[graph->vertexNum];
    int distance[graph->vertexNum];
    Heap heap;

    InitHeap(heap, graph->vertexNum);

    for (vertex = 0; vertex < graph->vertexNum; vertex++) {
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
        for (edge = graph->vertices[vertex].outEdges; edge; edge = edge->next) {
            adjacentVertex = edge->target;
            if (hasKnown[adjacentVertex] || distance[adjacentVertex] <= distance[vertex] + edge->data.WEIGHT_FOR_WEIGHTED_PATH)
                continue;
            distance[adjacentVertex] = distance[vertex] + edge->data.WEIGHT_FOR_WEIGHTED_PATH;
            parent[adjacentVertex] = vertex;
            G_HeapInsert(&heap, distance + adjacentVertex);
        }
    }
}

// 无负值圈
void weightedPath(GraphPtr graph, VertexId source, VertexId *parent) {
    VertexId vertex, adjacentVertex;
    EdgePtr edge;
    char isInQueue[graph->vertexNum];
    int distance[graph->vertexNum];
    Queue queue;

    InitQueue(queue, graph->vertexNum);
    for (vertex = 0; vertex < graph->vertexNum; vertex++) {
        isInQueue[vertex] = 0;
        distance[vertex] = INFINITY;
    }

    Enqueue(queue, source);
    distance[source] = 0;
    isInQueue[source] = 1;

    while (queue.front != queue.rear) {
        vertex = Dequeue(queue);
        isInQueue[vertex] = 0;

        for (edge = graph->vertices[vertex].outEdges; edge; edge = edge->next) {
            adjacentVertex = edge->target;

            if (distance[adjacentVertex] <= distance[vertex] + edge->data.WEIGHT_FOR_WEIGHTED_PATH)
                continue;

            distance[adjacentVertex] = distance[vertex] + edge->data.WEIGHT_FOR_WEIGHTED_PATH;
            parent[adjacentVertex] = vertex;

            if (!isInQueue[adjacentVertex]) {
                Enqueue(queue, adjacentVertex);
                isInQueue[adjacentVertex] = 1;
            }
        }
    }
}

#endif