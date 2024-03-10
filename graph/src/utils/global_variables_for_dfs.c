#include "graph.h"

char *hasVisitedForDfs;
GraphPtr pGraphForDfs;
Vertex *parentForDfs;
int *preorderNumberForDfs;
int *lowNumberForDfs;
int counterForDfs;
Vertex *outputArrayForDfs = 0;
// 这个文件必须做点什么，否则没法塞进静态库里