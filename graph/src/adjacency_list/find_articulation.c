#include "adjacency_list/find_articulation.h"
#include <string.h>

typedef struct {
    char *hasVisited;
    GraphPtr pGraph;
    VertexId *parent;
    int *preNumber;
    int *lowNumber;
    int counter;
    VertexId *outputArray;
}Package;

static void findArticulation(Package *pPackage, VertexId vertex) {
    VertexId adjacentVertex;

    pPackage->hasVisited[vertex] = 1;
    pPackage->lowNumber[vertex] = pPackage->preNumber[vertex] = pPackage->counter++;
    for(EdgePtr pEdge = pPackage->pGraph->vertices[vertex].pOutEdge; pEdge; pEdge = pEdge->next){
        adjacentVertex = pEdge->target;
        if(!pPackage->hasVisited[adjacentVertex]){
            pPackage->parent[adjacentVertex] = vertex;
            findArticulation(pPackage, adjacentVertex);
            if(pPackage->lowNumber[adjacentVertex] >= pPackage->preNumber[vertex]){
                *pPackage->outputArray = vertex;
                pPackage->outputArray++;
            }
            if(pPackage->lowNumber[adjacentVertex] < pPackage->lowNumber[vertex])
                pPackage->lowNumber[vertex] = pPackage->lowNumber[adjacentVertex];
        }else if(pPackage->parent[vertex] != adjacentVertex && pPackage->preNumber[adjacentVertex] < pPackage->lowNumber[vertex]){
            pPackage->lowNumber[vertex] = pPackage->preNumber[adjacentVertex];
        }
    }
}

void FindArticulation(GraphPtr pGraph, VertexId *outputArray) {
    int preNumber[pGraph->vertexNum];
    int lowNumber[pGraph->vertexNum];
    char hasVisited[pGraph->vertexNum];
    VertexId parent[pGraph->vertexNum];
    memset(hasVisited, 0, pGraph->vertexNum);
    Package package = {hasVisited, pGraph, parent, preNumber, lowNumber, 0, outputArray};
    findArticulation(&package, 0);
    int counter = 0;
    for(EdgePtr pEdge = pGraph->vertices[0].pOutEdge; pEdge; pEdge = pEdge->next){
        if(!parent[pEdge->target] && counter++)
            break;
    }
    if(counter == 1)
        package.outputArray[-1] = INFINITY;
}