#include "min_spanning_tree.h"
#include "utils/heap.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>

static void printTree(GraphPtr pGraph, VertexId root, int level){
    for(int i = 0; i < level; i++)
        putchar('\t');
    printf("%d\n", root);

    EdgePtr pEdge;

    for(pEdge = pGraph->vertices[root].pEdge; pEdge; pEdge = pEdge->next){
        if(pGraph->vertices[pEdge->id].path == root)
            printTree(pGraph, pEdge->id, level + 1);
    }

}

void PrimMinSpanningTree(GraphPtr pGraph, VertexId root) {
    VertexId vertexId, adjacentVertexId;
    EdgePtr pEdge;
    VertexPtr vertices, pVertex, pAdjacentVertex;
    char hasKnown[pGraph->vertexNum];
    Heap heap;

    vertices = pGraph->vertices;
    vertices[root].path = root;
    memset(hasKnown, 0, pGraph->vertexNum);
    InitHeap(heap, pGraph->vertexNum);
    G_HeapInsert(&heap, &vertices[root].distance);

    while (heap.size) {
        pVertex = (void *)G_DeleteMin(&heap) - offsetof(Vertex, distance);
        vertexId = (VertexId) (pVertex - vertices);
        hasKnown[vertexId] = 1;
        for (pEdge = pVertex->pEdge; pEdge; pEdge = pEdge->next) {
            adjacentVertexId = pEdge->id;
            pAdjacentVertex = vertices + adjacentVertexId;
            if (!hasKnown[adjacentVertexId] && vertices[adjacentVertexId].distance > pEdge->weight){
                vertices[adjacentVertexId].distance = pEdge->weight;
                vertices[adjacentVertexId].path = vertexId;
                G_HeapInsert(&heap, &pAdjacentVertex->distance);
            }
        }
    }
}

void PrintTree(GraphPtr pGraph, VertexId root) {
    if (root < 0 || root >= pGraph->vertexNum || pGraph->vertices[root].path != root) {
        fputs("PrintTree: Invalid root vertex!\n", stderr);
        return;
    }

    printTree(pGraph, root, 0);
}