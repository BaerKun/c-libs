#include "adjacency_list/find_scc.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    EdgePtr *pEdges;
    VertexId *parent;
    char *hasVisited;
    int *number;
    int thisNumber;
    int vertexNum;
    VertexId root;
}Package;

EdgePtr *CopyEdges(GraphPtr pGraph) {
    EdgePtr *pEdges = malloc(pGraph->vertexNum * sizeof(EdgePtr) + pGraph->edgeNum * sizeof(Edge));
    EdgePtr edges = (EdgePtr)(pEdges + pGraph->vertexNum);
    EdgePtr pOriginalEdge, pThisEdge;
    int couter = 0;
    for(VertexId vertex = 0; vertex < pGraph->vertexNum; vertex++) {
        if(!(pOriginalEdge = pGraph->vertices[vertex].pOutEdge)) {
            pEdges[vertex] = NULL;
            continue;
        }
        pThisEdge = pEdges[vertex] = edges + couter++;
        pEdges[vertex]->target = pOriginalEdge->target;
        pEdges[vertex]->data = pOriginalEdge->data;
        for (pOriginalEdge = pOriginalEdge->next; pOriginalEdge != NULL; pOriginalEdge = pOriginalEdge->next) {
            pThisEdge->next = edges + couter++;
            pThisEdge = pThisEdge->next;
            pThisEdge->target = pOriginalEdge->target;
            pThisEdge->data = pOriginalEdge->data;
        }
        pThisEdge->next = NULL;
    }
    return pEdges;
}

void DeleteEdges(EdgePtr *pEdges) {
    free(pEdges);
}

void findSccGo(Package *pPackage, VertexId thisVertex){
    pPackage->hasVisited[thisVertex] = 1;
    EdgePtr pNextEdge, pEdge = pPackage->pEdges[thisVertex];
    pPackage->pEdges[thisVertex] = NULL;
    for(; pEdge; pEdge = pNextEdge){
        if(!pPackage->hasVisited[pEdge->target])
            findSccGo(pPackage, pEdge->target);
        pNextEdge = pEdge->next;
        pEdge->next = pPackage->pEdges[pEdge->target];
        pPackage->pEdges[pEdge->target] = pEdge;
        pEdge->target = thisVertex;
    }
    pPackage->number[++pPackage->thisNumber] = thisVertex;
    if(thisVertex == pPackage->root){
        while(++pPackage->root < pPackage->vertexNum)
            if(!pPackage->hasVisited[pPackage->root]) {
                findSccGo(pPackage, pPackage->root);
                break;
            }
    }
}

void findSccBack(Package *pPackage, VertexId thisVertex){
    pPackage->hasVisited[thisVertex] = 0;
    for(EdgePtr pEdge = pPackage->pEdges[thisVertex]; pEdge; pEdge = pEdge->next){
        if(!pPackage->hasVisited[pEdge->target])
            continue;
        pPackage->parent[pEdge->target] = thisVertex;
        findSccBack(pPackage, pEdge->target);
    }
    if(thisVertex == pPackage->root){
        pPackage->parent[thisVertex] = thisVertex;
        while(--pPackage->thisNumber >= 0)
            if(pPackage->hasVisited[pPackage->number[pPackage->thisNumber]]) {
                findSccBack(pPackage, pPackage->root = pPackage->number[pPackage->thisNumber]);
                break;
            }
    }
}

void FindScc(GraphPtr pGraph, VertexId *parent) {
    EdgePtr *pEdges = CopyEdges(pGraph);
    int number[pGraph->vertexNum];
    char hasVisited[pGraph->vertexNum];
    memset(hasVisited, 0, pGraph->vertexNum);
    Package package = {pEdges, parent, hasVisited, number, -1, pGraph->vertexNum, 0};
    findSccGo(&package, 0);
    findSccBack(&package, package.root = package.number[package.thisNumber]);
    DeleteEdges(pEdges);
}