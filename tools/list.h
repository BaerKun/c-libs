#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

# ifndef  LIST_ELEMENT_TYPE
# define LIST_ELEMENT_TYPE int
#endif

// 包含头节点
typedef struct Node *Node;
typedef struct Node *List;

struct Node{
    LIST_ELEMENT_TYPE element;
    Node next;
};

static List newList() {
    List list = malloc(sizeof(struct Node));
    list->next = NULL;
    return list;
}

static void list_makeEmpty(List list) {
    for (Node next = list->next; next; next = list->next) {
        list->next = next->next;
        free(next);
    }
}

static Node list_find(List list, LIST_ELEMENT_TYPE element) {
    Node node;
    for (node = list->next; node && node->element != element; node = node->next);
    return node;
}

static Node list_findPrev(List list, LIST_ELEMENT_TYPE element) {
    Node prev, next;
    for (prev = list, next = prev->next;
         next && next->element != element; next = next->next)
        prev = next;
    if (next)
        return prev;
    return NULL;
}

static Node list_deleteNode(List list, LIST_ELEMENT_TYPE element) {
    Node prev = list_findPrev(list, element);
    if (prev) {
        Node next = prev->next;
        prev->next = next->next;
        return next;
    }
    return NULL;
}

static void list_insertNode(List list, Node node) {
    node->next = list->next;
    list->next = node;
}

static void list_insertData(List list, LIST_ELEMENT_TYPE element) {
    Node _new = malloc(sizeof(struct Node));
    _new->element = element;
    list_insertNode(list, _new);
}

static void list_deleteAndFree(List list, LIST_ELEMENT_TYPE element) {
    Node node = list_deleteNode(list, element);
    free(node);
}

static void list_destroy(List list) {
    list_makeEmpty(list);
    free(list);
}

#endif //LINKEDLIST_H
