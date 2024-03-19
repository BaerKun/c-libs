#ifndef GRAPH_WEIGHT_PATH_H
#define GRAPH_WEIGHT_PATH_H

#include "graph.h"

void DijkstraWeightedPath(GraphPtr pGraph, VertexId source, VertexId target, VertexId *parent);

void WeightedPath(GraphPtr pGraph, VertexId source, VertexId *parent);

#endif //GRAPH_WEIGHT_PATH_H
