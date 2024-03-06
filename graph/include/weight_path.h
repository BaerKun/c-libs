#ifndef GRAPH_WEIGHT_PATH_H
#define GRAPH_WEIGHT_PATH_H

#include "graph.h"

void BuildNonnegWeightedPath(GraphPtr pGraph, VertexId startId, VertexId endId);

void BuildWeightedPath(GraphPtr pGraph, VertexId startId);


#endif //GRAPH_WEIGHT_PATH_H
