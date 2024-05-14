#ifndef TREE_MACRO_H
#define TREE_MACRO_H

#define NODE(nodePtr) pTree->nodes[nodePtr]
#define LEFT(nodePtr) pTree->nodes[nodePtr].left
#define RIGHT(nodePtr) pTree->nodes[nodePtr].right
#define DATAPTR(nodePtr) pTree->nodes[nodePtr].dataPtr
#define DATA(dataPtr) pTree->data[dataPtr]
#define NEXT(dataPtr) pTree->nextDataPtr[dataPtr]

#endif //TREE_MACRO_H
