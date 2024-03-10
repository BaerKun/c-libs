#include "graph.h"
#include <stdlib.h>
#include <stdio.h>

GraphPtr CreateGraph(int capacity, int vertexNum) {
    GraphPtr pGraph = malloc(sizeof(Graph));
    pGraph->capacity = capacity;
    pGraph->vertexNum = vertexNum;
    pGraph->indegree = NULL;
    pGraph->parent = NULL;
    pGraph->vertexData = NULL;
    pGraph->edges = calloc(capacity,  sizeof(EdgePtr));

    return pGraph;
}

void DeleteGraph(GraphPtr pGraph) {
    Vertex vertex;
    EdgePtr pEdge, pNextEdge;

    for (vertex = 0; vertex < pGraph->vertexNum; vertex++) {
        for (pEdge = pGraph->edges[vertex]; pEdge; pEdge = pNextEdge) {
            pNextEdge = pEdge->next;
            free(pEdge);
        }
    }

    free(pGraph->edges);
    if(pGraph->indegree)
        free(pGraph->indegree);
    if(pGraph->parent)
        free(pGraph->parent);
    if(pGraph->vertexData)
        free(pGraph->vertexData);
    free(pGraph);
}

void AddEdge(GraphPtr pGraph, Vertex source, Vertex target, EdgeData data) {
    if (source < 0 || target < 0 || source >= pGraph->vertexNum || target >= pGraph->vertexNum) {
        fputs("AddEdge:InvalidVertex\n", stderr);
        return;
    }

    EdgePtr pEdge;

    for (pEdge = pGraph->edges[source]; pEdge && pEdge->target != target; pEdge = pEdge->next);
    if (pEdge)
        return;

    pEdge = malloc(sizeof(Edge));
    pEdge->target = target;
    pEdge->data = data;
    pEdge->next = pGraph->edges[source];
    pGraph->edges[source] = pEdge;

    if(pGraph->indegree)
        pGraph->indegree[target]++;
}

int HasPath(const Vertex *parent, int vertexNum, Vertex source, Vertex target) {
    if(source < 0 || source >= vertexNum)
        return 0;

    while(source != target){
        if(target < 0 || target >= vertexNum)
            return 0;
        target = parent[target];
    }

    return 1;
}

void UseIndegree(GraphPtr pGraph) {
    if(!pGraph->indegree)
        pGraph->indegree =  calloc(pGraph->capacity, sizeof(int));
}

void UseParent(GraphPtr pGraph) {
    if(!pGraph->parent)
        pGraph->parent = calloc(pGraph->capacity, sizeof(Vertex));
}