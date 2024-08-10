#ifndef GRAPH_WEIGHT_PATH_H
#define GRAPH_WEIGHT_PATH_H

#include "graph.h"

void DijkstraWeightedPath(GraphPtr graph, VertexId source, VertexId target, VertexId *parent);

void weightedPath(GraphPtr graph, VertexId source, VertexId *parent);

#endif //GRAPH_WEIGHT_PATH_H
