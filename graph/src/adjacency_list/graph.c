#include "adjacency_list/graph.h"
#include <stdlib.h>
#include <stdio.h>

GraphPtr newGraph(int capacity, int vertexNum) {
    GraphPtr pGraph = malloc(sizeof(Graph));
    pGraph->capacity = capacity;
    pGraph->vertexNum = vertexNum;
    pGraph->indegree = NULL;
    pGraph->vertices = calloc(capacity, sizeof(Vertex));

    return pGraph;
}

void graphDestroy(GraphPtr graph) {
    VertexPtr vertex, end;
    EdgePtr this, next;

    for (vertex = graph->vertices, end = vertex + graph->vertexNum; vertex != end; vertex++) {
        for (this = vertex->outEdges; this; this = next) {
            next = this->next;
            free(this);
        }
    }

    free(graph->vertices);

    if(graph->indegree)
        free(graph->indegree);

    free(graph);
}

void graphAddEdge(GraphPtr graph, VertexId source, VertexId target, EdgeData data) {
    if (source < 0 || target < 0 || source >= graph->vertexNum || target >= graph->vertexNum) {
        fputs("graphAddEdge:InvalidVertex\n", stderr);
        return;
    }

    EdgePtr edge;
    for (edge = graph->vertices[source].outEdges; edge && edge->target != target; edge = edge->next);
    if (edge)
        return;

    edge = malloc(sizeof(Edge));
    edge->target = target;
    edge->data = data;
    edge->next = graph->vertices[source].outEdges;
    graph->vertices[source].outEdges = edge;
    graph->edgeNum++;

    if(graph->indegree)
        graph->indegree[target]++;
}

int graphHasPath(const VertexId *parent, int vertexNum, VertexId source, VertexId target) {
    if(source < 0 || source >= vertexNum)
        return 0;

    while(source != target){
        if(target < 0 || target >= vertexNum)
            return 0;
        target = parent[target];
    }

    return 1;
}

void UseIndegree(GraphPtr graph) {
    if(!graph->indegree)
        graph->indegree =  calloc(graph->capacity, sizeof(int));
}