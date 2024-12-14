#ifndef NODE_H
#define NODE_H

#include <stdlib.h>

#ifndef LIST_ELEMENT_TYPE
#define LIST_ELEMENT_TYPE int
#endif

#define FOR_EACH_LIST_LIKE(_node, _head) for (typeof(_head) _node = _head; _node; _node = _node->next)

typedef struct Node_ Node, *NodePtr;

struct Node_ {
    NodePtr next;
    LIST_ELEMENT_TYPE element;
};

static inline NodePtr newNode(LIST_ELEMENT_TYPE const element) {
    const NodePtr node = malloc(sizeof(Node));
    node->element = element;
    node->next = NULL;

    return node;
}

static inline void nodeInsert(NodePtr *const prevNextPtr, LIST_ELEMENT_TYPE const element) {
    const NodePtr node = malloc(sizeof(Node));
    node->element = element;
    node->next = *prevNextPtr;
    *prevNextPtr = node;
}

static inline NodePtr nodeUnlink(NodePtr *const prevNextPtr) {
    const NodePtr node = *prevNextPtr;
    *prevNextPtr = node->next;
    return node;
}

static NodePtr NodeUnlinkWithData(NodePtr *const headNextPtr, LIST_ELEMENT_TYPE const element) {
    for (NodePtr *prev = headNextPtr, node = *prev; node; prev = &node->next, node = *prev) {
        if (node->element == element)
            return nodeUnlink(prev);
    }
    return NULL;
}

static inline void nodeDelete(NodePtr *const prevNextPtr){
    const NodePtr node = nodeUnlink(prevNextPtr);
    if (node != NULL)
        free(node);
}

static void nodeClear(NodePtr *const headNextPtr) {
    for (NodePtr node = *headNextPtr; node; node = *headNextPtr) {
        *headNextPtr = node->next;
        free(node);
    }
}

#endif //NODE_H
