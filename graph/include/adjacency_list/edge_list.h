#ifndef EDGE_LIST_H
#define EDGE_LIST_H

#include "graph.h"

extern inline void edgeInsert(EdgePtr *prevNextPtr, EdgePtr edge);

extern inline void edgeInsertWithData(EdgePtr *prevNextPtr, VertexId target, EdgeData data);

extern inline EdgePtr edgeUnlink(EdgePtr *prevNextPtr);

extern inline EdgePtr edgeUnlinkWithTarget(EdgePtr *prevNextPtr, VertexId target);

EdgePtr *edgeFind(EdgePtr *prevNextPtr, VertexId target);

void edgeClear(EdgePtr *prevNextPtr);

#endif //EDGE_LIST_H
