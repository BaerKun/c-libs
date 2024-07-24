#include "utils/disjiont_set.h"

ClassId Find(ElementType set[], ElementType element) {
    ClassId root;

    for (root = element; set[root] >= 0; root = set[root]);
    if (root != element)
        set[element] = root;

    return root;
}

void Union(ElementType set[], ClassId root1, ClassId root2) {
    if (set[root1] > set[root2])
        set[root1] = root2;
    else {
        if (set[root1] == set[root2])
            set[root1]--;
        set[root2] = root1;
    }
}