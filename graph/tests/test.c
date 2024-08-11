#include <stdio.h>
#include "adjacency_list/Euler_path.h"
#include <time.h>

void AddUndirectedEdge(GraphPtr pGraph, VertexId source, VertexId target) {
    graphAddEdge(pGraph, source, target, (EdgeData){1});
    graphAddEdge(pGraph, target, source, (EdgeData){1});
}

int main() {
    GraphPtr graph = newGraph(16, 7);

    AddUndirectedEdge(graph, 0, 1);
    AddUndirectedEdge(graph, 0, 3);
    AddUndirectedEdge(graph, 1, 2);
    AddUndirectedEdge(graph, 2, 6);
    AddUndirectedEdge(graph, 2, 3);
    AddUndirectedEdge(graph, 3, 4);
    AddUndirectedEdge(graph, 3, 5);
    AddUndirectedEdge(graph, 4, 5);

    List path;
    EulerPath(graph, &path, 2, 6);

    for(ListPtr p = &path; p != NULL; p = p->next){
        printf("%d ", p->element);
    }
    graphDestroy(graph);
    return 0;
}