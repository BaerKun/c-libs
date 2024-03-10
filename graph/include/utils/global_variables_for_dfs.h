#ifndef GRAPH_GLOBAL_VARIABLES_FOR_DFS_H
#define GRAPH_GLOBAL_VARIABLES_FOR_DFS_H

#include "graph.h"

extern char *hasVisitedForDfs;
extern GraphPtr pGraphForDfs;
extern Vertex *parentForDfs;
extern int *preorderNumberForDfs;
extern int *lowNumberForDfs;
extern int counterForDfs;
extern Vertex *outputArrayForDfs;

#endif //GRAPH_GLOBAL_VARIABLES_FOR_DFS_H
