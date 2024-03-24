#include "matrix/all_pairs_weighted_path.h"
#include <stdlib.h>

void allPairsWeightedPath(GraphPtr pGraph) {
    WeightType (*distances)[pGraph->vertexNum] = malloc(sizeof(WeightType) * pGraph->vertexNum * pGraph->vertexNum);

    VertexId source, target, center;

    for (source = 0; source < pGraph->vertexNum; source++) {
        for (target = 0; target < pGraph->vertexNum; target++){
            distances[source][target] = pGraph->edges[source][target].weight;
        }
    }

    for(center = 0; center < pGraph->vertexNum; center++){
        for(source = 0; source < pGraph->vertexNum; source++){
            if(distances[source][center] == INFINITY)
                continue;

            for(target = 0; target < pGraph->vertexNum; target++){
                if(distances[center][target] == INFINITY)
                    continue;

                if(distances[source][center] + distances[center][target] < distances[source][target]){
                    distances[source][target] = distances[source][center] + distances[center][target];

                    if(pGraph->edges[source][center].weight != INFINITY){
                        pGraph->edges[source][target].path = center;
                    }
                }
            }
        }
    }

    free(distances);
}