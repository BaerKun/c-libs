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
    EdgePtr outEdges;
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

GraphPtr newGraph(int capacity, int vertexNum);

void graphDestroy(GraphPtr graph);

void graphAddEdge(GraphPtr graph, VertexId source, VertexId target, EdgeData data);

int graphHasPath(const VertexId *parent, int vertexNum, VertexId source, VertexId target);

void UseIndegree(GraphPtr graph);

void UseParent(GraphPtr graph);

#endif //GRAPH_GRAPH_H
