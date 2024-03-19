#ifndef GRAPH_MIN_SPANNING_TREE_H
#define GRAPH_MIN_SPANNING_TREE_H

#include "graph.h"

void PrimMinSpanningTree(GraphPtr pGraph, VertexId root, VertexId *parent);

void PrintTree(GraphPtr pGraph, VertexId *parent, VertexId root);

#endif //GRAPH_MIN_SPANNING_TREE_H
