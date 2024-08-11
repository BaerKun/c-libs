#ifndef GRAPH_EULER_PATH_H
#define GRAPH_EULER_PATH_H

#include "graph.h"
#include "list.h"

void EulerCircuit(GraphPtr graph, ListPtr path, VertexId source);

void EulerPath(GraphPtr graph, ListPtr path, VertexId source, VertexId target);

#endif //GRAPH_EULER_PATH_H
