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

    EdgePtr pEdge;

    for (pEdge = graph->vertices[source].outEdges; pEdge && pEdge->target != target; pEdge = pEdge->next);
    if (pEdge)
        return;

    pEdge = malloc(sizeof(Edge));
    pEdge->target = target;
    pEdge->data = data;
    pEdge->next = graph->vertices[source].outEdges;
    graph->vertices[source].outEdges = pEdge;
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