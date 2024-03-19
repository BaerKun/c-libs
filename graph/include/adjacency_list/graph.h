#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#ifndef INFINITY
#define INFINITY 0xefff
#endif

#include "utils/vertex_edge.h"

typedef int VertexId;
typedef struct Vertex Vertex, *VertexPtr;
typedef struct Edge Edge, *EdgePtr;
typedef struct Graph Graph, *GraphPtr;
typedef struct EdgeData EdgeData;
typedef struct VertexData VertexData;
struct Vertex {
    VertexData data;
    EdgePtr pOutEdge;
};
struct Edge {
    VertexId target;
    EdgeData data;
    EdgePtr next;
};
struct Graph {
    int capacity;
    int vertexNum;
    int edgeNum;
    VertexPtr vertices;
    int *indegree;
};

GraphPtr CreateGraph(int capacity, int vertexNum);

void DeleteGraph(GraphPtr pGraph);

void AddEdge(GraphPtr pGraph, VertexId source, VertexId target, EdgeData data);

int HasPath(const VertexId *parent, int vertexNum, VertexId source, VertexId target);

void UseIndegree(GraphPtr pGraph);

void UseParent(GraphPtr pGraph);

#endif //GRAPH_GRAPH_H
