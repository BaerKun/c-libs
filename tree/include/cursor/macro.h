#ifndef TREE_MACRO_H
#define TREE_MACRO_H

#define NODE(nodeId) pTree->nodes[nodeId]
#define LEFT(nodeId) pTree->nodes[nodeId].left
#define RIGHT(nodeId) pTree->nodes[nodeId].right
#define DATAIDX(nodeId) pTree->nodes[nodeId].dataIdx
#define DATA(dataId) pTree->datas[dataId]
#define NEXT(dataIdx) pTree->nextDataIdx[dataIdx]

#endif //TREE_MACRO_H
