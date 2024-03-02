#include "Graph.h"
#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct Heap {
    int size;
    VertexPtr *elements;
} Heap, *HeapPtr;

static void InitIndegree(GraphPtr pGraph, int inDegree[]) {
    VertexId vertexId;

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++)
        inDegree[vertexId] = pGraph->vertexes[vertexId].inDegree;
}

static void InitGraph(GraphPtr pGraph) {
    VertexId vertexId;

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++) {
        pGraph->vertexes[vertexId].path = INFINITY;
        pGraph->vertexes[vertexId].distance = INFINITY;
    }
}

static HeapPtr G_CreateHeap(int capacity) {
    HeapPtr pHeap = malloc(sizeof(Heap));

    pHeap->size = 0;
    pHeap->elements = malloc((capacity + 1) * sizeof(VertexPtr));

    return pHeap;
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

static void DeleteHeap(HeapPtr pHeap) {
    free(pHeap->elements);
    free(pHeap);
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

    VertexId thisIndex = copyPath(pGraph, startId, pGraph->vertexes[endId].path, copyArray);
    copyArray[thisIndex] = endId;

    return thisIndex + 1;
}

GraphPtr CreateGraph(int vertexNum) {
    VertexPtr pVertex, vertexes;
    GraphPtr graph = malloc(sizeof(Graph));

    for (graph->capacity = INITIAL_NODES_NUMBER; vertexNum > graph->capacity; graph->capacity *= 2);
    vertexes = malloc(sizeof(Vertex) * graph->capacity);
    graph->vertexNum = vertexNum;
    graph->vertexes = vertexes;

    for (VertexId vertexId = 0; vertexId < vertexNum; vertexId++) {
        pVertex = vertexes + vertexId;
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
        for (pAdjacencyList = pGraph->vertexes[vertexId].pAdjacencyList; pAdjacencyList; pAdjacencyList = pNextList) {
            pNextList = pAdjacencyList->next;
            free(pAdjacencyList);
        }
    }
    free(pGraph->vertexes);
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
    VertexPtr pStartVertex = pGraph->vertexes + startId;

    for (pNewList = pStartVertex->pAdjacencyList; pNewList && pNewList->id != endId; pNewList = pNewList->next);
    if (pNewList)
        return;

    pNewList = malloc(sizeof(AdjacencyList));
    pNewList->id = endId;
    pNewList->weight = weight;
    pNewList->next = pStartVertex->pAdjacencyList;
    pStartVertex->pAdjacencyList = pNewList;
    pGraph->vertexes[endId].inDegree++;
}

void TopSort(GraphPtr pGraph, VertexId copyArray[]) {
    if (copyArray == NULL) {
        fputs("TopSort:Empty\n", stderr);
        return;
    }

    VertexId vertexId;
    AdjacencyListPtr pAdjacencyList;
    int inDegree[pGraph->vertexNum];
    Queue pQueue = CreateQueue(pGraph->vertexNum);
    int counter = 0;

    InitIndegree(pGraph, inDegree);
    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++) {
        if (!inDegree[vertexId])
            Enqueue(pQueue, vertexId);
    }

    while (pQueue->size) {
        vertexId = Dequeue(pQueue);
        copyArray[counter++] = vertexId;
        for (pAdjacencyList = pGraph->vertexes[vertexId].pAdjacencyList; pAdjacencyList; pAdjacencyList = pAdjacencyList->next)
            if (!--inDegree[pAdjacencyList->id])
                Enqueue(pQueue, pAdjacencyList->id);
    }

    if (counter != pGraph->vertexNum)
        fputs("TopSort:HasCycle\n", stderr);

    DeleteQueue(pQueue);
}

void BuildUnweightedPath(GraphPtr pGraph, VertexId startId, VertexId endId) {
    VertexId vertexId, adjacentVertexId;
    WeightType distance;
    VertexPtr vertexes, pVertex;
    AdjacencyListPtr pAdjacencyList;
    Queue pQueue = CreateQueue(pGraph->vertexNum);
    vertexes = pGraph->vertexes;

    InitGraph(pGraph);
    Enqueue(pQueue, startId);
    vertexes[startId].distance = 0;

    while (pQueue->size) {
        vertexId = Dequeue(pQueue);
        distance = vertexes[vertexId].distance + 1;
        for (pAdjacencyList = vertexes[vertexId].pAdjacencyList; pAdjacencyList; pAdjacencyList = pAdjacencyList->next) {
            adjacentVertexId = pAdjacencyList->id;
            pVertex = vertexes + adjacentVertexId;
            if (pVertex->distance == INFINITY) {
                pVertex->distance = distance;
                pVertex->path = vertexId;
                if (adjacentVertexId == endId)
                    goto END;
                Enqueue(pQueue, adjacentVertexId);

            }
        }
    }
    END:
    DeleteQueue(pQueue);
}

