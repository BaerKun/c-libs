#include "only_edge/min_spanning_tree.h"
#include "utils/heap.h"
#include <stddef.h>
#include "utils/disjiont_set.h"

void KruskalMinSpanningTree(GraphPtr graph, EdgeId outputArray[]) {
    int counter = 0;
    EdgeId edgeId;
    EdgePtr edge;
    Heap heap;
    VertexId disjSet[graph->vertexNum];
    ClassId root1, root2;
    int offset = offsetof(Edge, weight);

    InitHeap(heap, graph->edgeNum);
    for(edgeId = 0; edgeId < graph->edgeNum; edgeId++)
        heap.elements[edgeId] = &graph->edges[edgeId].weight;

    G_BuildHeap(&heap, graph->edgeNum);

    for(VertexId vertex = 0; vertex < graph->vertexNum; vertex++)
        disjSet[vertex] = -1;

    while(counter < graph->vertexNum - 1){
        edge = (void *)G_DeleteMin(&heap) - offset;
        root1 = Find(disjSet, edge->vertex1);
        root2 = Find(disjSet, edge->vertex2);

        if(root1 != root2){
            outputArray[counter++] = (EdgeId)(edge - graph->edges);
            Union(disjSet, root1, root2);
        }
    }
}