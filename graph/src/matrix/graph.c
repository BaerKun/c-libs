#include "matrix/graph.h"
#include <stdlib.h>

void initGraph(GraphPtr pGraph) {
    for (VertexId source = 0; source < pGraph->vertexNum; source++) {
        for (VertexId target = 0; target < pGraph->vertexNum; target++) {
            pGraph->edges[source][target] = (source == target) ? INITIAL_SELF_POINTING_EDGE : NO_EDGE;
        }
    }
}

GraphPtr createGraph(int capacity, int vertexNum) {
    GraphPtr pGraph = (GraphPtr) malloc(sizeof(Graph));
    pGraph->capacity = capacity;
    pGraph->edgeNum = 0;
    pGraph->vertexNum = vertexNum;
    pGraph->vertices = (Vertex *) malloc(sizeof(Vertex) * capacity);
    pGraph->edges = (Edge (*)[MAX_VERTEX]) malloc(sizeof(Edge) * capacity * MAX_VERTEX);

    initGraph(pGraph);

    return pGraph;
}

void deleteGraph(GraphPtr pGraph) {
    free(pGraph->vertices);
    free(pGraph->edges);
    free(pGraph);
}

void addEdge(GraphPtr pGraph, VertexId source, VertexId target, WeightType weight) {
    pGraph->edges[source][target] = (Edge) {weight, target};
    pGraph->edgeNum++;
}