#include <stdio.h>
#include "min_spanning_tree.h"

int main() {
    GraphPtr pGraph = CreateGraph(6);
    AddEdge(pGraph, 0, 1, 3, 0);
    AddEdge(pGraph, 0, 2, 2, 0);
    AddEdge(pGraph, 1, 3, 3, 0);
    AddEdge(pGraph, 1, 2, 1, 0);
    AddEdge(pGraph, 1, 4, 4, 0);
    AddEdge(pGraph, 2, 4, 2, 0);
    AddEdge(pGraph, 3, 5, 2, 0);
    AddEdge(pGraph, 4, 5, 3, 0);

    PrimMinSpanningTree(pGraph, 0);
    PrintTree(pGraph, 0);
    DeleteGraph(pGraph);
    return 0;
}