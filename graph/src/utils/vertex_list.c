#include "utils/vertex_list.h"
#include <stdlib.h>

void vertexList_insert(VertexListPtr pPreNode, VertexId vertex) {
    VertexListPtr pNewNode = malloc(sizeof(VertexList));
    pNewNode->vertex = vertex;
    pNewNode->next = pPreNode->next;
    pPreNode->next = pNewNode;
}

void vertexList_MakeEmpty(VertexListPtr pHead) {
    VertexListPtr pThisNode, pNextNode;

    for(pThisNode = pHead->next; pThisNode; pThisNode = pNextNode){
        pNextNode = pThisNode->next;
        free(pThisNode);
    }
}