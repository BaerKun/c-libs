#ifndef DISJSET_H
#define DISJSET_H

typedef int ClassId;
typedef int ElementType;

void Union(ElementType set[], ClassId root1, ClassId root2);

ClassId Find(ElementType set[], ElementType element);

#endif //DISJSET_H
