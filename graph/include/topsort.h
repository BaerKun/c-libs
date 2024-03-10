#ifndef GRAPH_TOPSORT_H
#define GRAPH_TOPSORT_H

#include "graph.h"

void Topsort(GraphPtr pGraph, Vertex *sortArray);

void BuildTopPath(GraphPtr pGraph, Vertex *parent);

#endif //GRAPH_TOPSORT_H
