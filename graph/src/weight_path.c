#include "weight_path.h"
#include "utils/queue.h"
#include "utils/heap.h"
#include <stddef.h>

void BuildNonnegWeightedPath(GraphPtr pGraph, VertexId startId, VertexId endId) {
    VertexId vertexId, adjacentVertexId;
    EdgePtr pEdge;
    VertexPtr vertices, pVertex, pAdjacentVertex;
    char hasKnown[pGraph->vertexNum];
    Heap heap;

    InitHeap(heap, pGraph->vertexNum);
    vertices = pGraph->vertices;

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++)
        hasKnown[vertexId] = 0;
    InitGraph(pGraph);
    vertices[startId].distance = 0;
    G_HeapInsert(&heap, &vertices[startId].distance);

    while (heap.size) {
        pVertex = (void *)G_DeleteMin(&heap) - offsetof(Vertex, distance);
        vertexId = (VertexId) (pVertex - vertices);
        if (vertexId == endId)
            return;
        hasKnown[vertexId] = 1;
        for (pEdge = pVertex->pEdge; pEdge; pEdge = pEdge->next) {
            adjacentVertexId = pEdge->id;
            pAdjacentVertex = vertices + adjacentVertexId;
            if (hasKnown[adjacentVertexId] || pAdjacentVertex->distance <= pVertex->distance + pEdge->weight)
                continue;
            pAdjacentVertex->distance = pVertex->distance + pEdge->weight;
            pAdjacentVertex->path = vertexId;
            G_HeapInsert(&heap, &pAdjacentVertex->distance);

        }
    }
}

// 无负值圈
void BuildWeightedPath(GraphPtr pGraph, VertexId startId) {
    VertexId vertexId, adjacentVertexId;
    EdgePtr pEdge;
    VertexPtr vertices, pVertex, pAdjacentVertex;
    char isInQueue[pGraph->vertexNum];
    Queue queue;
    vertices = pGraph->vertices;

    InitQueue(queue, pGraph->vertexNum);
    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++)
        isInQueue[vertexId] = 0;
    InitGraph(pGraph);
    Enqueue(queue, startId);
    vertices[startId].distance = 0;
    isInQueue[startId] = 1;

    while (queue.front != queue.rear) {
        vertexId = Dequeue(queue);
        pVertex = vertices + vertexId;
        isInQueue[vertexId] = 0;
        for (pEdge = pVertex->pEdge; pEdge; pEdge = pEdge->next) {
            adjacentVertexId = pEdge->id;
            pAdjacentVertex = vertices + adjacentVertexId;
            if (pAdjacentVertex->distance <= pVertex->distance + pEdge->weight)
                continue;
            pAdjacentVertex->distance = pVertex->distance + pEdge->weight;
            pAdjacentVertex->path = vertexId;
            if (!isInQueue[adjacentVertexId]) {
                Enqueue(queue, adjacentVertexId);
                isInQueue[adjacentVertexId] = 1;
            }
        }
    }
}