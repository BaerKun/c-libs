#include "adjacency_list/topsort.h"
#include "utils/init_indegree.h"
#include "queue.h"
#include <stdio.h>

void buildTopPath(GraphPtr graph, VertexId *parent) {
    VertexId thisVertex, adjacentVertex;
    EdgePtr thisEdge;
    int counter = 0;
    int *inDegree = malloc(graph->vertexNum * sizeof(int));
    const QueuePtr queue = newQueue(graph->vertexNum);

    InitIndegree(graph, inDegree, queue);
    for (thisVertex = 0; thisVertex < graph->vertexNum; thisVertex++)
        parent[thisVertex] = INFINITY;

    while (queue->front != queue->rear) {
        counter++;
        thisVertex = dequeue(queue);

        for (thisEdge = graph->vertices[thisVertex].outEdges; thisEdge; thisEdge = thisEdge->next) {
            adjacentVertex = thisEdge->target;
            if (parent[adjacentVertex] == INFINITY)
                parent[adjacentVertex] = thisVertex;
            if (!--inDegree[adjacentVertex])
                enqueue(queue, adjacentVertex);
        }
    }

    if (counter != graph->vertexNum)
        fputs("buildTopPath: Has Cycle\n", stderr);

    free(inDegree);
    queue_destroy(queue);
}

void topSort(GraphPtr graph, VertexId *sortArray) {
    VertexId vertex;
    EdgePtr edge;
    int counter = 0;
    int *inDegree = malloc(graph->vertexNum * sizeof(int));
    const QueuePtr queue = newQueue(graph->vertexNum);

    InitIndegree(graph, inDegree, queue);

    while (queue->front != queue->rear) {
        vertex = dequeue(queue);
        sortArray[counter++] = vertex;

        for (edge = graph->vertices[vertex].outEdges; edge; edge = edge->next) {
            if (!--inDegree[edge->target])
                enqueue(queue, edge->target);
        }
    }

    if (counter != graph->vertexNum)
        fputs("topSort: Has Cycle\n", stderr);

    free(inDegree);
    queue_destroy(queue);
}