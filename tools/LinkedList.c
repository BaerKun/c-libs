#include "LinkedList.h"
#include <stdlib.h>

List CreateList() {
    List list = malloc(sizeof(struct Node));
    list->next = NULL;
    return list;
}

int IsLast(Node node) {
    return node->next == NULL;
}

void MakeListEmpty(List list) {
    Node thisNode, nextNode;
    for (thisNode = list->next; thisNode; thisNode = nextNode) {
        nextNode = thisNode->next;
        free(thisNode);
    }
    list->next = NULL;
}

Node ListFind(List list, ListElement element) {
    Node node;
    for (node = list->next; node && node->element != element; node = node->next);
    return node;
}

Node ListFindPre(List list, ListElement element) {
    Node thisNode, nextNode;
    for (thisNode = list, nextNode = thisNode->next;
         nextNode && nextNode->element != element; nextNode = nextNode->next)
        thisNode = nextNode;
    if (nextNode)
        return thisNode;
    return NULL;
}

Node CutOffNode(List list, ListElement element) {
    Node previousNode, thisNode;
    previousNode = ListFindPre(list, element);
    if (previousNode) {
        thisNode = previousNode->next;
        previousNode->next = thisNode->next;
        return thisNode;
    }
    return NULL;
}

void ListInsert(List list, ListElement element) {
    Node newNode = malloc(sizeof(struct Node));
    newNode->element = element;
    newNode->next = list->next;
    list->next = newNode;
}

void InsertNode(List list, Node node) {
    node->next = list->next;
    list->next = node;
}

void ListDelete(List list, ListElement element) {
    Node node = CutOffNode(list, element);
    free(node);
}

void DeleteList(List list) {
    MakeListEmpty(list);
    free(list);
}