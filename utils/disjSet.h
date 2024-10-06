#ifndef DISJSET_H
#define DISJSET_H

typedef int DisjSet[];
typedef int ClassId;
typedef int ElementType;

static ClassId disjSet_find(ElementType set[], ElementType element) {
    ClassId root;

    for (root = element; set[root] >= 0; root = set[root]);
    if (root != element)
        set[element] = root;

    return root;
}

static void disjSet_union(ElementType set[], ClassId root1, ClassId root2) {
    if (set[root1] > set[root2])
        set[root1] = root2;
    else {
        if (set[root1] == set[root2])
            set[root1]--;
        set[root2] = root1;
    }
}

#endif //DISJSET_H
