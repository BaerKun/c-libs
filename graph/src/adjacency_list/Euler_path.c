#include "adjacency_list/Euler_path.h"
#include <stdio.h>

typedef struct {
    GraphPtr pGraph;
    EdgePtr *pNextEdge;
    VertexId target;
} Package;

static EdgePtr GetNextEdge(VertexPtr pThisVertex, EdgePtr *ppNextEdge, VertexId lastVertex) {
    if (!*ppNextEdge)
        return NULL;

    if ((*ppNextEdge)->target == lastVertex)
        return *ppNextEdge = (*ppNextEdge)->next;


    EdgePtr pEdge, pNextEdge;
    for (pEdge = *ppNextEdge; (pNextEdge = pEdge->next) && pNextEdge->target != lastVertex; pEdge = pNextEdge);

    if (pNextEdge) {
        pEdge->next = pNextEdge->next;
        pNextEdge->next = pThisVertex->pOutEdge;
        pThisVertex->pOutEdge = pNextEdge;
    }

    return *ppNextEdge;
}

static int eulerCircuit(Package *pPackage, VertexListPtr path, VertexId thisVertex, VertexId lastVertex) {
    EdgePtr pThisEdge;
    pThisEdge = GetNextEdge(pPackage->pGraph->vertices + thisVertex, pPackage->pNextEdge + thisVertex, lastVertex);

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

void EulerPath(GraphPtr pGraph, VertexListPtr pPathHead, VertexId source, VertexId target) {
    EdgePtr pNextEdge[pGraph->vertexNum];

    for (VertexId vertex = 0; vertex < pGraph->vertexNum; vertex++)
        pNextEdge[vertex] = pGraph->vertices[vertex].pOutEdge;
    pPathHead->vertexId = source;
    pPathHead->next = NULL;
    Package package = {pGraph, pNextEdge, target};

    eulerCircuit(&package, pPathHead, source, INFINITY);
}

void EulerCircuit(GraphPtr pGraph, VertexListPtr pPathHead, VertexId source) {
    EulerPath(pGraph, pPathHead, source, source);
}