#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

#ifndef LIST_ELEMENT_TYPE
#define LIST_ELEMENT_TYPE int
#endif

#define FOR_EACH_LIST_LIKE(_node, _head) for (const typeof(_head) _node = _head; _node; _node = _node->next)

typedef struct Node_ Node, *NodePtr;
typedef struct List_ List, *ListPtr;

struct Node_ {
    NodePtr next;
    LIST_ELEMENT_TYPE element;
};

struct List_ {
    NodePtr head;
    NodePtr tail;
    unsigned size;
};

static inline ListPtr newList() {
    const ListPtr list = malloc(sizeof(List));
    list->head = list->tail = NULL;
    list->size = 0;
    return list;
}

static inline NodePtr newNode(LIST_ELEMENT_TYPE const element) {
    const NodePtr node = malloc(sizeof(Node));
    node->element = element;
    node->next = NULL;

    return node;
}

static void listClear(const ListPtr list) {
    for (NodePtr prev = NULL, node = list->head; node; ) {
        prev = node;
        node = node->next;
        free(prev);
    }
    list->tail = list->head = NULL;
    list->size = 0;
}

static NodePtr listFind(const ListPtr list, LIST_ELEMENT_TYPE const element) {
    NodePtr node = list->head;
    while (node && node->element != element)
        node = node->next;
    return node;
}

static NodePtr listFindPrev(const ListPtr list, LIST_ELEMENT_TYPE const element) {
    NodePtr prev = NULL;
    for (NodePtr node = list->head; node && node->element != element; node = node->next)
        prev = node;
    return prev;
}

static void insertNode(const NodePtr parent, const NodePtr node) {
    node->next = parent->next;
    parent->next = node;
}

static void insertData(const NodePtr parent, LIST_ELEMENT_TYPE const element) {
    const NodePtr node = malloc(sizeof(Node));
    node->element = element;
    insertNode(parent, node);
}

static void listPush(const ListPtr list, LIST_ELEMENT_TYPE const element) {
    const NodePtr node = malloc(sizeof(Node));
    node->element = element;
    node->next = list->head;
    list->head = node;
    if (++list->size == 1)
        list->tail = node;
}

static LIST_ELEMENT_TYPE listPop(const ListPtr list) {
    if (list->size == 0)
        return (LIST_ELEMENT_TYPE){0};

    const NodePtr node = list->head;
    const LIST_ELEMENT_TYPE element = node->element;
    list->head = node->next;
    free(node);
    if (--list->size == 0)
        list->tail = NULL;
    return element;
}

static void listDelete(const ListPtr list, LIST_ELEMENT_TYPE const element) {
    const NodePtr prev = listFindPrev(list, element);
    if (prev == list->tail)
        return;

    if (prev == NULL) {
        const NodePtr head = list->head;
        list->head = head->next;
        free(head);
        if (--list->size == 0)
            list->tail = NULL;
        return;
    }

    const NodePtr node = prev->next;
    prev->next = node->next;
    if (node == list->tail)
        list->tail = prev;
    free(node);
    list->size--;
}

static inline void listEnqueue(const ListPtr list, LIST_ELEMENT_TYPE const element) {
    const NodePtr node = newNode(element);
    if (++list->size == 1)
        list->head = node;
    else
        list->tail->next = node;

    list->tail = node;
}

static inline LIST_ELEMENT_TYPE listDequeue(const ListPtr list) {
    return listPop(list);
}

static inline void listDestroy(const ListPtr list) {
    listClear(list);
    free(list);
}

#endif //LINKEDLIST_H
