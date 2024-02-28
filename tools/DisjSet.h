#ifndef DISJSET_H
#define DISJSET_H

typedef int *DisjSet;
typedef int SetId;
typedef int SetElement;

DisjSet CreateDisjSet(int len);

void UnionSet(DisjSet set, SetId root1, SetId root2);

SetId SetFind(DisjSet set, SetElement element);

void DeleteSet(DisjSet set);

#endif //DISJSET_H
