#include "matrix/graph.h"
#include <stdlib.h>

static void initGraph(GraphPtr graph) {
    for (VertexId source = 0; source < graph->vertexNum; source++) {
        for (VertexId target = 0; target < graph->vertexNum; target++) {
            graph->edges[source][target] = (source == target) ? INITIAL_SELF_POINTING_EDGE : NO_EDGE;
        }
    }
}

GraphPtr createGraph(int capacity, int vertexNum) {
    GraphPtr graph = (GraphPtr) malloc(sizeof(Graph));
    if(graph == NULL)
        return NULL;

    graph->capacity = capacity;
    graph->edgeNum = 0;
    graph->vertexNum = vertexNum;
    graph->vertices = (Vertex *) malloc(sizeof(Vertex) * capacity);
    graph->edges = (Edge (*)[MAX_VERTEX]) malloc(sizeof(Edge) * capacity * MAX_VERTEX);

    initGraph(graph);

    return graph;
}

void deleteGraph(GraphPtr graph) {
    free(graph->vertices);
    free(graph->edges);
    free(graph);
}

void addEdge(GraphPtr graph, const VertexId source, const VertexId target, const WeightType weight) {
    graph->edges[source][target] = (Edge) {weight, target};
    graph->edgeNum++;
}