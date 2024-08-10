#ifndef GRAPH_VERTEX_LIST_H
#define GRAPH_VERTEX_LIST_H

#include "adjacency_list/graph.h"

typedef struct VertexList VertexList, *VertexListPtr;
struct VertexList {
    VertexId vertex;
    VertexListPtr next;
};

void vertexList_insert(VertexListPtr pPreNode, VertexId vertex);

void vertexList_MakeEmpty(VertexListPtr pHead);

#endif //GRAPH_VERTEX_LIST_H
