#include "adjacency_list/topsort.h"
#include "utils/init_indegree.h"
#include "utils/queue.h"
#include <stdio.h>

void buildTopPath(GraphPtr graph, VertexId *parent) {
    VertexId thisVertex, adjacentVertex;
    EdgePtr thisEdge;
    Queue queue;
    int inDegree[graph->vertexNum];
    int counter = 0;

    InitQueue(queue, graph->vertexNum);
    InitIndegree(graph, inDegree, &queue);
    for (thisVertex = 0; thisVertex < graph->vertexNum; thisVertex++)
        parent[thisVertex] = INFINITY;

    while (queue.front != queue.rear) {
        counter++;
        thisVertex = Dequeue(queue);

        for (thisEdge = graph->vertices[thisVertex].outEdges; thisEdge; thisEdge = thisEdge->next) {
            adjacentVertex = thisEdge->target;
            if (parent[adjacentVertex] == INFINITY)
                parent[adjacentVertex] = thisVertex;
            if (!--inDegree[adjacentVertex])
                Enqueue(queue, adjacentVertex);
        }
    }

    if (counter != graph->vertexNum)
        fputs("buildTopPath: Has Cycle\n", stderr);
}

void topSort(GraphPtr graph, VertexId *sortArray) {
    VertexId thisVertex;
    EdgePtr pEdge;
    Queue queue;
    int inDegree[graph->vertexNum];
    int counter = 0;

    InitQueue(queue, graph->vertexNum);
    InitIndegree(graph, inDegree, &queue);

    while (queue.front != queue.rear) {
        thisVertex = Dequeue(queue);
        sortArray[counter++] = thisVertex;

        for (pEdge = graph->vertices[thisVertex].outEdges; pEdge; pEdge = pEdge->next) {
            if (!--inDegree[pEdge->target])
                Enqueue(queue, pEdge->target);
        }
    }

    if (counter != graph->vertexNum)
        fputs("topSort: Has Cycle\n", stderr);
}