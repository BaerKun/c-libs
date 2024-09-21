#include "only_edge/min_spanning_tree.h"
#include "disjSet.h"

#define HEAP_ELEMENT_TYPE EdgePtr
#define HEAP_LESS_THAN(a, b) ((a)->weight < (b)->weight)
#include "heap.h"


void KruskalMinSpanningTree(GraphPtr graph, EdgeId outputArray[]) {
    int counter = 0;
    const HeapPtr heap = newHeap(graph->edgeNum);
    VertexId disjSet[graph->vertexNum];
    ClassId root1, root2;
    EdgePtr edge = graph->edges + graph->edgeNum - 1;

    for(EdgePtr *heapElement = heap->prev + 1; edge >= graph->edges; --edge, ++heapElement)
        *heapElement = edge;

    buildHeap(heap, graph->edgeNum);

    for(VertexId vertex = 0; vertex < graph->vertexNum; vertex++)
        disjSet[vertex] = -1;

    while (heap->size != 0){
        edge = heap_deleteMin(heap);
        root1 = disjSet_find(disjSet, edge->vertex1);
        root2 = disjSet_find(disjSet, edge->vertex2);

        if(root1 != root2){
            outputArray[counter++] = (EdgeId)(edge - graph->edges);
            disjSet_union(disjSet, root1, root2);
        }
    }
    if(counter + 1 != graph->vertexNum)
        printf("No spanning tree!\n");

    outputArray[counter] = -1;
    heap_destroy(heap);
}