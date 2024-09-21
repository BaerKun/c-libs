#include "only_edge/graph.h"
#include <stdlib.h>
#include <stdio.h>

GraphPtr newGraph(const int edgeCapacity, const int vertexNum) {
    const GraphPtr graph = malloc(sizeof(Graph));

    for (graph->edgeCapacity = INITIAL_VERTICES_NUMBER; edgeCapacity > graph->edgeCapacity; graph->edgeCapacity *= 2);

    graph->edges = malloc(sizeof(Edge) * graph->edgeCapacity);
    graph->edgeNum = 0;
    graph->vertexNum = vertexNum;

    return graph;
}

void graphDestroy(GraphPtr graph) {
    free(graph->edges);
    free(graph);
}

void graphAddEdge(GraphPtr graph, const VertexId vertex1, const VertexId vertex2, const int weight) {
    if(graph->edgeNum == graph->edgeCapacity){
        fputs("graphAddEdge:Graph is full\n", stderr);
        return;
    }

    graph->edges[graph->edgeNum++] = (Edge){vertex1, vertex2, weight};
}