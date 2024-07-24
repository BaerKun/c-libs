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

static List createList() {
    List list = malloc(sizeof(struct Node));
    list->next = NULL;
    return list;
}

static int isLast(Node node) {
    return node->next == NULL;
}

static void makeListEmpty(List list) {
    for (Node next = list->next; next; next = list->next) {
        list->next = next->next;
        free(next);
    }
}

static Node listFind(List list, LIST_ELEMENT_TYPE element) {
    Node node;
    for (node = list->next; node && node->element != element; node = node->next);
    return node;
}

static Node listFindPrev(List list, LIST_ELEMENT_TYPE element) {
    Node prev, next;
    for (prev = list, next = prev->next;
         next && next->element != element; next = next->next)
        prev = next;
    if (next)
        return prev;
    return NULL;
}

static Node listDeleteNode(List list, LIST_ELEMENT_TYPE element) {
    Node prev = listFindPrev(list, element);
    if (prev) {
        Node next = prev->next;
        prev->next = next->next;
        return next;
    }
    return NULL;
}

static void listInsert_node(List list, Node node) {
    node->next = list->next;
    list->next = node;
}

static void listInsert_data(List list, LIST_ELEMENT_TYPE element) {
    Node _new = malloc(sizeof(struct Node));
    _new->element = element;
    listInsert_node(list, _new);
}

static void listDeleteAndFree(List list, LIST_ELEMENT_TYPE element) {
    Node node = listDeleteNode(list, element);
    free(node);
}

static void deleteList(List list) {
    makeListEmpty(list);
    free(list);
}

#endif //LINKEDLIST_H
