#include "matrix/all_pairs_weighted_path.h"
#include <stdlib.h>

void allPairsWeightedPath(GraphPtr graph) {
    WeightType (*distances)[graph->vertexNum] = malloc(sizeof(WeightType) * graph->vertexNum * graph->vertexNum);

    VertexId source, target, middle;

    for (source = 0; source < graph->vertexNum; source++) {
        for (target = 0; target < graph->vertexNum; target++){
            distances[source][target] = graph->edges[source][target].weight;
        }
    }

    for(middle = 0; middle < graph->vertexNum; middle++){
        for(source = 0; source < graph->vertexNum; source++){
            if(distances[source][middle] == INFINITY)
                continue;

            for(target = 0; target < graph->vertexNum; target++){
                if(distances[middle][target] == INFINITY)
                    continue;

                if(distances[source][middle] + distances[middle][target] < distances[source][target]){
                    distances[source][target] = distances[source][middle] + distances[middle][target];

                    if(graph->edges[source][middle].weight != INFINITY){
                        graph->edges[source][target].path = middle;
                    }
                }
            }
        }
    }
    free(distances);
}