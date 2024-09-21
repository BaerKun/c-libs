#ifndef GRAPH_WEIGHT_PATH_H
#define GRAPH_WEIGHT_PATH_H

#include "graph.h"

void DijkstraWeightedPath(GraphPtr graph, VertexId *parent, VertexId source, VertexId target);

void weightedPath(GraphPtr graph, VertexId *parent, VertexId source);

#endif //GRAPH_WEIGHT_PATH_H
