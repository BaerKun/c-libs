#include "adjacency_list/min_spanning_tree.h"
#include <stdio.h>
#include <stdlib.h>
#define HEAP_ELEMENT_TYPE int *
#define HEAP_LESS_THAN(a, b) (*a < *b)
#include "heap.h"


static void printTreeHelper(VertexPtr vertices, VertexId *parent, VertexId root, int deepth){
    for(int i = 0; i < deepth; i++)
        printf("\t");
    printf("|-%d\n", root);

    for(EdgePtr edge = vertices[root].outEdges; edge; edge = edge->next)
        if(parent[edge->target] == root)
            printTreeHelper(vertices, parent, edge->target, deepth + 1);
}

void PrimMinSpanningTree(GraphPtr graph, VertexId root, VertexId *parent) {
    VertexId vertex, adjacentVertex;
    EdgePtr thisEdge;
    char *hasKnown = malloc(graph->vertexNum);
    int *minWeight = malloc(graph->vertexNum * sizeof(int));
    HeapPtr heap = newHeap(graph->vertexNum);

    parent[root] = root;
    for(vertex = 0; vertex < graph->vertexNum; vertex++){
        hasKnown[vertex] = 0;
        minWeight[vertex] = INFINITY;
    }

    heap_insert(heap, minWeight + root);

    while (heap->size) {
        vertex = (VertexId) (heap_deleteMin(heap) - minWeight);
        hasKnown[vertex] = 1;

        for (thisEdge = graph->vertices[vertex].outEdges; thisEdge; thisEdge = thisEdge->next) {
            adjacentVertex = thisEdge->target;

            if (!hasKnown[adjacentVertex] && minWeight[adjacentVertex] > thisEdge->data.weight){
                minWeight[adjacentVertex] = thisEdge->data.weight;
                parent[adjacentVertex] = vertex;
                heap_insert(heap, minWeight + adjacentVertex);
            }
        }
    }
    free(hasKnown);
    free(minWeight);
    heap_destroy(heap);
}

void printTree(GraphPtr graph, VertexId *parent, VertexId root) {
    if (root < 0 || root >= graph->vertexNum || parent[root] != root) {
        fputs("printTree: Invalid root vertex!\n", stderr);
        return;
    }
    printTreeHelper(graph->vertices, parent, root, 0);
}