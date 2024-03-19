#include "adjacency_list/graph.h"
#include <stdlib.h>
#include <stdio.h>

GraphPtr CreateGraph(int capacity, int vertexNum) {
    GraphPtr pGraph = malloc(sizeof(Graph));
    pGraph->capacity = capacity;
    pGraph->vertexNum = vertexNum;
    pGraph->indegree = NULL;
    pGraph->vertices = calloc(capacity, sizeof(Vertex));

    return pGraph;
}

void DeleteGraph(GraphPtr pGraph) {
    VertexId vertexId;
    EdgePtr pEdge, pNextEdge;

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++) {
        for (pEdge = pGraph->vertices[vertexId].pOutEdge; pEdge; pEdge = pNextEdge) {
            pNextEdge = pEdge->next;
            free(pEdge);
        }
    }

    free(pGraph->vertices);

    if(pGraph->indegree)
        free(pGraph->indegree);

    free(pGraph);
}

void AddEdge(GraphPtr pGraph, VertexId source, VertexId target, EdgeData data) {
    if (source < 0 || target < 0 || source >= pGraph->vertexNum || target >= pGraph->vertexNum) {
        fputs("AddEdge:InvalidVertex\n", stderr);
        return;
    }

    EdgePtr pEdge;

    for (pEdge = pGraph->vertices[source].pOutEdge; pEdge && pEdge->target != target; pEdge = pEdge->next);
    if (pEdge)
        return;

    pEdge = malloc(sizeof(Edge));
    pEdge->target = target;
    pEdge->data = data;
    pEdge->next = pGraph->vertices[source].pOutEdge;
    pGraph->vertices[source].pOutEdge = pEdge;
    pGraph->edgeNum++;

    if(pGraph->indegree)
        pGraph->indegree[target]++;
}

int HasPath(const VertexId *parent, int vertexNum, VertexId source, VertexId target) {
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