#include "min_spanning_tree.h"
#include "utils/heap.h"
#include <stddef.h>
#include "utils/disjiont_set.h"

void KruskalMinSpanningTree(GraphPtr pGraph, EdgeId outputArray[]) {
    int counter = 0;
    EdgeId edgeId;
    EdgePtr pEdge;
    Heap heap;
    VertexId disjSet[pGraph->vertexNum];
    ClassId root1, root2;
    int offset = offsetof(Edge, weight);

    InitHeap(heap, pGraph->edgeNum);
    for(edgeId = 0; edgeId < pGraph->edgeNum; edgeId++)
        heap.elements[edgeId] = &pGraph->edges[edgeId].weight;
    G_BuildHeap(&heap, pGraph->edgeNum);
    for(VertexId vertex = 0; vertex < pGraph->vertexNum; vertex++)
        disjSet[vertex] = -1;

    while(counter < pGraph->vertexNum - 1){
        pEdge = (void *)G_DeleteMin(&heap) - offset;
        root1 = Find(disjSet, pEdge->vertex1);
        root2 = Find(disjSet, pEdge->vertex2);
        if(root1 != root2){
            outputArray[counter++] = (EdgeId)(pEdge - pGraph->edges);
            Union(disjSet, root1, root2);
        }
    }
}