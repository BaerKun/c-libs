#include "adjacency_list/min_spanning_tree.h"
#include "utils/heap.h"
#include <stdio.h>

typedef struct{
    GraphPtr pGraph;
    VertexId *parent;
}Package;

static void printTree(Package *pPackage, VertexId root, int deepth){
    for(int i = 0; i < deepth; i++)
        printf("\t");
    printf("|-%d\n", root);

    EdgePtr pEdge;

    for(pEdge = pPackage->pGraph->vertices[root].pOutEdge; pEdge; pEdge = pEdge->next)
        if(pPackage->parent[pEdge->target] == root)
            printTree(pPackage, pEdge->target, deepth + 1);
}

void PrimMinSpanningTree(GraphPtr pGraph, VertexId root, VertexId *parent) {
    VertexId vertex, adjacentVertex;
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
        vertex = (VertexId) (G_DeleteMin(&heap) - minWeight);
        hasKnown[vertex] = 1;
        for (pEdge = pGraph->vertices[vertex].pOutEdge; pEdge; pEdge = pEdge->next) {
            adjacentVertex = pEdge->target;
            if (!hasKnown[adjacentVertex] && minWeight[adjacentVertex] > pEdge->data.weight){
                minWeight[adjacentVertex] = pEdge->data.weight;
                parent[adjacentVertex] = vertex;
                G_HeapInsert(&heap, minWeight + adjacentVertex);
            }
        }
    }
}

void PrintTree(GraphPtr pGraph, VertexId *parent, VertexId root) {
    if (root < 0 || root >= pGraph->vertexNum || parent[root] != root) {
        fputs("PrintTree: Invalid root vertex!\n", stderr);
        return;
    }
    Package package = {pGraph, parent};
    printTree(&package, root, 0);
}