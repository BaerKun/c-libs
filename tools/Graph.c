#include "Graph.h"
#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct HeapStruct {
    int size;
    AdjacencyList *elements;
} *Heap;

static void InitIndegree(Graph graph, Vertex inDegree[]) {
    Vertex thisVertex;
    AdjacencyList adjacencyList;
    for (thisVertex = 0; thisVertex < graph->vertexNum; thisVertex++)
        inDegree[thisVertex] = 0;
    for (thisVertex = 0; thisVertex < graph->vertexNum; thisVertex++) {
        for (adjacencyList = (graph->adjacencyLists +
                              thisVertex)->next; adjacencyList; adjacencyList = adjacencyList->next) {
            inDegree[adjacencyList->vertex]++;
        }
    }
}

static void InitDistance(Graph graph) {
    Vertex thisVertex;
    for (thisVertex = 0; thisVertex < graph->vertexNum; thisVertex++)
        (graph->adjacencyLists + thisVertex)->value = INFINITY;
}

static Heap CreateHeap(int capacity) {
    Heap heap = malloc(sizeof(struct HeapStruct));
    heap->size = 0;
    heap->elements = malloc((capacity + 1) * sizeof(struct AdjacencyList));
    return heap;
}

static void HeapInsert(Heap heap, AdjacencyList list) {
    AdjacencyList *elements = heap->elements;
    Vertex i, father;
    for (i = ++heap->size; (father = i >> 1) && elements[father]->value > list->value; i = father)
        heap->elements[i] = elements[father];
    heap->elements[i] = list;
}

static AdjacencyList DeleteMin(Heap heap) {
    int father, child;
    AdjacencyList *elements = heap->elements;
    AdjacencyList theLast = elements[heap->size--];
    AdjacencyList output = elements[1];
    for (father = 1; father << 1 <= heap->size; father = child) {
        child = father << 1;
        if (child != heap->size && elements[child + 1]->value < elements[child]->value)
            child++;
        if (theLast->value > elements[child]->value)
            elements[father] = elements[child];
        else
            break;
    }
    elements[father] = theLast;
    return output;
}

static void DeleteHeap(Heap heap) {
    free(heap->elements);
    free(heap);
}

static Vertex copyPath(Graph graph, Vertex start, Vertex end, Vertex copyArray[]) {
    if (start == end) {
        *copyArray = start;
        return 1;
    }
    if (end == INFINITY) {
        fputs("copyPath:NoPath\n", stderr);
        return 0;
    }
    Vertex thisIndex = copyPath(graph, start, (graph->adjacencyLists + end)->vertex, copyArray);
    copyArray[thisIndex] = end;
    return thisIndex + 1;
}

Graph CreateGraph(int vertexNum) {
    Graph graph = malloc(sizeof(struct Graph));
    AdjacencyList theLists = malloc(sizeof(struct AdjacencyList) * vertexNum);
    AdjacencyList thisList;
    graph->vertexNum = vertexNum;
    graph->adjacencyLists = theLists;
    for (int i = 0; i < vertexNum; i++) {
        thisList = theLists + i;
        thisList->value = INFINITY;
        thisList->vertex = INFINITY;
        thisList->next = NULL;
    }
    return graph;
}

void DeleteGraph(Graph graph) {
    Vertex thisVertex;
    AdjacencyList adjacencyList, nextList;
    for (thisVertex = 0; thisVertex < graph->vertexNum; thisVertex++) {
        for (adjacencyList = (graph->adjacencyLists + thisVertex)->next; adjacencyList; adjacencyList = nextList) {
            nextList = adjacencyList->next;
            free(adjacencyList);
        }
    }
    free(graph->adjacencyLists);
    free(graph);
}

void Connect(Graph graph, Vertex start, Vertex end, int value, int isDirected) {
    AdjacencyList startList = graph->adjacencyLists + start;
    AdjacencyList newList;
    if (!isDirected)
        Connect(graph, end, start, value, 1);
    for (newList = startList->next; newList && newList->vertex != end; newList = newList->next);
    if (newList)
        return;
    newList = malloc(sizeof(struct AdjacencyList));
    newList->vertex = end;
    newList->value = value;
    newList->next = startList->next;
    startList->next = newList;
}

void TopSort(Graph graph, Vertex copyArray[]) {
    if (copyArray == NULL) {
        fputs("TopSort:Empty\n", stderr);
        return;
    }
    Vertex thisVertex;
    Vertex counter = 0;
    Vertex inDegree[graph->vertexNum];
    AdjacencyList adjacencyLists = graph->adjacencyLists;
    AdjacencyList adjacencyList;
    Queue queue = CreateQueue(graph->vertexNum);
    InitIndegree(graph, inDegree);
    for (thisVertex = 0; thisVertex < graph->vertexNum; thisVertex++) {
        inDegree[thisVertex] = (adjacencyLists + thisVertex)->value;
        if (!inDegree[thisVertex])
            Enqueue(queue, thisVertex);
    }
    while (queue->size) {
        thisVertex = Dequeue(queue);
        copyArray[counter++] = thisVertex;
        for (adjacencyList = (adjacencyLists + thisVertex)->next; adjacencyList; adjacencyList = adjacencyList->next)
            if (!--inDegree[adjacencyList->vertex])
                Enqueue(queue, adjacencyList->vertex);
    }
    if (counter != graph->vertexNum)
        fputs("TopSort:HasCycle\n", stderr);
    DeleteQueue(queue);
}

