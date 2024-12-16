#include "adjacency_list/edge_list.h"

#include <stddef.h>

inline void edgeInsert(EdgePtr *const next, const EdgePtr edge) {
    edge->next = *next;
    *next = edge;
}

inline EdgePtr edgeUnlink(EdgePtr *const next) {
    const EdgePtr edge = *next;
    *next = edge->next;
    return edge;
}

EdgePtr edgeUnlinkWithTarget(EdgePtr *const next, const VertexId target) {
    for (EdgePtr *prev = next, edge = *prev; edge; prev = &edge->next, edge = *prev) {
        if (edge->target == target) {
            *prev = edge->next;
            return edge;
        }
    }
    return NULL;
}