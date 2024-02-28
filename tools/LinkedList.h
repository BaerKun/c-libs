#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef int ListElement;
typedef struct Node *Node;
typedef struct Node *List;

struct Node{
    ListElement element;
    Node next;
};
List CreateList();
void MakeListEmpty(List);
int IsLast(Node);
Node ListFind(List, ListElement);
Node ListFindPre(List, ListElement);
void ListDelete(List, ListElement);
Node CutOffNode(List, ListElement);
void ListInsert(List, ListElement);
void InsertNode(List, Node);
void DeleteList(List);

#endif //LINKEDLIST_H
