#ifndef NETWORKFLOW_H
#define NETWORKFLOW_H

#ifndef INFINITY
#define INFINITY 0x7fffffff
#endif

#ifndef INITIAL_VERTICES_NUMBER
#define INITIAL_VERTICES_NUMBER 32
#endif

typedef int FlowType;
typedef int Vertex;

typedef struct {
    FlowType capacity;
    FlowType flow;
}Edge, *EdgePtr;

typedef struct  {
    int capacity;
    int vertexNum;
    Edge **edges;
}Network, *NetworkPtr;

NetworkPtr CreateNetwork(int vertexNum);

void DeleteNetwork(NetworkPtr pNetwork);

void AddEdge(NetworkPtr pNetwork, Vertex source, Vertex target, FlowType capacity);

NetworkPtr CopyNetwork(NetworkPtr network);

FlowType EdmondKarpMaxFlow(NetworkPtr network, Vertex source, Vertex sink);

#endif //NETWORKFLOW_H
