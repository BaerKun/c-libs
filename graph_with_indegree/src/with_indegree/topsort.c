#include "with_indegree/graph.h"
#include "with_indegree/topsort.h"
#include "utils/queue.h"
#include "with_indegree/init_degree.h"
#include <stdio.h>

void BuildTopPath(GraphPtr pGraph) {
    VertexId vertexId, adjacentVertexId;
    EdgePtr pEdge;
    VertexPtr vertices = pGraph->vertices;
    Queue queue;
    int inDegree[pGraph->vertexNum];
    int counter = 0;

    InitQueue(queue, pGraph->vertexNum);
    G_InitIndegree(pGraph, inDegree);
    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++) {
        vertices[vertexId].path = INFINITY;
        if (inDegree[vertexId])
            vertices[vertexId].distance = INFINITY;
        else {
            vertices[vertexId].distance = 0;
            Enqueue(queue, vertexId);
        }
    }

    while (queue.front != queue.rear) {
        counter++;
        vertexId = Dequeue(queue);
        for (pEdge = vertices[vertexId].pEdge; pEdge; pEdge = pEdge->next) {
            adjacentVertexId = pEdge->id;
            if (vertices[adjacentVertexId].distance > vertices[vertexId].distance + pEdge->weight) {
                vertices[adjacentVertexId].distance = vertices[vertexId].distance + pEdge->weight;
                vertices[adjacentVertexId].path = vertexId;
            }
            if (!--inDegree[adjacentVertexId])
                Enqueue(queue, adjacentVertexId);
        }
    }

    if (counter != pGraph->vertexNum)
        fputs("BuildTopPath:HasCycle\n", stderr);
}

void TopSort(GraphPtr pGraph, VertexId inputArray[]) {
    VertexId vertexId, adjacentVertexId;
    EdgePtr pEdge;
    VertexPtr vertices = pGraph->vertices;
    Queue queue;
    int inDegree[pGraph->vertexNum];
    int counter = 0;

    InitQueue(queue, pGraph->vertexNum);
    G_InitIndegree(pGraph, inDegree);
    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++) {
        vertices[vertexId].path = INFINITY;
        if (inDegree[vertexId])
            vertices[vertexId].distance = INFINITY;
        else {
            vertices[vertexId].distance = 0;
            Enqueue(queue, vertexId);
        }
    }

    while (queue.front != queue.rear) {
        vertexId = Dequeue(queue);
        inputArray[counter++] = vertexId;
        for (pEdge = vertices[vertexId].pEdge; pEdge; pEdge = pEdge->next) {
            adjacentVertexId = pEdge->id;
            if (vertices[adjacentVertexId].distance > vertices[vertexId].distance + pEdge->weight) {
                vertices[adjacentVertexId].distance = vertices[vertexId].distance + pEdge->weight;
                vertices[adjacentVertexId].path = vertexId;
            }
            if (!--inDegree[adjacentVertexId])
                Enqueue(queue, adjacentVertexId);
        }
    }

    if (counter != pGraph->vertexNum)
        fputs("BuildTopPath:HasCycle\n", stderr);
}