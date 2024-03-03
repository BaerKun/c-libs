#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>
#include "HeapForGraph.h"
#include "QueueForGraph.h"

typedef struct {
    int front;
    int rear;
    VertexId *elements;
}Queue;

typedef struct {
    int size;
    VertexPtr *elements;
} Heap, *HeapPtr;

static void InitIndegree(GraphPtr pGraph, int inDegree[]) {
    VertexId vertexId;

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++)
        inDegree[vertexId] = pGraph->vertices[vertexId].inDegree;
}

static void InitGraph(GraphPtr pGraph) {
    VertexId vertexId;

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++) {
        pGraph->vertices[vertexId].path = INFINITY;
        pGraph->vertices[vertexId].distance = INFINITY;
    }
}

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

static VertexId copyPath(GraphPtr pGraph, VertexId startId, VertexId endId, VertexId copyArray[]) {
    if (startId == endId) {
        *copyArray = startId;
        return 1;
    }

    if (endId < 0 || endId >= pGraph->vertexNum) {
        fputs("copyPath:NoPath\n", stderr);
        return 0;
    }

    VertexId thisIndex = copyPath(pGraph, startId, pGraph->vertices[endId].path, copyArray);
    copyArray[thisIndex] = endId;

    return thisIndex + 1;
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
        pVertex->inDegree = 0;
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

void Connect(GraphPtr pGraph, VertexId startId, VertexId endId, int weight, int isDirected) {
    if (startId < 0 || endId < 0 || startId >= pGraph->vertexNum || endId >= pGraph->vertexNum) {
        fputs("Connect:InvalidVertex\n", stderr);
        return;
    }

    if (!isDirected)
        Connect(pGraph, endId, startId, weight, 1);

    AdjacencyListPtr pNewList;
    VertexPtr pStartVertex = pGraph->vertices + startId;

    for (pNewList = pStartVertex->pAdjacencyList; pNewList && pNewList->id != endId; pNewList = pNewList->next);
    if (pNewList)
        return;

    pNewList = malloc(sizeof(AdjacencyList));
    pNewList->id = endId;
    pNewList->weight = weight;
    pNewList->next = pStartVertex->pAdjacencyList;
    pStartVertex->pAdjacencyList = pNewList;
    pGraph->vertices[endId].inDegree++;
}

void TopSort(GraphPtr pGraph, VertexId copyArray[]) {
    if (copyArray == NULL) {
        fputs("TopSort:Empty\n", stderr);
        return;
    }

    VertexId vertexId;
    AdjacencyListPtr pAdjacencyList;
    int inDegree[pGraph->vertexNum];
    Queue queue;
    int counter = 0;

    InitQueue(queue, pGraph->vertexNum, VertexId);
    InitIndegree(pGraph, inDegree);
    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++) {
        if (!inDegree[vertexId])
            Enqueue(queue, vertexId);
    }

    while (queue.front != queue.rear) {
        vertexId = Dequeue(queue);
        copyArray[counter++] = vertexId;
        for (pAdjacencyList = pGraph->vertices[vertexId].pAdjacencyList; pAdjacencyList; pAdjacencyList = pAdjacencyList->next)
            if (!--inDegree[pAdjacencyList->id])
                Enqueue(queue, pAdjacencyList->id);
    }

    if (counter != pGraph->vertexNum)
        fputs("TopSort:HasCycle\n", stderr);
}

