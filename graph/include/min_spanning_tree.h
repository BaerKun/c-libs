#ifndef GRAPH_MIN_SPANNING_TREE_H
#define GRAPH_MIN_SPANNING_TREE_H

#include "graph.h"

void PrimMinSpanningTree(GraphPtr pGraph, Vertex root, Vertex *parent);

void PrintTree(GraphPtr pGraph, Vertex *parent, Vertex root);

#endif //GRAPH_MIN_SPANNING_TREE_H
