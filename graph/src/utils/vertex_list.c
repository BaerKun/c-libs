#include "utils/vertex_list.h"
#include <stdlib.h>

void VertexListInsert(VertexListPtr pPreNode, Vertex vertex) {
    VertexListPtr pNewNode = malloc(sizeof(VertexList));
    pNewNode->vertex = vertex;
    pNewNode->next = pPreNode->next;
    pPreNode->next = pNewNode;
}

void MakeListEmpty(VertexListPtr pHead) {
    VertexListPtr pThisNode, pNextNode;

    for(pThisNode = pHead->next; pThisNode; pThisNode = pNextNode){
        pNextNode = pThisNode->next;
        free(pThisNode);
    }
}