void BuildUnweightedPath(GraphPtr pGraph, VertexId startId, VertexId endId) {
    VertexId vertexId, adjacentVertexId;
    WeightType distance;
    VertexPtr vertices, pVertex;
    AdjacencyListPtr pAdjacencyList;
    Queue queue;
    vertices = pGraph->vertices;

    InitQueue(queue, pGraph->vertexNum, VertexId);
    InitGraph(pGraph);
    Enqueue(queue, startId);
    vertices[startId].distance = 0;

    while (queue.front != queue.rear) {
        vertexId = Dequeue(queue);
        distance = vertices[vertexId].distance + 1;
        for (pAdjacencyList = vertices[vertexId].pAdjacencyList; pAdjacencyList; pAdjacencyList = pAdjacencyList->next) {
            adjacentVertexId = pAdjacencyList->id;
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

// 无负权值
void BuildNonnegWeightedPath(GraphPtr pGraph, VertexId startId, VertexId endId) {
    VertexId vertexId, adjacentVertexId;
    AdjacencyListPtr pAdjacencyList;
    VertexPtr vertices, pVertex, pAdjacentVertex;
    char hasKnown[pGraph->vertexNum];
    Heap heap;

    InitHeap(heap, pGraph->vertexNum, VertexPtr);
    vertices = pGraph->vertices;

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++)
        hasKnown[vertexId] = 0;
    InitGraph(pGraph);
    vertices[startId].distance = 0;
    HeapInsert(&heap, vertices + startId);

    while (heap.size) {
        pVertex = DeleteMin(&heap);
        vertexId = (VertexId) (pVertex - vertices);
        if (vertexId == endId)
            return;
        hasKnown[vertexId] = 1;
        for (pAdjacencyList = pVertex->pAdjacencyList; pAdjacencyList; pAdjacencyList = pAdjacencyList->next) {
            adjacentVertexId = pAdjacencyList->id;
            pAdjacentVertex = vertices + adjacentVertexId;
            if (hasKnown[adjacentVertexId] || pAdjacentVertex->distance <= pVertex->distance + pAdjacencyList->weight)
                continue;
            pAdjacentVertex->distance = pVertex->distance + pAdjacencyList->weight;
            pAdjacentVertex->path = vertexId;
            HeapInsert(&heap, pAdjacentVertex);

        }
    }
}

// 无负值圈
void BuildWeightedPath(GraphPtr pGraph, VertexId startId) {
    VertexId vertexId, adjacentVertexId;
    AdjacencyListPtr pAdjacencyList;
    VertexPtr vertices, pVertex, pAdjacentVertex;
    char isInQueue[pGraph->vertexNum];
    Queue queue;
    vertices = pGraph->vertices;

    InitQueue(queue, pGraph->vertexNum, VertexId);
    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++)
        isInQueue[vertexId] = 0;
    InitGraph(pGraph);
    Enqueue(queue, startId);
    vertices[startId].distance = 0;
    isInQueue[startId] = 1;

    while (queue.front != queue.rear) {
        vertexId = Dequeue(queue);
        pVertex = vertices + vertexId;
        isInQueue[vertexId] = 0;
        for (pAdjacencyList = pVertex->pAdjacencyList; pAdjacencyList; pAdjacencyList = pAdjacencyList->next) {
            adjacentVertexId = pAdjacencyList->id;
            pAdjacentVertex = vertices + adjacentVertexId;
            if (pAdjacentVertex->distance <= pVertex->distance + pAdjacencyList->weight)
                continue;
            pAdjacentVertex->distance = pVertex->distance + pAdjacencyList->weight;
            pAdjacentVertex->path = vertexId;
            if (!isInQueue[adjacentVertexId]) {
                Enqueue(queue, adjacentVertexId);
                isInQueue[adjacentVertexId] = 1;
            }
        }
    }
}

void CopyPath(GraphPtr pGraph, VertexId copyArray[], VertexId startId, VertexId endId) {
    if (startId < 0 || startId >= pGraph->vertexNum || endId < 0 || endId >= pGraph->vertexNum) {
        fputs("CpoyPath:InputError\n", stderr);
        return;
    }

    copyPath(pGraph, startId, endId, copyArray);
}

int HasPath(GraphPtr pGraph, VertexId startId, VertexId endId) {
    if (startId < 0 || startId >= pGraph->vertexNum || endId < 0 || endId >= pGraph->vertexNum) {
        fputs("HasPath:InputError\n", stderr);
        return 0;
    }

    while (startId != endId) {
        endId = pGraph->vertices[endId].path;
        if (endId < 0 || endId >= pGraph->vertexNum)
            return 0;
    }

    return 1;
}

WeightType GetDistance(GraphPtr pGraph, VertexId vertexId) {
    if (vertexId < 0 || vertexId >= pGraph->vertexNum) {
        fputs("GetDistance:InputError\n", stderr);
        return 0;
    }

    return pGraph->vertices[vertexId].distance;
}

void BuildTopPath(GraphPtr pGraph) {
    VertexId vertexId, adjacentVertexId;
    AdjacencyListPtr pAdjacencyList;
    VertexPtr vertices = pGraph->vertices;
    Queue queue;
    int inDegree[pGraph->vertexNum];
    int counter = 0;

    InitQueue(queue, pGraph->vertexNum, VertexId);
    InitIndegree(pGraph, inDegree);
    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++) {
        vertices[vertexId].path = INFINITY;
        if (inDegree[vertexId])
            vertices[vertexId].distance = INFINITY;
        else {
            vertices[vertexId].distance = 0;
            Enqueue(queue, vertexId);
        }
    }

    while (queue.front != queue.rear) {
        counter++;
        vertexId = Dequeue(queue);
        for (pAdjacencyList = vertices[vertexId].pAdjacencyList; pAdjacencyList; pAdjacencyList = pAdjacencyList->next) {
            adjacentVertexId = pAdjacencyList->id;
            if (vertices[adjacentVertexId].distance > vertices[vertexId].distance + pAdjacencyList->weight) {
                vertices[adjacentVertexId].distance = vertices[vertexId].distance + pAdjacencyList->weight;
                vertices[adjacentVertexId].path = vertexId;
            }
            if (!--inDegree[adjacentVertexId])
                Enqueue(queue, adjacentVertexId);
        }
    }

    if (counter != pGraph->vertexNum)
        fputs("BuildTopPath:HasCycle\n", stderr);
}