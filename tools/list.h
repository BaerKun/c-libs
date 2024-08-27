#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

# ifndef  LIST_ELEMENT_TYPE
# define LIST_ELEMENT_TYPE int
#endif

// 包含头节点
typedef struct Node Node, *NodePtr;
typedef struct Node List, *ListPtr;

struct Node{
    NodePtr next;
    LIST_ELEMENT_TYPE element;
};

static inline ListPtr newList() {
    ListPtr list = malloc(sizeof(struct Node));
    list->next = NULL;
    return list;
}

static inline NodePtr newNode(LIST_ELEMENT_TYPE element) {
    NodePtr node = malloc(sizeof(struct Node));
    node->element = element;

    return node;
}

static void list_makeEmpty(ListPtr list) {
    for (NodePtr next = list->next; next; next = list->next) {
        list->next = next->next;
        free(next);
    }
}

static NodePtr list_find(ListPtr list, LIST_ELEMENT_TYPE element) {
    NodePtr node;
    for (node = list->next; node && node->element != element; node = node->next);
    return node;
}

static NodePtr list_findPrev(ListPtr list, LIST_ELEMENT_TYPE element) {
    NodePtr prev, next;
    for (prev = list, next = prev->next;
         next && next->element != element; next = next->next)
        prev = next;
    if (next)
        return prev;
    return NULL;
}

static inline NodePtr list_deleteNode(ListPtr list, LIST_ELEMENT_TYPE element) {
    NodePtr prev = list_findPrev(list, element);
    if (prev) {
        NodePtr next = prev->next;
        prev->next = next->next;
        return next;
    }
    return NULL;
}

static inline void list_insertNode(ListPtr list, NodePtr node) {
    node->next = list->next;
    list->next = node;
}

static inline void list_insertData(ListPtr list, LIST_ELEMENT_TYPE element) {
    NodePtr newNode = malloc(sizeof(struct Node));
    newNode->element = element;
    list_insertNode(list, newNode);
}

static inline void list_deleteAndFree(ListPtr list, LIST_ELEMENT_TYPE element) {
    NodePtr node = list_deleteNode(list, element);
    free(node);
}

static inline void list_destroy(ListPtr list) {
    list_makeEmpty(list);
    free(list);
}

#endif //LINKEDLIST_H
