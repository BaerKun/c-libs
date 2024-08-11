#include "utils/init_indegree.h"
#include <string.h>

void InitIndegree(GraphPtr graph, int *indegree, QueuePtr queue) {
    VertexId vertex;

    if(graph->indegree){
        for(vertex = 0; vertex < graph->vertexNum; vertex++) {
            if(!(indegree[vertex] = graph->indegree[vertex]) && queue)
                enqueue(queue, vertex);
        }
        return;
    }

    memset(indegree, 0, graph->vertexNum * sizeof(int));
    for(vertex = 0; vertex < graph->vertexNum; vertex++) {
        for (EdgePtr pEdge = graph->vertices[vertex].outEdges; pEdge; pEdge = pEdge->next)
            indegree[pEdge->target]++;
    }

    if(queue){
        for(vertex = 0; vertex < graph->vertexNum; vertex++) {
            if(!indegree[vertex])
                enqueue(queue, vertex);
        }
    }
}
