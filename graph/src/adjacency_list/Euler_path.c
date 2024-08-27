#include "adjacency_list/Euler_path.h"
#include <stdio.h>

typedef struct {
    EdgePtr *availableEdge;
    ListPtr path;
    VertexId source;
    VertexId previous;
} Argument;
#define STACK_ELEMENT_TYPE Argument
#include "stack.h"

typedef struct {
    EdgePtr *availableEdges;
    VertexId tmpdst;
} Package;

// availableEdges[v] 顶点v的可用边链表，充当递归时的“全局变量”
static EdgePtr *getAvailableEdges(GraphPtr graph) {
    EdgePtr *availableEdges = malloc(graph->vertexNum * sizeof(EdgePtr));
    VertexPtr end = graph->vertices + graph->vertexNum;
    EdgePtr *pedge = availableEdges;

    for (VertexPtr vertex = graph->vertices; vertex != end; ++vertex) {
        *pedge = vertex->outEdges;
        ++pedge;
    }

    return availableEdges;
}

// 获取下一条可用边，并用previous更新availableEdge
static EdgePtr getEdge(EdgePtr *availableEdge, VertexId previous) {
    EdgePtr nextEdge = *availableEdge;
    if (nextEdge == NULL)
        return NULL;

    *availableEdge = nextEdge->next;
    if (nextEdge->target == previous) {
        nextEdge = *availableEdge;
        if (nextEdge != NULL)
            *availableEdge = nextEdge->next;
        return nextEdge;
    }

    EdgePtr this, next;
    for (this = nextEdge; (next = this->next) && next->target != previous; this = next);

    if (next) {
        if (this == nextEdge)
            *availableEdge = next->next;
        else {
            this->next = next->next;
            next->next = *availableEdge;
            nextEdge->next = next;
        }
    }

    return nextEdge;
}

static VertexId getTarget(StackPtr stack) {
    Argument argument = stack_peek(stack);
    EdgePtr nextEdge = getEdge(argument.availableEdge, argument.previous);
    if(nextEdge == NULL){
        stack_pop(stack);
        return -1;
    }
    return nextEdge->target;
}

static int EulerCircuitHelper(Package *package, ListPtr path, VertexId source, VertexId previous) {
    EdgePtr edge;
    while (1) {
        edge = getEdge(package->availableEdges + source, previous);
        if (edge == NULL)
            break;

        list_insertData(path, edge->target);

        if (EulerCircuitHelper(package, path->next, edge->target, source))
            return 1;
    }

    if (source != package->tmpdst)
        return 1;

    package->tmpdst = previous;
    return 0;
}

static void EulerPath_stack(GraphPtr graph, ListPtr path, VertexId src, VertexId dst) {
    StackPtr stack = newStack(graph->edgeNum);
    EdgePtr *availableEdges = getAvailableEdges(graph);
    VertexId previous, target, tmpdst = dst;
    Argument arg;

    path->element = src;
    path->next = NULL;
    stack_push(stack, (Argument){availableEdges + src, path, src, -1});
    do {
        target = getTarget(stack);
        if (target == -1) {
            if (src != tmpdst) {
                puts("EulerCircuit: No Circuit\n");
                break;
            }
            tmpdst = previous;

            // 模拟函数返回
            arg = stack_peek(stack);
            path = arg.path;
            src = arg.source;
            previous = arg.previous;
            continue;
        }
        list_insertData(path, target);

        // 模拟函数调用
        path = path->next;
        previous = src;
        src = target;
        stack_push(stack, (Argument){availableEdges + target, path, src, previous});
    } while (stack->top != 0);

    free(availableEdges);
    stack_destroy(stack);
}

static void EulerPath_recursive(GraphPtr graph, ListPtr path, VertexId src, VertexId dst) {
    EdgePtr *availableEdges = getAvailableEdges(graph);

    path->element = src;
    path->next = NULL;
    Package package = {availableEdges, dst};

    if (EulerCircuitHelper(&package, path, src, -1))
        puts("EulerCircuit: No Circuit\n");

    free(availableEdges);
}

inline void EulerPath(GraphPtr graph, ListPtr path, VertexId src, VertexId dst) {
    EulerPath_recursive(graph, path, src, dst);
}

inline void EulerCircuit(GraphPtr graph, ListPtr path, VertexId source) {
    EulerPath(graph, path, source, source);
}
