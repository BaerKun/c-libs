#include "min_spanning_tree.h"
#include "stdio.h"

int main() {
    GraphPtr pGraph = CreateGraph(12, 7);

    AddEdge(pGraph, 0, 1, 2);
    AddEdge(pGraph, 0, 2, 4);
    AddEdge(pGraph, 0, 3, 1);
    AddEdge(pGraph, 1, 3, 3);
    AddEdge(pGraph, 1, 4, 10);
    AddEdge(pGraph, 2, 3, 2);
    AddEdge(pGraph, 2, 5, 5);
    AddEdge(pGraph, 3, 4, 7);
    AddEdge(pGraph, 3, 5, 8);
    AddEdge(pGraph, 3, 6, 4);
    AddEdge(pGraph, 5, 6, 1);
    AddEdge(pGraph, 4, 6, 6);
    EdgeId edges[6];
    KruskalMinSpanningTree(pGraph, edges);
    for(int i = 0; i < 6; i++)
        printf("%d %d %d\n", pGraph->edges[edges[i]].vertex1, pGraph->edges[edges[i]].vertex2, pGraph->edges[edges[i]].weight);
    DeleteGraph(pGraph);
    return 0;
}