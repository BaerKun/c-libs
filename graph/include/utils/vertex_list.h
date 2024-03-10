#ifndef GRAPH_VERTEX_LIST_H
#define GRAPH_VERTEX_LIST_H

#include "graph.h"

typedef struct VertexList VertexList, *VertexListPtr;
struct VertexList {
    Vertex vertex;
    VertexListPtr next;
};

void VertexListInsert(VertexListPtr pPreNode, Vertex vertex);

void MakeListEmpty(VertexListPtr pHead);

#endif //GRAPH_VERTEX_LIST_H