// 无负权值
void BuildNonnegWeightedPath(GraphPtr pGraph, VertexId startId, VertexId endId) {
    VertexId vertexId, adjacentVertexId;
    AdjacencyListPtr pAdjacencyList;
    VertexPtr vertexes, pVertex, pAdjacentVertex;
    char hasKnown[pGraph->vertexNum];
    HeapPtr pHeap;

    pHeap = G_CreateHeap(pGraph->vertexNum);
    vertexes = pGraph->vertexes;

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++)
        hasKnown[vertexId] = 0;
    InitGraph(pGraph);
    vertexes[startId].distance = 0;
    HeapInsert(pHeap, vertexes + startId);

    while (pHeap->size) {
        pVertex = DeleteMin(pHeap);
        vertexId = (VertexId) (pVertex - vertexes);
        if (vertexId == endId)
            goto END;
        hasKnown[vertexId] = 1;
        for (pAdjacencyList = pVertex->pAdjacencyList; pAdjacencyList; pAdjacencyList = pAdjacencyList->next) {
            adjacentVertexId = pAdjacencyList->id;
            pAdjacentVertex = vertexes + adjacentVertexId;
            if (hasKnown[adjacentVertexId] || pAdjacentVertex->distance <= pVertex->distance + pAdjacencyList->weight)
                continue;
            pAdjacentVertex->distance = pVertex->distance + pAdjacencyList->weight;
            pAdjacentVertex->path = vertexId;
            HeapInsert(pHeap, pAdjacentVertex);

        }
    }

    END:
    DeleteHeap(pHeap);
}

// 无负值圈
void BuildWeightedPath(GraphPtr pGraph, VertexId startId) {
    VertexId vertexId, adjacentVertexId;
    AdjacencyListPtr pAdjacencyList;
    VertexPtr vertexes, pVertex, pAdjacentVertex;
    char isInQueue[pGraph->vertexNum];
    Queue pQueue;

    vertexes = pGraph->vertexes;
    pQueue = CreateQueue(pGraph->vertexNum);

    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++)
        isInQueue[vertexId] = 0;
    InitGraph(pGraph);
    Enqueue(pQueue, startId);
    vertexes[startId].distance = 0;
    isInQueue[startId] = 1;

    while (pQueue->size) {
        vertexId = Dequeue(pQueue);
        pVertex = vertexes + vertexId;
        isInQueue[vertexId] = 0;
        for (pAdjacencyList = pVertex->pAdjacencyList; pAdjacencyList; pAdjacencyList = pAdjacencyList->next) {
            adjacentVertexId = pAdjacencyList->id;
            pAdjacentVertex = vertexes + adjacentVertexId;
            if (pAdjacentVertex->distance <= pVertex->distance + pAdjacencyList->weight)
                continue;
            pAdjacentVertex->distance = pVertex->distance + pAdjacencyList->weight;
            pAdjacentVertex->path = vertexId;
            if (!isInQueue[adjacentVertexId]) {
                Enqueue(pQueue, adjacentVertexId);
                isInQueue[adjacentVertexId] = 1;
            }
        }
    }

    DeleteQueue(pQueue);
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
        endId = pGraph->vertexes[endId].path;
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

    return pGraph->vertexes[vertexId].distance;
}

void BuildTopPath(GraphPtr pGraph) {
    VertexId vertexId, adjacentVertexId;
    AdjacencyListPtr pAdjacencyList;
    VertexPtr vertexes = pGraph->vertexes;
    Queue pQueue = CreateQueue(pGraph->vertexNum);
    int inDegree[pGraph->vertexNum];
    int counter = 0;

    InitIndegree(pGraph, inDegree);
    for (vertexId = 0; vertexId < pGraph->vertexNum; vertexId++) {
        vertexes[vertexId].path = INFINITY;
        if (inDegree[vertexId])
            vertexes[vertexId].distance = INFINITY;
        else {
            vertexes[vertexId].distance = 0;
            Enqueue(pQueue, vertexId);
        }
    }

    while (pQueue->size) {
        counter++;
        vertexId = Dequeue(pQueue);
        for (pAdjacencyList = vertexes[vertexId].pAdjacencyList; pAdjacencyList; pAdjacencyList = pAdjacencyList->next) {
            adjacentVertexId = pAdjacencyList->id;
            if (vertexes[adjacentVertexId].distance > vertexes[vertexId].distance + pAdjacencyList->weight) {
                vertexes[adjacentVertexId].distance = vertexes[vertexId].distance + pAdjacencyList->weight;
                vertexes[adjacentVertexId].path = vertexId;
            }
            if (!--inDegree[adjacentVertexId])
                Enqueue(pQueue, adjacentVertexId);
        }
    }

    if (counter != pGraph->vertexNum)
        fputs("BuildTopPath:HasCycle\n", stderr);

    DeleteQueue(pQueue);
}