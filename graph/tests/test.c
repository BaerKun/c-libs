#include <stdio.h>
#include "only_edge/min_spanning_tree.h"
#include <mach/mach_time.h>


int main() {
    GraphPtr graph = newGraph(16, 7);
    graphAddEdge(graph, 0, 1, 1);
    graphAddEdge(graph, 0, 3, 2);
    graphAddEdge(graph, 0, 5, 3);
    graphAddEdge(graph, 1, 2, 4);
    graphAddEdge(graph, 1, 3, 5);
    graphAddEdge(graph, 1, 5, 6);
    graphAddEdge(graph, 2, 5, 7);
    graphAddEdge(graph, 2, 3, 8);
    graphAddEdge(graph, 3, 4, 9);
    graphAddEdge(graph, 4, 5, 10);

    EdgeId outputArray[20];
    KruskalMinSpanningTree(graph, outputArray);
    for(int i = 0; outputArray[i] != -1; ++i){
        printf("%d\n", outputArray[i]);
    }

    return 0;
}