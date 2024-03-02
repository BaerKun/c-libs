/*
 * GraphPtr 类型包括 顶点数 和 邻接表数组；
 * 邻接表的表头存储了 路径 和 距离 信息；
 * BuildPath 会修改这些数据；
 * 无效的终点输入， 或 起点->终点的路径 不存在， BuildPath 会遍历所有可到达的顶点；
 * */

#ifndef GRAPH_H
#define GRAPH_H

#ifndef INFINITY
#define INFINITY 0x7fffffff
#endif

#ifndef INITIAL_NODES_NUMBER
#define INITIAL_NODES_NUMBER 32
#endif

typedef int VertexId;
typedef int WeightType;
typedef struct AdjacencyList AdjacencyList, *AdjacencyListPtr;
typedef struct Vertex {
    WeightType distance;
    int inDegree;
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
    Vertex *vertexes;
};

GraphPtr CreateGraph(int vertexNum);

void Connect(GraphPtr pGraph, VertexId startId, VertexId endId, int weight, int isDirected);

void DeleteGraph(GraphPtr pGraph);

void TopSort(GraphPtr pGraph, VertexId copyArray[]);

void BuildUnweightedPath(GraphPtr pGraph, VertexId startId, VertexId endId);

void BuildNonnegWeightedPath(GraphPtr pGraph, VertexId startId, VertexId endId);

void BuildWeightedPath(GraphPtr pGraph, VertexId startId);

void BuildTopPath(GraphPtr pGraph);

void CopyPath(GraphPtr pGraph, VertexId CopyArray[], VertexId startId, VertexId endId);

int HasPath(GraphPtr pGraph, VertexId startId, VertexId endId);

int GetDistance(GraphPtr pGraph, VertexId vertexId);

#endif //GRAPH_H
