#include "MinSpanningTree.h"
#include "HeapForGraph.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int size;
    VertexPtr *elements;
} Heap, *HeapPtr;

static void HeapInsert(HeapPtr pHeap, VertexPtr pVertex) {
    VertexPtr *elements = pHeap->elements;
    int i, father;

    for (i = ++pHeap->size; (father = i >> 1) && elements[father]->distance > pVertex->distance; i = father)
        pHeap->elements[i] = elements[father];

    pHeap->elements[i] = pVertex;
}

static VertexPtr DeleteMin(HeapPtr pHeap) {
    VertexPtr *elements = pHeap->elements;
    VertexPtr pVertex = elements[pHeap->size--];
    VertexPtr output = elements[1];
    int father, child;

    for (father = 1; father << 1 <= pHeap->size; father = child) {
        child = father << 1;
        if (child != pHeap->size && elements[child + 1]->distance < elements[child]->distance)
            child++;
        if (pVertex->distance > elements[child]->distance)
            elements[father] = elements[child];
        else
            break;
    }
    elements[father] = pVertex;

    return output;
}

static void connect(GraphPtr pGraph, VertexId startId, VertexId endId, int weight){
    AdjacencyListPtr pNewList;

    pNewList = malloc(sizeof(AdjacencyList));
    pNewList->id = endId;
    pNewList->weight = weight;
    pNewList->next = pGraph->vertices[startId].pAdjacencyList;
    pGraph->vertices[startId].pAdjacencyList = pNewList;
}

GraphPtr CreateGraph(int vertexNum) {
    VertexPtr pVertex, vertices;
    GraphPtr graph = malloc(sizeof(Graph));

    for (graph->capacity = INITIAL_VERTICES_NUMBER; vertexNum > graph->capacity; graph->capacity *= 2);
    vertices = malloc(sizeof(Vertex) * graph->capacity);
    graph->vertexNum = vertexNum;
    graph->vertices = vertices;

    for (VertexId vertexId = 0; vertexId < vertexNum; vertexId++) {
        pVertex = vertices + vertexId;
        pVertex->distance = INFINITY;
        pVertex->path = INFINITY;
        pVertex->pAdjacencyList = NULL;
    }

    return graph;
}

void DeleteGraph(GraphPtr pGraph) {
    VertexId vertexId;
    AdjacencyListPtr pAdjacencyList, pNextList;

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++) {
        for (pAdjacencyList = pGraph->vertices[vertexId].pAdjacencyList; pAdjacencyList; pAdjacencyList = pNextList) {
            pNextList = pAdjacencyList->next;
            free(pAdjacencyList);
        }
    }

    free(pGraph->vertices);
    free(pGraph);
}

void Connect(GraphPtr pGraph, VertexId startId, VertexId endId, int weight) {
    if (startId < 0 || endId < 0 || startId >= pGraph->vertexNum || endId >= pGraph->vertexNum) {
        fputs("Connect:InvalidVertex\n", stderr);
        return;
    }
    AdjacencyListPtr pAdjacencyList;

    for (pAdjacencyList = pGraph->vertices[startId].pAdjacencyList; pAdjacencyList && pAdjacencyList->id != endId; pAdjacencyList = pAdjacencyList->next);
    if (pAdjacencyList)
        return;

    connect(pGraph, startId, endId, weight);
    connect(pGraph, endId, startId, weight);
}

void PrimMinSpanningTree(GraphPtr pGraph, VertexId root) {
    VertexId vertexId, adjacentVertexId;
    AdjacencyListPtr pAdjacencyList;
    VertexPtr vertices, pVertex, pAdjacentVertex;
    char hasKnown[pGraph->vertexNum];
    Heap heap;

    vertices = pGraph->vertices;
    InitHeap(heap, pGraph->vertexNum, VertexPtr);
    HeapInsert(&heap, vertices + root);

    while (heap.size) {
        pVertex = DeleteMin(&heap);
        vertexId = (VertexId) (pVertex - vertices);
        hasKnown[vertexId] = 1;
        for (pAdjacencyList = pVertex->pAdjacencyList; pAdjacencyList; pAdjacencyList = pAdjacencyList->next) {
            adjacentVertexId = pAdjacencyList->id;
            pAdjacentVertex = vertices + adjacentVertexId;
            if (!hasKnown[adjacentVertexId] && vertices[adjacentVertexId].distance > pAdjacencyList->weight){
                vertices[adjacentVertexId].distance = pAdjacencyList->weight;
                vertices[adjacentVertexId].path = vertexId;
                HeapInsert(&heap, pAdjacentVertex);
            }
        }
    }
}