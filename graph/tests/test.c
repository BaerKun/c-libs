#include <stdio.h>
#include "adjacency_list/min_spanning_tree.h"
#include "adjacency_list/weight_path.h"
#include "adjacency_list/find_articulation.h"
#include "adjacency_list/Euler_path.h"

void AddUndirectedEdge(GraphPtr pGraph, VertexId source, VertexId target) {
    AddEdge(pGraph, source, target, (EdgeData){1});
    AddEdge(pGraph, target, source, (EdgeData){1});
}

int main() {
    GraphPtr pGraph = CreateGraph(16, 7);

    AddUndirectedEdge(pGraph, 0, 1);
    AddUndirectedEdge(pGraph, 0, 3);
    AddUndirectedEdge(pGraph, 1, 2);
    AddUndirectedEdge(pGraph, 2, 6);
    AddUndirectedEdge(pGraph, 2, 3);
    AddUndirectedEdge(pGraph, 3, 4);
    AddUndirectedEdge(pGraph, 3, 5);
    AddUndirectedEdge(pGraph, 4, 5);


//    int a, b;
//    for(int i = 0; i < 12; i++){
//        scanf("%d %d", &a, &b);
//        AddUndirectedEdge(pGraph, a, b);
//    }
    VertexList path;
    EulerCircuit(pGraph, &path, 0);

    for(VertexListPtr p = &path; p != NULL; p = p->next){
        printf("%d ", p->vertexId);
    }
    DeleteGraph(pGraph);
    return 0;
}