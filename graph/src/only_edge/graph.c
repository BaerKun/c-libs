#include "only_edge/graph.h"
#include <stdlib.h>
#include <stdio.h>

GraphPtr newGraph(int edgeCapacity, int vertexNum) {
    GraphPtr pGraph = malloc(sizeof(Graph));

    for (pGraph->edgeCapacity = INITIAL_VERTICES_NUMBER; edgeCapacity > pGraph->edgeCapacity; pGraph->edgeCapacity *= 2);

    pGraph->edges = malloc(sizeof(Edge) * pGraph->edgeCapacity);
    pGraph->edgeNum = 0;
    pGraph->vertexNum = vertexNum;

    return pGraph;
}

void graphDestroy(GraphPtr pGraph) {
    free(pGraph->edges);
    free(pGraph);
}

void addEdge(GraphPtr pGraph, VertexId vertex1, VertexId vertex2, int weight) {
    if(pGraph->edgeNum == pGraph->edgeCapacity){
        fputs("graphAddEdge:Graph is full\n", stderr);
        return;
    }

    pGraph->edges[pGraph->edgeNum++] = (Edge){vertex1, vertex2, weight};
}