#ifndef GRAPH_VERTEX_LIST_H
#define GRAPH_VERTEX_LIST_H

#include "adjacency_list/graph.h"

typedef struct VertexList VertexList, *VertexListPtr;
struct VertexList {
    VertexId vertexId;
    VertexListPtr next;
};

void VertexListInsert(VertexListPtr pPreNode, VertexId vertex);

void MakeListEmpty(VertexListPtr pHead);

#endif //GRAPH_VERTEX_LIST_H
