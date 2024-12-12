#include "stdio.h"
#include "turnpike_reconstruction.h"
int main() {
    DistanceType distances[10] = {3, 1, 2, 5, 2, 4};
    DistanceType points[5];
    reconstructTurnpike(distances, points, 4);
    for (int i = 0; i < 4; ++i) {
        printf("%d ", points[i]);
    }
}