#ifndef GRAPH_VERTEX_EDGE_H
#define GRAPH_VERTEX_EDGE_H

#ifndef VERTEX_DATA
#define VERTEX_DATA

struct VertexData {
    int data;
};

#define NO_VERTEX_DATA (VertexData){0x7fffffff}
#endif

#ifndef EDGE_DATA
#define EDGE_DATA

typedef int WeightType;

struct EdgeData {
    WeightType weight;
};

#define NO_EDGE_DATA (EdgeData){0}

typedef struct VertexData VertexData;
typedef struct EdgeData EdgeData;

#ifndef WEIGHT_FOR_WEIGHTED_PATH
#define WEIGHT_FOR_WEIGHTED_PATH weight
#endif

#endif

#endif //GRAPH_VERTEX_EDGE_H
