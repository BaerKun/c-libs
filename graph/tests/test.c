#include <stdio.h>
#include <timer.h>

#include "adjacency_list/find_scc.h"

GraphPtr testSCC() {
    const GraphPtr graph = newGraph(100, 100);
    graphAddEdge(graph, 0, 1, NO_EDGE_DATA);
    graphAddEdge(graph, 1, 2, NO_EDGE_DATA);
    graphAddEdge(graph, 2, 3, NO_EDGE_DATA);
    graphAddEdge(graph, 3, 1, NO_EDGE_DATA);
    graphAddEdge(graph, 3, 0, NO_EDGE_DATA);
    return graph;
}

int main() {

    return 0;
}