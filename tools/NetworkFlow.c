#include "NetworkFlow.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define InitHeap(heap, capacity) \
    VertexPtr elements[capacity];  \
    heap.size = 0;                  \
    heap.elements = elements

#define InitQueue(queue, capacity) \
    VertexId elements[capacity];    \
    queue.front = queue.rear = 0;   \
    queue.elements = elements

#define Enqueue(queue, element) \
    queue.elements[queue.rear++] = element

#define Dequeue(queue) queue.elements[queue.front++]

typedef struct {
    int front;
    int rear;
    Vertex *elements;
} Queue;

NetworkPtr CreateNetwork(int vertexNum) {
    NetworkPtr pNetwork = malloc(sizeof(Network));

    for (pNetwork->capacity = INITIAL_VERTICES_NUMBER; pNetwork->capacity < vertexNum; pNetwork->capacity *= 2);
    pNetwork->vertexNum = vertexNum;
    pNetwork->edges = malloc(pNetwork->capacity * 8);
    *pNetwork->edges = malloc(pNetwork->capacity * pNetwork->capacity * sizeof(Edge));
    for (int i = 1; i < pNetwork->capacity; i++)
        pNetwork->edges[i] = pNetwork->edges[i - 1] + pNetwork->capacity;
    memset(*pNetwork->edges, 0, pNetwork->capacity * pNetwork->capacity * sizeof(Edge));

    return pNetwork;
}

void DeleteNetwork(NetworkPtr pNetwork) {
    free(pNetwork->edges);
    free(pNetwork);
}

void AddEdge(NetworkPtr pNetwork, Vertex source, Vertex target, FlowType capacity) {
    pNetwork->edges[source][target].capacity = capacity;
}

NetworkPtr CopyNetwork(NetworkPtr pNetwork) {
    NetworkPtr pCopyNetwork = malloc(sizeof(Network));
    memcpy(pCopyNetwork, pNetwork, sizeof(Network));
    pCopyNetwork->edges = malloc(pNetwork->capacity * 8);
    *pCopyNetwork->edges = malloc(pNetwork->capacity * pNetwork->capacity * sizeof(Edge));
    for (int i = 1; i < pNetwork->capacity; i++)
        pCopyNetwork->edges[i] = pCopyNetwork->edges[i - 1] + pNetwork->capacity;
    memcpy(*pCopyNetwork->edges, *pNetwork->edges, pCopyNetwork->capacity * pCopyNetwork->capacity * sizeof(Edge));

    return pCopyNetwork;
}

static int
BFS(NetworkPtr pNetwork, Queue queue, Vertex source, Vertex target, Vertex path[], FlowType *singlrPathFlow) {
    int vertexNum;
    Vertex thisVertex, outgoingVertex;
    Edge **edges;

    edges = pNetwork->edges;
    vertexNum = pNetwork->vertexNum;
    Enqueue(queue, source);

    for (thisVertex = 0; thisVertex < vertexNum; thisVertex++)
        path[thisVertex] = INFINITY;
    path[source] = source;

    while (queue.front != queue.rear) {
        thisVertex = Dequeue(queue);
        for (outgoingVertex = 0; outgoingVertex < vertexNum; outgoingVertex++) {
            if (edges[thisVertex][outgoingVertex].capacity && path[outgoingVertex] == INFINITY) {
                path[outgoingVertex] = thisVertex;
                if (edges[thisVertex][outgoingVertex].capacity < edges[path[thisVertex]][thisVertex].flow)
                    edges[thisVertex][outgoingVertex].flow = edges[thisVertex][outgoingVertex].capacity;
                else
                    edges[thisVertex][outgoingVertex].flow = edges[path[thisVertex]][thisVertex].flow;
                if (outgoingVertex == target) {
                    *singlrPathFlow = edges[thisVertex][outgoingVertex].flow;
                    return 1;
                }
                Enqueue(queue, outgoingVertex);
            }
        }
    }
    return 0;
}

FlowType EdmondKarpMaxFlow(NetworkPtr pNetwork, Vertex source, Vertex sink) {
    FlowType singlrPathFlow;
    Vertex thisVertex, incomingVertex, path[pNetwork->vertexNum], vertices[pNetwork->vertexNum];
    Queue queue;
    NetworkPtr pResidualNetwork;

    pResidualNetwork = CopyNetwork(pNetwork);
    queue.front = queue.rear = 0;
    queue.elements = vertices;
    pResidualNetwork->edges[source][source].flow = INFINITY;

    while (BFS(pResidualNetwork, queue, source, sink, path, &singlrPathFlow)) {
        pNetwork->edges[sink][sink].flow += singlrPathFlow;
        for (thisVertex = sink; thisVertex != source; thisVertex = path[thisVertex]) {
            incomingVertex = path[thisVertex];
            if (pNetwork->edges[incomingVertex][thisVertex].capacity)
                pNetwork->edges[incomingVertex][thisVertex].flow += singlrPathFlow;
            else
                pNetwork->edges[incomingVertex][thisVertex].flow -= singlrPathFlow;
            pResidualNetwork->edges[incomingVertex][thisVertex].flow = 0;
            pResidualNetwork->edges[incomingVertex][thisVertex].capacity -= singlrPathFlow;
            pResidualNetwork->edges[thisVertex][incomingVertex].capacity += singlrPathFlow;
        }
    }
    DeleteNetwork(pResidualNetwork);
    return pNetwork->edges[sink][sink].flow;
}