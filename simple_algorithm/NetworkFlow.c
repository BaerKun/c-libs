#include "NetworkFlow.h"
#include <stdlib.h>
#include <string.h>
#include "queue.h"


NetworkPtr CreateNetwork(int vertexNum) {
    const NetworkPtr network = malloc(sizeof(Network));

    for (network->capacity = INITIAL_VERTICES_NUMBER; network->capacity < vertexNum; network->capacity *= 2);

    network->vertexNum = vertexNum;
    network->edges = malloc(network->capacity * 8);
    *network->edges = malloc(network->capacity * network->capacity * sizeof(Edge));

    for (int i = 1; i < network->capacity; i++)
        network->edges[i] = network->edges[i - 1] + network->capacity;

    memset(*network->edges, 0, network->capacity * network->capacity * sizeof(Edge));

    return network;
}

void DeleteNetwork(NetworkPtr pNetwork) {
    free(pNetwork->edges);
    free(pNetwork);
}

void AddEdge(NetworkPtr network, Vertex source, Vertex target, FlowType capacity) {
    network->edges[source][target].capacity = capacity;
}

NetworkPtr CopyNetwork(NetworkPtr network) {
    const NetworkPtr copyNetwork = malloc(sizeof(Network));
    memcpy(copyNetwork, network, sizeof(Network));

    copyNetwork->edges = malloc(network->capacity * sizeof(EdgePtr));
    *copyNetwork->edges = malloc(network->capacity * network->capacity * sizeof(Edge));

    for (int i = 1; i < network->capacity; i++)
        copyNetwork->edges[i] = copyNetwork->edges[i - 1] + network->capacity;
    memcpy(*copyNetwork->edges, *network->edges, copyNetwork->capacity * copyNetwork->capacity * sizeof(Edge));

    return copyNetwork;
}

static int
BFS(NetworkPtr network, QueuePtr queue, Vertex source, Vertex target, Vertex path[], FlowType *singlrPathFlow) {
    const EdgePtr *edges = network->edges;
    const int vertexNum = network->vertexNum;

    for (Vertex thisVertex = 0; thisVertex < vertexNum; thisVertex++)
        path[thisVertex] = INFINITY;

    path[source] = source;
    enqueue(queue, source);
    while (queue->front != queue->rear) {
        const Vertex thisVertex = dequeue(queue);

        for (Vertex outgoingVertex = 0; outgoingVertex < vertexNum; outgoingVertex++) {
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

                enqueue(queue, outgoingVertex);
            }
        }
    }
    return 0;
}

FlowType EdmondKarpMaxFlow(NetworkPtr network, Vertex source, Vertex sink) {
    FlowType singlrPathFlow;
    Vertex path[network->vertexNum], vertices[network->vertexNum];
    Queue queue;


    const NetworkPtr residualNetwork = CopyNetwork(network);
    queue.front = queue.rear = 0;
    queue.elements = vertices;
    residualNetwork->edges[source][source].flow = INFINITY;

    while (BFS(residualNetwork, &queue, source, sink, path, &singlrPathFlow)) {
        network->edges[sink][sink].flow += singlrPathFlow;
        for (Vertex thisVertex = sink; thisVertex != source; thisVertex = path[thisVertex]) {
            const Vertex incomingVertex = path[thisVertex];

            if (network->edges[incomingVertex][thisVertex].capacity)
                network->edges[incomingVertex][thisVertex].flow += singlrPathFlow;
            else
                network->edges[incomingVertex][thisVertex].flow -= singlrPathFlow;

            residualNetwork->edges[incomingVertex][thisVertex].flow = 0;
            residualNetwork->edges[incomingVertex][thisVertex].capacity -= singlrPathFlow;
            residualNetwork->edges[thisVertex][incomingVertex].capacity += singlrPathFlow;
        }
    }

    DeleteNetwork(residualNetwork);
    return network->edges[sink][sink].flow;
}
