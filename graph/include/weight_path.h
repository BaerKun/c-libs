#ifndef GRAPH_WEIGHT_PATH_H
#define GRAPH_WEIGHT_PATH_H

#include "graph.h"

void DijkstraWeightedPath(GraphPtr pGraph, Vertex source, Vertex target, Vertex *parent);

void WeightedPath(GraphPtr pGraph, Vertex source, Vertex *parent);

#endif //GRAPH_WEIGHT_PATH_H