void BuildUnweightedPath(Graph graph, Vertex start, Vertex end) {
    Vertex thisVertex, adjacentVertex, distance;
    AdjacencyList adjacencyList, adjacentVertexList;
    AdjacencyList theLists = graph->adjacencyLists;
    Queue queue = CreateQueue(graph->vertexNum);
    InitDistance(graph);
    Enqueue(queue, start);
    (theLists + start)->value = 0;
    while (queue->size) {
        thisVertex = Dequeue(queue);
        distance = (theLists + thisVertex)->value + 1;
        for (adjacencyList = (theLists +
                              thisVertex)->next; adjacencyList; adjacencyList = adjacencyList->next) {
            adjacentVertex = adjacencyList->vertex;
            adjacentVertexList = theLists + adjacentVertex;
            if (adjacentVertexList->value == INFINITY) {
                adjacentVertexList->value = distance;
                adjacentVertexList->vertex = thisVertex;
                if (adjacentVertex == end) {
                    DeleteQueue(queue);
                    return;
                }
                Enqueue(queue, adjacentVertex);

            }
        }
    }
    DeleteQueue(queue);
}

// 无负权值
void BuildNonnegWeightedPath(Graph graph, Vertex start, Vertex end) {
    Vertex thisVertex, adjacentVertex;
    AdjacencyList thisList, adjacencyList, adjacentVertexList;
    AdjacencyList theLists = graph->adjacencyLists;
    Heap heap = CreateHeap(graph->vertexNum);
    char hasKnown[graph->vertexNum];
    for(int i = 0; i < graph->vertexNum; i++)
        hasKnown[i] = 0;
    InitDistance(graph);
    (theLists + start)->value = 0;
    HeapInsert(heap, theLists + start);
    while (heap->size) {
        thisList = DeleteMin(heap);
        thisVertex = (Vertex) (thisList - theLists);
        if (thisVertex == end) {
            DeleteHeap(heap);
            return;
        }
        hasKnown[thisVertex] = 1;
        for (adjacencyList = thisList->next; adjacencyList; adjacencyList = adjacencyList->next) {
            adjacentVertex = adjacencyList->vertex;
            adjacentVertexList = theLists + adjacentVertex;
            if (hasKnown[adjacentVertex] || adjacentVertexList->value <= thisList->value + adjacencyList->value)
                continue;
            adjacentVertexList->value = thisList->value + adjacencyList->value;
            adjacentVertexList->vertex = thisVertex;
            HeapInsert(heap, adjacentVertexList);

        }
    }
    DeleteHeap(heap);
}

// 无负值圈
void BuildWeightedPath(Graph graph, Vertex start){
    Vertex thisVertex, adjacentVertex;
    AdjacencyList thisList, adjacencyList, adjacentVertexList;
    AdjacencyList theLists = graph->adjacencyLists;
    Queue queue = CreateQueue(graph->vertexNum);
    char isInQueue[graph->vertexNum];
    for(int i = 0; i < graph->vertexNum; i++)
        isInQueue[i] = 0;
    InitDistance(graph);
    Enqueue(queue, start);
    (theLists + start)->value = 0;
    isInQueue[start] = 1;
    while(queue->size){
        thisVertex = Dequeue(queue);
        thisList = theLists + thisVertex;
        isInQueue[thisVertex] = 0;
        for(adjacencyList = thisList->next; adjacencyList; adjacencyList = adjacencyList->next){
            adjacentVertex = adjacencyList->vertex;
            adjacentVertexList = theLists + adjacentVertex;
            if(adjacentVertexList->value <= thisList->value + adjacencyList->value)
                continue;
            adjacentVertexList->value = thisList->value + adjacencyList->value;
            adjacentVertexList->vertex = thisVertex;
            if(!isInQueue[adjacentVertex]) {
                Enqueue(queue, adjacentVertex);
                isInQueue[adjacentVertex] = 1;
            }
        }
    }
    DeleteQueue(queue);
}

void CopyPath(Graph graph, Vertex copyArray[], Vertex start, Vertex end) {
    if (start < 0 || start >= graph->vertexNum || end < 0 || end >= graph->vertexNum) {
        fputs("CpoyPath:InputError\n", stderr);
        return;
    }
    copyPath(graph, start, end, copyArray);
}

int HasPath(Graph graph, Vertex start, Vertex end){
    if (start < 0 || start >= graph->vertexNum || end < 0 || end >= graph->vertexNum) {
        fputs("HasPath:InputError\n", stderr);
        return 0;
    }
    while(start != end){
        end = (graph->adjacencyLists + end)->vertex;
        if(end == INFINITY)
            return 0;
    }
    return 1;
}

int GetDistance(Graph graph, Vertex vertex) {
    if (vertex < 0 || vertex >= graph->vertexNum) {
        fputs("GetDistance:InputError\n", stderr);
        return 0;
    }
    return (graph->adjacencyLists + vertex)->value;
}