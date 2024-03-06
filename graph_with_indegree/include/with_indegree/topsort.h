#ifndef GRAPH_TOPSORT_H
#define GRAPH_TOPSORT_H

#include "graph.h"

void TopSort(GraphPtr pGraph, VertexId copyArray[]);

void BuildTopPath(GraphPtr pGraph);

#endif //GRAPH_TOPSORT_H
