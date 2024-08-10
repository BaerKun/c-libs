#include "adjacency_list/unweighed_path.h"
#include "utils/queue.h"

void buildUnweightedPath(GraphPtr graph, VertexId *parent, VertexId source, VertexId target) {
    VertexId thisVertex, adjacentVertex;
    EdgePtr edge;
    Queue queue;

    for(thisVertex = 0; thisVertex < graph->vertexNum; thisVertex++)
        parent[thisVertex] = INFINITY;

    InitQueue(queue, graph->vertexNum);
    Enqueue(queue, source);

    while (queue.front != queue.rear) {
        thisVertex = Dequeue(queue);

        for (edge = graph->vertices[thisVertex].outEdges; edge; edge = edge->next) {
            adjacentVertex = edge->target;

            if (parent[adjacentVertex] == INFINITY) {
                parent[adjacentVertex] = thisVertex;
                if (adjacentVertex == target)
                    return;

                Enqueue(queue, adjacentVertex);
            }
        }
    }
}