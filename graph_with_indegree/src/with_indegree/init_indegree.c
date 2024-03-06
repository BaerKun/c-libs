#include "with_indegree/init_degree.h"

void G_InitIndegree(GraphPtr pGraph, int inDegree[]) {
    VertexId vertexId;

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++)
        inDegree[vertexId] = pGraph->vertices[vertexId].inDegree;
}
