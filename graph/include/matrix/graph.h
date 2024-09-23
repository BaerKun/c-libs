#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "share/vertex_edge.h"
#define MAX_VERTEX 64
#define WIGHT_MAX 0x7fffffff
#define INIT_EDGE (Edge){0x7fffffff, 0}

typedef int WeightType;
typedef int VertexId;

typedef struct {
    EdgeData data;
    VertexId path;
} Edge, *EdgePtr;

typedef struct {
    VertexData data;
} Vertex;

typedef struct {
    int capacity;
    int edgeNum;
    int vertexNum;
    Vertex *vertices;
    Edge (*edges)[MAX_VERTEX];
} Graph, *GraphPtr;

GraphPtr createGraph(int capacity, int vertexNum);

void deleteGraph(GraphPtr graph);

void addEdge(GraphPtr graph, VertexId source, VertexId target, WeightType weight);

#endif //GRAPH_GRAPH_H
