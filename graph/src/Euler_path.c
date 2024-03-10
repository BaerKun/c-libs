#include "Euler_path.h"
#include <stdio.h>

typedef struct {
    GraphPtr pGraph;
    EdgePtr *pNextEdge;
    Vertex target;
} Package;

static EdgePtr GetNextEdge(EdgePtr *ppHeader, EdgePtr *ppNextEdge, Vertex lastVertex) {
    if (!*ppNextEdge)
        return NULL;

    if ((*ppNextEdge)->target == lastVertex)
        return *ppNextEdge = (*ppNextEdge)->next;


    EdgePtr pEdge, pNextEdge;
    for (pEdge = *ppNextEdge; (pNextEdge = pEdge->next) && pNextEdge->target != lastVertex; pEdge = pNextEdge);

    if (pNextEdge) {
        pEdge->next = pNextEdge->next;
        pNextEdge->next = *ppHeader;
        *ppHeader = pNextEdge;
    }

    return *ppNextEdge;
}

static int eulerCircuit(Package *pPackage, VertexListPtr path, Vertex thisVertex, Vertex lastVertex) {
    EdgePtr pThisEdge;
    pThisEdge = GetNextEdge(pPackage->pGraph->edges + thisVertex, pPackage->pNextEdge + thisVertex, lastVertex);

    for (; pThisEdge; pThisEdge = pPackage->pNextEdge[thisVertex]) {

        pPackage->pNextEdge[thisVertex] = pThisEdge->next;

        VertexListInsert(path, pThisEdge->target);

        if(eulerCircuit(pPackage, path->next, pThisEdge->target, thisVertex))
            return 1;

        if (pThisEdge->target == pPackage->target)
            pPackage->target = thisVertex;
    }

    if(thisVertex != pPackage->target) {
        fputs("EulerCircuit: No Circuit\n", stderr);
        return 1;
    }
    return 0;
}

void EulerCircuit(GraphPtr pGraph, VertexListPtr pPathHead, Vertex source) {
    EdgePtr pNextEdge[pGraph->vertexNum];

    for (Vertex vertex = 0; vertex < pGraph->vertexNum; vertex++)
        pNextEdge[vertex] = pGraph->edges[vertex];
    pPathHead->vertex = source;
    pPathHead->next = NULL;
    Package package = {pGraph, pNextEdge, source};

    eulerCircuit(&package, pPathHead, source, INFINITY);
}

void EulerPath(GraphPtr pGraph, VertexListPtr pPathHead, Vertex source, Vertex target) {
    EdgePtr pNextEdge[pGraph->vertexNum];

    for (Vertex vertex = 0; vertex < pGraph->vertexNum; vertex++)
        pNextEdge[vertex] = pGraph->edges[vertex];
    pPathHead->vertex = source;
    pPathHead->next = NULL;
    Package package = {pGraph, pNextEdge, target};

    eulerCircuit(&package, pPathHead, source, INFINITY);
}