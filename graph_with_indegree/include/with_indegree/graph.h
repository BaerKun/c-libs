/*
 * GraphPtr 类型包括 顶点数 和 邻接表数组；
 * 邻接表的表头存储了 路径 和 距离 信息；
 * BuildPath 会修改这些数据；
 * 无效的终点输入， 或 起点->终点的路径 不存在， BuildPath 会遍历所有可到达的顶点；
 * */

#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#ifndef INFINITY
#define INFINITY 0x7fffffff
#endif

#ifndef INITIAL_VERTICES_NUMBER
#define INITIAL_VERTICES_NUMBER 32
#endif

typedef int VertexId;
typedef int WeightType;
typedef struct AdjacencyList Edge, *EdgePtr;
typedef struct {
    WeightType distance;
    int inDegree;
    VertexId path;
    EdgePtr pEdge;
}Vertex, *VertexPtr;
typedef struct {
    int capacity;
    int vertexNum;
    Vertex *vertices;
}Graph, *GraphPtr;
struct AdjacencyList {
    VertexId id;
    WeightType weight;
    EdgePtr next;
};

GraphPtr CreateGraph(int vertexNum);

void InitGraph(GraphPtr pGraph);

void AddEdge(GraphPtr pGraph, VertexId startId, VertexId endId, int weight, int isDirected);

void DeleteGraph(GraphPtr pGraph);

void CopyPath(GraphPtr pGraph, VertexId CopyArray[], VertexId startId, VertexId endId);

int HasPath(GraphPtr pGraph, VertexId startId, VertexId endId);

int GetDistance(GraphPtr pGraph, VertexId vertexId);

#endif //GRAPH_GRAPH_H
