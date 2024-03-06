#include "with_indegree/unweighed_path.h"
#include "utils/queue.h"

void BuildUnweightedPath(GraphPtr pGraph, VertexId startId, VertexId endId) {
    VertexId vertexId, adjacentVertexId;
    WeightType distance;
    VertexPtr vertices, pVertex;
    EdgePtr pEdge;
    Queue queue;
    vertices = pGraph->vertices;

    InitQueue(queue, pGraph->vertexNum);
    InitGraph(pGraph);
    Enqueue(queue, startId);
    vertices[startId].distance = 0;

    while (queue.front != queue.rear) {
        vertexId = Dequeue(queue);
        distance = vertices[vertexId].distance + 1;
        for (pEdge = vertices[vertexId].pEdge; pEdge; pEdge = pEdge->next) {
            adjacentVertexId = pEdge->id;
            pVertex = vertices + adjacentVertexId;
            if (pVertex->distance == INFINITY) {
                pVertex->distance = distance;
                pVertex->path = vertexId;
                if (adjacentVertexId == endId)
                    return;
                Enqueue(queue, adjacentVertexId);

            }
        }
    }
}