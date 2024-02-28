#include "DisjSet.h"
#include <stdlib.h>

DisjSet CreateDisjSet(int len) {
    return calloc(len, 4);
}

SetId SetFind(DisjSet set, SetElement element) {
    SetId root;
    for (root = element; set[root] > 0; root = set[root]);
    set[element] = root;
    return root;
}

void UnionSet(DisjSet set, SetId root1, SetId root2) {
    if (set[root1] > set[root2])
        set[root1] = root2;
    else {
        if (set[root1] == set[root2])
            set[root1]--;
        set[root2] = root1;
    }
}

void DeleteSet(DisjSet set) {
    free(set);
    set = NULL;
}