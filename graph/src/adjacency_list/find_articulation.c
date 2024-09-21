#include "adjacency_list/find_articulation.h"
#include <stdlib.h>

typedef struct {
    char *hasVisited;
    Vertex *vertices;
    VertexId *parent;
    int *preorder; // dfs中第一次访问节点的序数
    int *lowest; // 子树所那到达的最低节点的序数
    int counter;
    VertexId *outptr;
} Package;

static void findArticulationHelper(Package *package, VertexId vertex) {
    VertexId adjacentVertex;

    package->hasVisited[vertex] = 1;
    package->lowest[vertex] = package->preorder[vertex] = package->counter++;
    for (EdgePtr edge = package->vertices[vertex].outEdges; edge; edge = edge->next) {
        adjacentVertex = edge->target;

        if (!package->hasVisited[adjacentVertex]) {
            package->parent[adjacentVertex] = vertex;
            findArticulationHelper(package, adjacentVertex);

            if (package->lowest[adjacentVertex] >= package->preorder[vertex]) {
                *package->outptr = vertex;
                package->outptr++;
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
    int *memory = malloc(graph->vertexNum * 3 * sizeof(int));
    char *hasVisited = calloc(graph->vertexNum, sizeof(char));

    Package package = {hasVisited, graph->vertices, memory, memory + graph->vertexNum,
                       memory + 2 * graph->vertexNum, 0, outputArray};
    findArticulationHelper(&package, 0);

    int counter = 0;
    for (EdgePtr edge = graph->vertices[0].outEdges; edge; edge = edge->next) {
        if (!memory[edge->target] && counter++) // memory == parent
            break;
    }

    if (counter == 1)
        *(package.outptr - 1) = -1;

    free(memory);
    free(hasVisited);
}