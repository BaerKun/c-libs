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
static EdgePtr *getAvailableEdges(const GraphPtr graph) {
    EdgePtr *availableEdges = malloc(graph->vertexNum * sizeof(EdgePtr));
    const VertexPtr end = graph->vertices + graph->vertexNum;
    EdgePtr *pedge = availableEdges;

    for (VertexPtr vertex = graph->vertices; vertex != end; ++vertex) {
        *pedge = vertex->outEdges;
        ++pedge;
    }

    return availableEdges;
}

// 获取下一条可用边，并用previous更新availableEdge
static EdgePtr getEdge(EdgePtr *availableEdge, const VertexId previous) {
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
    for (this = nextEdge; (next = this->next) != NULL && next->target != previous; this = next);

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

static VertexId getTarget(const StackPtr stack) {
    const Argument argument = stack_peek(stack);
    const EdgePtr nextEdge = getEdge(argument.availableEdge, argument.previous);
    if(nextEdge == NULL){
        stack_pop(stack);
        return -1;
    }
    return nextEdge->target;
}

static int EulerCircuitHelper(Package *package, const ListPtr path, const VertexId source, const VertexId previous) {
    while (1) {
        const EdgePtr edge = getEdge(package->availableEdges + source, previous);
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

static void EulerPath_stack(const GraphPtr graph, ListPtr path, VertexId src, VertexId dst) {
    const StackPtr stack = newStack(graph->edgeNum);
    EdgePtr *availableEdges = getAvailableEdges(graph);
    VertexId previous = -1;

    path->element = src;
    path->next = NULL;
    stack_push(stack, (Argument){availableEdges + src, path, src, -1});
    do {
        const VertexId target = getTarget(stack);
        if (target == -1) {
            if (src != dst) {
                puts("EulerCircuit: No Circuit\n");
                break;
            }
            dst = previous;

            // 模拟函数返回
            const Argument arg = stack_peek(stack);
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

static void EulerPath_recursive(const GraphPtr graph, const ListPtr path, const VertexId src, const VertexId dst) {
    EdgePtr *availableEdges = getAvailableEdges(graph);

    path->element = src;
    path->next = NULL;
    Package package = {availableEdges, dst};

    if (EulerCircuitHelper(&package, path, src, -1))
        puts("EulerCircuit: No Circuit\n");

    free(availableEdges);
}

void EulerPath(const GraphPtr graph, const ListPtr path, const VertexId src, const VertexId dst) {
    EulerPath_recursive(graph, path, src, dst);
}

void EulerCircuit(const GraphPtr graph, const ListPtr path, const VertexId source) {
    EulerPath(graph, path, source, source);
}
