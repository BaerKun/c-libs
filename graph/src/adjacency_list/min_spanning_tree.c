#include "adjacency_list/min_spanning_tree.h"
#include "utils/heap.h"
#include <stdio.h>

typedef struct{
    GraphPtr graph;
    VertexId *parent;
}Package;

static void printTreeHelper(Package *package, VertexId root, int deepth){
    for(int i = 0; i < deepth; i++)
        printf("\t");
    printf("|-%d\n", root);

    for(EdgePtr edge = package->graph->vertices[root].outEdges; edge; edge = edge->next)
        if(package->parent[edge->target] == root)
            printTreeHelper(package, edge->target, deepth + 1);
}

void PrimMinSpanningTree(GraphPtr graph, VertexId root, VertexId *parent) {
    VertexId vertex, adjacentVertex;
    EdgePtr thisEdge;
    char hasKnown[graph->vertexNum];
    int minWeight[graph->vertexNum];
    Heap heap;

    parent[root] = root;
    for(vertex = 0; vertex < graph->vertexNum; vertex++){
        hasKnown[vertex] = 0;
        minWeight[vertex] = INFINITY;
    }

    InitHeap(heap, graph->vertexNum);
    G_HeapInsert(&heap, minWeight + root);

    while (heap.size) {
        vertex = (VertexId) (G_DeleteMin(&heap) - minWeight);
        hasKnown[vertex] = 1;

        for (thisEdge = graph->vertices[vertex].outEdges; thisEdge; thisEdge = thisEdge->next) {
            adjacentVertex = thisEdge->target;

            if (!hasKnown[adjacentVertex] && minWeight[adjacentVertex] > thisEdge->data.weight){
                minWeight[adjacentVertex] = thisEdge->data.weight;
                parent[adjacentVertex] = vertex;
                G_HeapInsert(&heap, minWeight + adjacentVertex);
            }
        }
    }
}

void printTree(GraphPtr graph, VertexId *parent, VertexId root) {
    if (root < 0 || root >= graph->vertexNum || parent[root] != root) {
        fputs("printTree: Invalid root vertex!\n", stderr);
        return;
    }
    Package package = {graph, parent};
    printTreeHelper(&package, root, 0);
}