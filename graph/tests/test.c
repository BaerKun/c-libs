#include <stdio.h>
#include <timer.h>

#include "adjacency_list/find_scc.h"
#include "adjacency_list/Euler_path.h"

void graphAddUndirectedEdge(GraphPtr graph, VertexId source, VertexId target, EdgeData data) {
    graphAddEdge(graph, source, target, data);
    graphAddEdge(graph, target, source, data);
}

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
    GraphPtr graph = newGraph(20, 5);
    graphAddUndirectedEdge(graph, 0, 1, NO_EDGE_DATA);
    graphAddUndirectedEdge(graph, 1, 2, NO_EDGE_DATA);
    graphAddUndirectedEdge(graph, 2, 3, NO_EDGE_DATA);
    graphAddUndirectedEdge(graph, 3, 4, NO_EDGE_DATA);
    graphAddUndirectedEdge(graph, 4, 0, NO_EDGE_DATA);
    graphAddUndirectedEdge(graph, 1, 3, NO_EDGE_DATA);
    graphAddUndirectedEdge(graph, 2, 0, NO_EDGE_DATA);
    graphAddUndirectedEdge(graph, 0, 3, NO_EDGE_DATA);
    NodePtr path = newNode(0);
    for (int i = 0; i < 20; i++) {
        TEST_TIME(EulerPath(graph, path, 2, 1));
        nodeClear(&path->next);
    }
    printf("stack\n");
    for (int i = 0; i < 20; i++) {
        TEST_TIME(EulerPath_stack(graph, path, 2, 1));
        nodeClear(&path->next);
    }
    return 0;
}