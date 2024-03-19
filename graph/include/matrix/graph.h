#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#define MAX_VERTEX 64

typedef int WeightType;
typedef int VertexId;
typedef struct {
    WeightType weight;
    VertexId path;
}Edge, *EdgePtr;
typedef struct {

}Vertex;
typedef struct {
    int capacity;
    int edgeNum;
    int vertexNum;
    Vertex *vertices;
    Edge (*edges)[MAX_VERTEX];
}Graph, *GraphPtr;

#endif //GRAPH_GRAPH_H
