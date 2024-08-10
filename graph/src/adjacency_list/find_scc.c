#include "adjacency_list/find_scc.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    EdgePtr *edges;
    VertexId *parent;
    char *hasVisited;
    int *number;
    int thisNumber;
    int vertexNum;
    VertexId root;
}Package;

EdgePtr *CopyEdges(GraphPtr graph) {
    EdgePtr *edges = malloc(graph->vertexNum * sizeof(EdgePtr) + graph->edgeNum * sizeof(Edge));
    EdgePtr edgePool = (EdgePtr)(edges + graph->vertexNum);
    EdgePtr prevEdge, thisEdge;
    int couter = 0;

    for(VertexId vertex = 0; vertex < graph->vertexNum; vertex++) {
        if(!(prevEdge = graph->vertices[vertex].outEdges)) {
            edges[vertex] = NULL;
            continue;
        }

        thisEdge = edges[vertex] = edgePool + couter++;
        edges[vertex]->target = prevEdge->target;
        edges[vertex]->data = prevEdge->data;

        for (prevEdge = prevEdge->next; prevEdge != NULL; prevEdge = prevEdge->next) {
            thisEdge->next = edgePool + couter++;
            thisEdge = thisEdge->next;
            thisEdge->target = prevEdge->target;
            thisEdge->data = prevEdge->data;
        }
        thisEdge->next = NULL;
    }
    return edges;
}

void findSccGo(Package *package, VertexId thisVertex){
    package->hasVisited[thisVertex] = 1;
    EdgePtr nextEdge, thisEdge = package->edges[thisVertex];
    package->edges[thisVertex] = NULL;

    for(; thisEdge; thisEdge = nextEdge){
        if(!package->hasVisited[thisEdge->target])
            findSccGo(package, thisEdge->target);

        nextEdge = thisEdge->next;
        thisEdge->next = package->edges[thisEdge->target];
        package->edges[thisEdge->target] = thisEdge;
        thisEdge->target = thisVertex;
    }

    package->number[++package->thisNumber] = thisVertex;
    if(thisVertex == package->root){
        while(++package->root < package->vertexNum)
            if(!package->hasVisited[package->root]) {
                findSccGo(package, package->root);
                break;
            }
    }
}

void findSccBack(Package *package, VertexId thisVertex){
    package->hasVisited[thisVertex] = 0;
    for(EdgePtr edge = package->edges[thisVertex]; edge; edge = edge->next){
        if(!package->hasVisited[edge->target])
            continue;
        package->parent[edge->target] = thisVertex;
        findSccBack(package, edge->target);
    }

    if(thisVertex == package->root){
        package->parent[thisVertex] = thisVertex;
        while(--package->thisNumber >= 0)
            if(package->hasVisited[package->number[package->thisNumber]]) {
                findSccBack(package, package->root = package->number[package->thisNumber]);
                break;
            }
    }
}

void graphFindScc(GraphPtr graph, VertexId *parent) {
    EdgePtr *edges = CopyEdges(graph);
    int number[graph->vertexNum];
    char hasVisited[graph->vertexNum];

    memset(hasVisited, 0, graph->vertexNum);
    Package package = {edges, parent, hasVisited, number, -1, graph->vertexNum, 0};

    findSccGo(&package, 0);
    findSccBack(&package, package.root = package.number[package.thisNumber]);

    free(edges);
}