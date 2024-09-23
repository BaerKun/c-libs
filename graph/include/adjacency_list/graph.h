#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "share/vertex_edge.h"

typedef int VertexId;
typedef struct Vertex Vertex, *VertexPtr;
typedef struct Edge Edge, *EdgePtr;
typedef struct Graph Graph, *GraphPtr;

struct Vertex {
    VertexData data;
    int indegree;
    VertexId path;
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
};

GraphPtr newGraph(int capacity, int vertexNum);

void graphDestroy(GraphPtr graph);

void graphAddEdge(GraphPtr graph, VertexId source, VertexId target, EdgeData data);

int graphHasPath(const VertexId parent[], int vertexNum, VertexId source, VertexId target);

#endif //GRAPH_GRAPH_H
