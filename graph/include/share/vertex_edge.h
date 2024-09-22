#ifndef GRAPH_VERTEX_EDGE_H
#define GRAPH_VERTEX_EDGE_H

#ifndef VERTEX_DATA
#define VERTEX_DATA

struct VertexData {
    int data;
};

#endif

#ifndef EDGE_DATA
#define EDGE_DATA

typedef int WeightType;

struct EdgeData {
    WeightType weight;
};

#ifndef WEIGHT_FOR_WEIGHTED_PATH
#define WEIGHT_FOR_WEIGHTED_PATH weight
#endif

#endif

#endif //GRAPH_VERTEX_EDGE_H
