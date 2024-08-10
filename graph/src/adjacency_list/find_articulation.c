#include "adjacency_list/find_articulation.h"
#include <string.h>

typedef struct {
    char *hasVisited;
    GraphPtr graph;
    VertexId *parent;
    int *preNumber;
    int *lowNumber;
    int counter;
    VertexId *outputArray;
}Package;

static void findArticulationHelper(Package *package, VertexId vertex) {
    VertexId adjacentVertex;

    package->hasVisited[vertex] = 1;
    package->lowNumber[vertex] = package->preNumber[vertex] = package->counter++;
    for(EdgePtr pEdge = package->graph->vertices[vertex].outEdges; pEdge; pEdge = pEdge->next){
        adjacentVertex = pEdge->target;
        if(!package->hasVisited[adjacentVertex]){
            package->parent[adjacentVertex] = vertex;
            findArticulationHelper(package, adjacentVertex);
            if(package->lowNumber[adjacentVertex] >= package->preNumber[vertex]){
                *package->outputArray = vertex;
                package->outputArray++;
            }
            if(package->lowNumber[adjacentVertex] < package->lowNumber[vertex])
                package->lowNumber[vertex] = package->lowNumber[adjacentVertex];
        }else if(package->parent[vertex] != adjacentVertex && package->preNumber[adjacentVertex] < package->lowNumber[vertex]){
            package->lowNumber[vertex] = package->preNumber[adjacentVertex];
        }
    }
}

void graphFindArticulation(GraphPtr graph, VertexId outputArray[]) {
    int preNumber[graph->vertexNum];
    int lowNumber[graph->vertexNum];
    char hasVisited[graph->vertexNum];
    VertexId parent[graph->vertexNum];

    memset(hasVisited, 0, graph->vertexNum);
    Package package = {hasVisited, graph, parent, preNumber, lowNumber, 0, outputArray};
    findArticulationHelper(&package, 0);

    int counter = 0;
    for(EdgePtr edge = graph->vertices[0].outEdges; edge; edge = edge->next){
        if(!parent[edge->target] && counter++)
            break;
    }
    if(counter == 1)
        package.outputArray[-1] = INFINITY;
}