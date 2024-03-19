#ifndef GRAPH_TOPSORT_H
#define GRAPH_TOPSORT_H

#include "graph.h"

void Topsort(GraphPtr pGraph, VertexId *sortArray);

void BuildTopPath(GraphPtr pGraph, VertexId *parent);

#endif //GRAPH_TOPSORT_H
