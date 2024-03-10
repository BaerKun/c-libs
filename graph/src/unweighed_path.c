#include "unweighed_path.h"
#include "utils/queue.h"

void BuildUnweightedPath(GraphPtr pGraph, Vertex *parent, Vertex source, Vertex target) {
    Vertex thisVertex, adjacentVertex;
    EdgePtr pEdge;
    Queue queue;

    for(thisVertex = 0; thisVertex < pGraph->vertexNum; thisVertex++)
        parent[thisVertex] = INFINITY;
    InitQueue(queue, pGraph->vertexNum);
    Enqueue(queue, source);

    while (queue.front != queue.rear) {
        thisVertex = Dequeue(queue);
        for (pEdge = pGraph->edges[thisVertex]; pEdge; pEdge = pEdge->next) {
            adjacentVertex = pEdge->target;
            if (parent[adjacentVertex] == INFINITY) {
                parent[adjacentVertex] = thisVertex;
                if (adjacentVertex == target)
                    return;
                Enqueue(queue, adjacentVertex);
            }
        }
    }
}