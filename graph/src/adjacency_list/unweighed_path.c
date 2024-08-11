#include "adjacency_list/unweighed_path.h"
#include "queue.h"

void buildUnweightedPath(GraphPtr graph, VertexId *parent, VertexId source, VertexId target) {
    VertexId thisVertex, adjacentVertex;
    EdgePtr edge;
    QueuePtr queue = newQueue(graph->vertexNum);

    for(thisVertex = 0; thisVertex < graph->vertexNum; thisVertex++)
        parent[thisVertex] = INFINITY;

    enqueue(queue, source);

    while (queue->front != queue->rear) {
        thisVertex = dequeue(queue);

        for (edge = graph->vertices[thisVertex].outEdges; edge; edge = edge->next) {
            adjacentVertex = edge->target;

            if (parent[adjacentVertex] == INFINITY) {
                parent[adjacentVertex] = thisVertex;
                if (adjacentVertex == target)
                    return;

                enqueue(queue, adjacentVertex);
            }
        }
    }

    queue_destroy(queue);
}