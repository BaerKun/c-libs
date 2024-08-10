#include "adjacency_list/Euler_path.h"
#include <stdio.h>

typedef struct {
    GraphPtr graph;
    EdgePtr *nextEdge;
    VertexId target;
} Package;

static EdgePtr getEdge(VertexPtr thisVertex, EdgePtr *nextEdges, VertexId lastVertex) {
    if (!*nextEdges)
        return NULL;

    if ((*nextEdges)->target == lastVertex)
        return *nextEdges = (*nextEdges)->next;


    EdgePtr this, next;
    for (this = *nextEdges; (next = this->next) && next->target != lastVertex; this = next);

    if (next) {
        this->next = next->next;
        next->next = thisVertex->outEdges;
        thisVertex->outEdges = next;
    }

    return *nextEdges;
}

static int EulerCircuitHelper(Package *package, VertexListPtr path, VertexId thisVertex, VertexId lastVertex) {
    EdgePtr thisEdge;
    thisEdge = getEdge(package->graph->vertices + thisVertex, package->nextEdge + thisVertex, lastVertex);

    for (; thisEdge; thisEdge = package->nextEdge[thisVertex]) {

        package->nextEdge[thisVertex] = thisEdge->next;

        vertexList_insert(path, thisEdge->target);

        if(EulerCircuitHelper(package, path->next, thisEdge->target, thisVertex))
            return 1;

        if (thisEdge->target == package->target)
            package->target = thisVertex;
    }

    if(thisVertex != package->target) {
        fputs("EulerCircuit: No Circuit\n", stderr);
        return 1;
    }
    return 0;
}

void EulerPath(GraphPtr graph, VertexListPtr path, VertexId source, VertexId target) {
    EdgePtr nextEdge[graph->vertexNum];

    for (VertexId vertex = 0; vertex < graph->vertexNum; vertex++)
        nextEdge[vertex] = graph->vertices[vertex].outEdges;

    path->vertex = source;
    path->next = NULL;
    Package package = {graph, nextEdge, target};

    EulerCircuitHelper(&package, path, source, INFINITY);
}

void EulerCircuit(GraphPtr pGraph, VertexListPtr pPathHead, VertexId source) {
    EulerPath(pGraph, pPathHead, source, source);
}