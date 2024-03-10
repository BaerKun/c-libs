#include "min_spanning_tree.h"
#include "utils/heap.h"
#include <stdio.h>

static GraphPtr pGlobalGraph;
static Vertex *globalParent;

static void printTree(Vertex root, int deepth){
    for(int i = 0; i < deepth; i++)
        printf("\t");
    printf("|-%d\n", root);

    EdgePtr pEdge;

    for(pEdge = pGlobalGraph->edges[root]; pEdge; pEdge = pEdge->next)
        if(globalParent[pEdge->target] == root)
            printTree(pEdge->target, deepth + 1);
}

void PrimMinSpanningTree(GraphPtr pGraph, Vertex root, Vertex *parent) {
    Vertex vertex, adjacentVertex;
    EdgePtr pEdge;
    char hasKnown[pGraph->vertexNum];
    int minWeight[pGraph->vertexNum];
    Heap heap;

    parent[root] = root;
    for(vertex = 0; vertex < pGraph->vertexNum; vertex++){
        hasKnown[vertex] = 0;
        minWeight[vertex] = INFINITY;
    }
    InitHeap(heap, pGraph->vertexNum);
    G_HeapInsert(&heap, minWeight + root);

    while (heap.size) {
        vertex = (Vertex) (G_DeleteMin(&heap) - minWeight);
        hasKnown[vertex] = 1;
        for (pEdge = pGraph->edges[vertex]; pEdge; pEdge = pEdge->next) {
            adjacentVertex = pEdge->target;
            if (!hasKnown[adjacentVertex] && minWeight[adjacentVertex] > pEdge->data.weight){
                minWeight[adjacentVertex] = pEdge->data.weight;
                parent[adjacentVertex] = vertex;
                G_HeapInsert(&heap, minWeight + adjacentVertex);
            }
        }
    }
}

void PrintTree(GraphPtr pGraph, Vertex *parent, Vertex root) {
    if (root < 0 || root >= pGraph->vertexNum || parent[root] != root) {
        fputs("PrintTree: Invalid root vertex!\n", stderr);
        return;
    }
    pGlobalGraph = pGraph;
    globalParent = parent;
    printTree(root, 0);
}