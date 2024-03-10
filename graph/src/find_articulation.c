#include "find_articulation.h"
#include <string.h>
#include "utils/global_variables_for_dfs.h"

static void findArticulation(Vertex vertex) {
    Vertex adjacentVertex;

    hasVisitedForDfs[vertex] = 1;
    lowNumberForDfs[vertex] = preorderNumberForDfs[vertex] = counterForDfs++;
    for(EdgePtr pEdge = pGraphForDfs->edges[vertex]; pEdge; pEdge = pEdge->next){
        adjacentVertex = pEdge->target;
        if(!hasVisitedForDfs[adjacentVertex]){
            parentForDfs[adjacentVertex] = vertex;
            findArticulation(adjacentVertex);
            if(lowNumberForDfs[adjacentVertex] >= preorderNumberForDfs[vertex]){
                *outputArrayForDfs = vertex;
                outputArrayForDfs++;
            }
            if(lowNumberForDfs[adjacentVertex] < lowNumberForDfs[vertex])
                lowNumberForDfs[vertex] = lowNumberForDfs[adjacentVertex];
        }else if(parentForDfs[vertex] != adjacentVertex && preorderNumberForDfs[adjacentVertex] < lowNumberForDfs[vertex]){
            lowNumberForDfs[vertex] = preorderNumberForDfs[adjacentVertex];
        }
    }
}

void FindArticulation(GraphPtr pGraph, Vertex *outputArray) {
    int number[pGraph->vertexNum];
    int lowNum[pGraph->vertexNum];
    char hasVisited[pGraph->vertexNum];
    Vertex parent[pGraph->vertexNum];
    memset(hasVisited, 0, pGraph->vertexNum);
    hasVisitedForDfs = hasVisited;
    pGraphForDfs = pGraph;
    parentForDfs = parent;
    preorderNumberForDfs = number;
    lowNumberForDfs = lowNum;
    outputArrayForDfs = outputArray;
    findArticulation(0);
    counterForDfs = 0;
    for(EdgePtr pEdge = pGraph->edges[0]; pEdge; pEdge = pEdge->next){
        if(!parent[pEdge->target] && counterForDfs++)
            break;
    }
    if(counterForDfs == 1)
        outputArrayForDfs[-1] = INFINITY;
    counterForDfs = 0;
}