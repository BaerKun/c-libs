#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#define MAX_VERTEX 64
#define INFINITY 0x7fffffff
#define NO_VERTEX (-1)
#define NO_EDGE (Edge){INFINITY, NO_VERTEX}
#define INITIAL_SELF_POINTING_EDGE (Edge){0, NO_VERTEX}


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

GraphPtr createGraph(int capacity, int vertexNum);

void deleteGraph(GraphPtr graph);

void addEdge(GraphPtr graph, VertexId source, VertexId target, WeightType weight);

#endif //GRAPH_GRAPH_H
