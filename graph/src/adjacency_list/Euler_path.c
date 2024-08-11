#include "adjacency_list/Euler_path.h"
#include <stdio.h>

typedef struct {
    GraphPtr graph;
    EdgePtr *nextEdge;
    VertexId target;
} Package;

static EdgePtr getEdge(VertexPtr thisVertex, EdgePtr *nextEdges, VertexId prevVertex) {
    if (!*nextEdges)
        return NULL;

    if ((*nextEdges)->target == prevVertex)
        return *nextEdges = (*nextEdges)->next;

    EdgePtr this, next;
    for (this = *nextEdges; (next = this->next) && next->target != prevVertex; this = next);

    if (next) {
        this->next = next->next;
        next->next = thisVertex->outEdges;
        thisVertex->outEdges = next;
    }

    return *nextEdges;
}

static int EulerCircuitHelper(Package *package, ListPtr path, VertexId thisVertex, VertexId prevVertex) {
    EdgePtr thisEdge;
    thisEdge = getEdge(package->graph->vertices + thisVertex, package->nextEdge + thisVertex, prevVertex);

    for (; thisEdge; thisEdge = package->nextEdge[thisVertex]) {

        package->nextEdge[thisVertex] = thisEdge->next;

        list_insertData(path, thisEdge->target);

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

void EulerPath(GraphPtr graph, ListPtr path, VertexId source, VertexId target) {
    EdgePtr *nextEdge = malloc(graph->vertexNum * sizeof(EdgePtr));

    for (VertexId vertex = 0; vertex < graph->vertexNum; vertex++)
        nextEdge[vertex] = graph->vertices[vertex].outEdges;

    path->element = source;
    path->next = NULL;
    Package package = {graph, nextEdge, target};

    EulerCircuitHelper(&package, path, source, INFINITY);

    free(nextEdge);
}

void EulerCircuit(GraphPtr pGraph, ListPtr path, VertexId source) {
    EulerPath(pGraph, path, source, source);
}
