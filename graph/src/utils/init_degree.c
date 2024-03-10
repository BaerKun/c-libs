#include "utils/init_indegree.h"
#include <string.h>

void InitIndegree(GraphPtr pGraph, int *indegree, QueuePtr pQueue) {
    Vertex vertex;

    if(pGraph->indegree){
        for(vertex = 0; vertex < pGraph->vertexNum; vertex++) {
            if(!(indegree[vertex] = pGraph->indegree[vertex]) && pQueue)
                Enqueue(*pQueue, vertex);
        }
        return;
    }

    memset(indegree, 0, pGraph->vertexNum * sizeof(int));
    for(vertex = 0; vertex < pGraph->vertexNum; vertex++) {
        for (EdgePtr pEdge = pGraph->edges[vertex]; pEdge; pEdge = pEdge->next)
            indegree[pEdge->target]++;
    }

    if(pQueue){
        for(vertex = 0; vertex < pGraph->vertexNum; vertex++) {
            if(!indegree[vertex])
                Enqueue(*pQueue, vertex);
        }
    }
}
