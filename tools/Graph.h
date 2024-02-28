/*
 * Graph 类型包括 顶点数 和 邻接表数组；
 * 邻接表的表头存储了 路径 和 距离 信息；
 * BuildPath 会修改这些数据， 并返回 起点->终点 的 路径长；
 * 无效的终点输入， 或 起点->终点的路径 不存在， BuildPath 会遍历所有可到达的顶点， 并输出 无穷大（INFINITY）；
 * */

#ifndef GRAPH_H
#define GRAPH_H

#ifndef INFINITY
#define INFINITY 0x7fffffff
#endif

typedef int Vertex;
typedef int ValueType;
typedef struct AdjacencyList *AdjacencyList;
typedef struct Graph *Graph;
struct AdjacencyList {
    Vertex vertex;
    ValueType value;
    AdjacencyList next;
};
struct Graph {
    int vertexNum;
    struct AdjacencyList *adjacencyLists;
};

Graph CreateGraph(int vertexNum);

void Connect(Graph graph, Vertex start, Vertex end, int value, int isDirected);

void DeleteGraph(Graph graph);

void TopSort(Graph graph, Vertex copyArray[]);

void BuildUnweightedPath(Graph graph, Vertex start, Vertex end);

void BuildNonnegWeightedPath(Graph graph, Vertex start, Vertex end);

void BuildWeightedPath(Graph graph, Vertex start);

void CopyPath(Graph graph, Vertex CopyArray[], Vertex start, Vertex end);

int HasPath(Graph graph, Vertex start, Vertex end);

int GetDistance(Graph graph, Vertex vertex);

#endif //GRAPH_H
