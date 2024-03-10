#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#ifndef INFINITY
#define INFINITY 0xefff
#endif

#include "utils/vertex_edge.h"

typedef int Vertex;
typedef int WeightType;
typedef struct AdjacencyList Edge, *EdgePtr;
typedef struct Graph Graph, *GraphPtr;
typedef struct EdgeData EdgeData;
typedef struct VertexData VertexData;
struct AdjacencyList {
    Vertex target;
    EdgeData data;
    EdgePtr next;
};
struct Graph {
    int capacity;
    int vertexNum;
    EdgePtr *edges;
    int *indegree;
    Vertex *parent;
    VertexData *vertexData;
};

GraphPtr CreateGraph(int capacity, int vertexNum);

void DeleteGraph(GraphPtr pGraph);

void AddEdge(GraphPtr pGraph, Vertex source, Vertex target, EdgeData data);

int HasPath(const Vertex *parent, int vertexNum, Vertex source, Vertex target);

void UseIndegree(GraphPtr pGraph);

void UseParent(GraphPtr pGraph);

#endif //GRAPH_GRAPH_H
