#ifndef EDGE_LIST_H
#define EDGE_LIST_H

#include "graph.h"

extern inline void edgeInsert(EdgePtr *next, EdgePtr edge);

extern inline EdgePtr edgeUnlink(EdgePtr *next);

EdgePtr edgeUnlinkWithTarget(EdgePtr *next, VertexId target);

#endif //EDGE_LIST_H
