#include "adjacency_list/Euler_path.h"
#include <stdio.h>

typedef struct {
    EdgePtr *availableEdge;
    NodePtr path;
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

    for (VertexId v = 0; v < graph->vertexNum; v++)
        availableEdges[v] = graph->vertices[v].outEdges;

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
        stackPop(stack);
        return -1;
    }
    return nextEdge->target;
}

static int EulerCircuitHelper(Package *package, const NodePtr path, const VertexId source, const VertexId previous) {
    while (1) {
        const EdgePtr edge = getEdge(package->availableEdges + source, previous);
        if (edge == NULL)
            break;

        insertData(path, edge->target);

        if (EulerCircuitHelper(package, path->next, edge->target, source))
            return 1;
    }

    if (source != package->tmpdst)
        return 1;

    package->tmpdst = previous;
    return 0;
}

static void EulerPath_stack(const GraphPtr graph, NodePtr path, VertexId src, VertexId dst) {
    const StackPtr stack = newStack(graph->edgeNum);
    EdgePtr *availableEdges = getAvailableEdges(graph);
    VertexId previous = -1;

    insertData(path, src);
    stackPush(stack, (Argument){availableEdges + src, path->next, src, -1});
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
        insertData(path, target);

        // 模拟函数调用
        previous = src;
        src = target;
        stackPush(stack, (Argument){availableEdges + target, path->next, src, previous});
    } while (stack->top != 0);

    free(availableEdges);
    stackDestroy(stack);
}

static void EulerPath_recursive(const GraphPtr graph, const NodePtr path, const VertexId src, const VertexId dst) {
    EdgePtr *availableEdges = getAvailableEdges(graph);

    insertData(path, src);
    Package package = {availableEdges, dst};

    if (EulerCircuitHelper(&package, path, src, -1))
        puts("EulerCircuit: No Circuit\n");

    free(availableEdges);
}

void EulerPath(const GraphPtr graph, const NodePtr path, const VertexId src, const VertexId dst) {
    EulerPath_recursive(graph, path, src, dst);
}

void EulerCircuit(const GraphPtr graph, const NodePtr path, const VertexId source) {
    EulerPath(graph, path, source, source);
}
