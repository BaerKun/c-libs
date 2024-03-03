#ifndef MINSPANNINGTREE_H
#define MINSPANNINGTREE_H

#ifndef INFINITY
#define INFINITY 0x7fffffff
#endif

#ifndef INITIAL_VERTICES_NUMBER
#define INITIAL_VERTICES_NUMBER 32
#endif

typedef int VertexId;
typedef int WeightType;
typedef struct AdjacencyList AdjacencyList, *AdjacencyListPtr;
typedef struct Vertex {
    WeightType distance;
    VertexId path;
    AdjacencyListPtr pAdjacencyList;
}Vertex, *VertexPtr;
typedef struct Graph Graph, *GraphPtr;
struct AdjacencyList {
    VertexId id;
    WeightType weight;
    AdjacencyListPtr next;
};
struct Graph {
    int capacity;
    int vertexNum;
    Vertex *vertices;
};

GraphPtr CreateGraph(int vertexNum);

void Connect(GraphPtr pGraph, VertexId startId, VertexId endId, int weight);

void DeleteGraph(GraphPtr pGraph);

void PrimMinSpanningTree(GraphPtr pGraph, VertexId root);

#endif //MINSPANNINGTREE_H
