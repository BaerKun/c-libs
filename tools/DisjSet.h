#ifndef DISJSET_H
#define DISJSET_H

typedef int DisjSet[];
typedef int ClassId;
typedef int ElementType;

void Union(DisjSet set, ClassId root1, ClassId root2);

ClassId Find(DisjSet set, ElementType element);

#endif //DISJSET_H
