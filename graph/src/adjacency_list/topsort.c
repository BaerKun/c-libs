#include "adjacency_list/topsort.h"
#include "utils/init_indegree.h"
#include "utils/queue.h"
#include <stdio.h>

void BuildTopPath(GraphPtr pGraph, VertexId *parent) {
    VertexId thisVertex, adjacentVertex;
    EdgePtr pEdge;
    Queue queue;
    int inDegree[pGraph->vertexNum];
    int counter = 0;

    InitQueue(queue, pGraph->vertexNum);
    InitIndegree(pGraph, inDegree, &queue);
    for (thisVertex = 0; thisVertex < pGraph->vertexNum; thisVertex++)
        parent[thisVertex] = INFINITY;

    while (queue.front != queue.rear) {
        counter++;
        thisVertex = Dequeue(queue);
        for (pEdge = pGraph->vertices[thisVertex].pOutEdge; pEdge; pEdge = pEdge->next) {
            adjacentVertex = pEdge->target;
            if (parent[adjacentVertex] == INFINITY)
                parent[adjacentVertex] = thisVertex;
            if (!--inDegree[adjacentVertex])
                Enqueue(queue, adjacentVertex);
        }
    }

    if (counter != pGraph->vertexNum)
        fputs("BuildTopPath: Has Cycle\n", stderr);
}

void Topsort(GraphPtr pGraph, VertexId *sortArray) {
    VertexId thisVertex;
    EdgePtr pEdge;
    Queue queue;
    int inDegree[pGraph->vertexNum];
    int counter = 0;

    InitQueue(queue, pGraph->vertexNum);
    InitIndegree(pGraph, inDegree, &queue);

    while (queue.front != queue.rear) {
        thisVertex = Dequeue(queue);
        sortArray[counter++] = thisVertex;
        for (pEdge = pGraph->vertices[thisVertex].pOutEdge; pEdge; pEdge = pEdge->next) {
            if (!--inDegree[pEdge->target])
                Enqueue(queue, pEdge->target);
        }
    }

    if (counter != pGraph->vertexNum)
        fputs("Topsort: Has Cycle\n", stderr);
}