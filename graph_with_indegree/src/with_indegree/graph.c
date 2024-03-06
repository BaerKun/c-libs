#include "with_indegree/graph.h"
#include <stdlib.h>
#include <stdio.h>

void InitGraph(GraphPtr pGraph) {
    VertexId vertexId;

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++) {
        pGraph->vertices[vertexId].path = INFINITY;
        pGraph->vertices[vertexId].distance = INFINITY;
    }
}

static VertexId copyPath(GraphPtr pGraph, VertexId startId, VertexId endId, VertexId copyArray[]) {
    if (startId == endId) {
        *copyArray = startId;
        return 1;
    }

    if (endId < 0 || endId >= pGraph->vertexNum) {
        fputs("copyPath:NoPath\n", stderr);
        return 0;
    }

    VertexId thisIndex = copyPath(pGraph, startId, pGraph->vertices[endId].path, copyArray);
    copyArray[thisIndex] = endId;

    return thisIndex + 1;
}

GraphPtr CreateGraph(int vertexNum) {
    VertexPtr pVertex, vertices;
    GraphPtr pGraph = malloc(sizeof(Graph));

    for (pGraph->capacity = INITIAL_VERTICES_NUMBER; vertexNum > pGraph->capacity; pGraph->capacity *= 2);
    vertices = malloc(sizeof(Vertex) * pGraph->capacity);
    pGraph->vertexNum = vertexNum;
    pGraph->vertices = vertices;

    for (VertexId vertexId = 0; vertexId < vertexNum; vertexId++) {
        pVertex = vertices + vertexId;
        pVertex->distance = INFINITY;
        pVertex->path = INFINITY;
        pVertex->pEdge = NULL;
        pVertex->inDegree = 0;
    }

    return pGraph;
}

void DeleteGraph(GraphPtr pGraph) {
    VertexId vertexId;
    EdgePtr pEdge, pNextEdge;

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++) {
        for (pEdge = pGraph->vertices[vertexId].pEdge; pEdge; pEdge = pNextEdge) {
            pNextEdge = pEdge->next;
            free(pEdge);
        }
    }

    free(pGraph->vertices);
    free(pGraph);
}

void AddEdge(GraphPtr pGraph, VertexId startId, VertexId endId, int weight, int isDirected) {
    if (startId < 0 || endId < 0 || startId >= pGraph->vertexNum || endId >= pGraph->vertexNum) {
        fputs("Connect:InvalidVertex\n", stderr);
        return;
    }

    if (!isDirected)
        Connect(pGraph, endId, startId, weight, 1);

    EdgePtr pNewList;
    VertexPtr pStartVertex = pGraph->vertices + startId;

    for (pNewList = pStartVertex->pEdge; pNewList && pNewList->id != endId; pNewList = pNewList->next);
    if (pNewList)
        return;

    pNewList = malloc(sizeof(Edge));
    pNewList->id = endId;
    pNewList->weight = weight;
    pNewList->next = pStartVertex->pEdge;
    pStartVertex->pEdge = pNewList;
    pGraph->vertices[endId].inDegree++;
}

void CopyPath(GraphPtr pGraph, VertexId copyArray[], VertexId startId, VertexId endId) {
    if (startId < 0 || startId >= pGraph->vertexNum || endId < 0 || endId >= pGraph->vertexNum) {
        fputs("CpoyPath:InputError\n", stderr);
        return;
    }

    copyPath(pGraph, startId, endId, copyArray);
}

int HasPath(GraphPtr pGraph, VertexId startId, VertexId endId) {
    if (startId < 0 || startId >= pGraph->vertexNum || endId < 0 || endId >= pGraph->vertexNum) {
        fputs("HasPath:InputError\n", stderr);
        return 0;
    }

    while (startId != endId) {
        endId = pGraph->vertices[endId].path;
        if (endId < 0 || endId >= pGraph->vertexNum)
            return 0;
    }

    return 1;
}

WeightType GetDistance(GraphPtr pGraph, VertexId vertexId) {
    if (vertexId < 0 || vertexId >= pGraph->vertexNum) {
        fputs("GetDistance:InputError\n", stderr);
        return 0;
    }

    return pGraph->vertices[vertexId].distance;
}
