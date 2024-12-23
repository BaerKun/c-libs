#include <turnpike_reconstruction.h>
#include <stdio.h>

int main() {
    DistanceType distances[] = {2, 5, 3, 1, 6, 3, 2, 5, 8, 7};
    DistanceType points[5];
    reconstructTurnpike(distances, points, 5);

    for (int i = 0; i < 5; ++i) {
        printf("%d ", points[i]);
    }

}