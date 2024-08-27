#include "adjacency_list/find_articulation.h"
#include <stdlib.h>

typedef struct {
    char *hasVisited;
    GraphPtr graph;
    VertexId *parent;
    int *preorder; // dfs中第一次访问节点的序数
    int *lowest; // 子树所那到达的最低节点的序数
    int counter;
    VertexId *outputArray;
} Package;

static void findArticulationHelper(Package *package, VertexId vertex) {
    VertexId adjacentVertex;

    package->hasVisited[vertex] = 1;
    package->lowest[vertex] = package->preorder[vertex] = package->counter++;
    for (EdgePtr edge = package->graph->vertices[vertex].outEdges; edge; edge = edge->next) {
        adjacentVertex = edge->target;

        if (!package->hasVisited[adjacentVertex]) {
            package->parent[adjacentVertex] = vertex;
            findArticulationHelper(package, adjacentVertex);

            if (package->lowest[adjacentVertex] >= package->preorder[vertex]) {
                *package->outputArray = vertex;
                package->outputArray++;
            }

            if (package->lowest[adjacentVertex] < package->lowest[vertex])
                package->lowest[vertex] = package->lowest[adjacentVertex];

        } else if (package->parent[vertex] != adjacentVertex &&
                   package->preorder[adjacentVertex] < package->lowest[vertex]) {
            package->lowest[vertex] = package->preorder[adjacentVertex];
        }
    }
}

void graphFindArticulation(GraphPtr graph, VertexId outputArray[]) {
    int *pool = malloc(graph->vertexNum * 3 * sizeof(int));
    char *hasVisited = calloc(graph->vertexNum, sizeof(char));

    Package package = {hasVisited, graph, pool, pool + graph->vertexNum,
                       pool + 2 * graph->vertexNum, 0, outputArray};
    findArticulationHelper(&package, 0);

    int counter = 0;
    for (EdgePtr edge = graph->vertices[0].outEdges; edge; edge = edge->next) {
        if (!pool[edge->target] && counter++) // pool == parent
            break;
    }

    if (counter == 1)
        package.outputArray[-1] = -1;

    free(pool);
    free(hasVisited);
}