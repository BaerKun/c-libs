#include "turnpike_reconstruction.h"
#include "utils/binary_search_tree.h"
#include <stdio.h>

typedef struct {
    BSTPtr pDistTree;
    DistanceType *distances;
    DistanceType *points;
    int numOfPoints;
} Package;

int RT_Delete(BSTPtr pDistTree, DistanceType points[], DistanceType dist, int left, int right, int end) {
    int i;
    for (i = 0; i < left; i++) {
        if (!bstDelete(pDistTree, dist - points[i])) {
            return i;
        }
    }

    for (i = right + 1; i < end; i++) {
        if (!bstDelete(pDistTree, points[i] - dist)) {
            return i;
        }
    }

    return end;
}

void RT_Insert(BSTPtr pDistTree, DistanceType points[], DistanceType dist, int left, int right, int end) {
    int i;
    if (end < left)
        left = end;

    for (i = 0; i < left; i++) {
        bstInsert(pDistTree, NO_DATA_INDEX, dist - points[i]);
    }

    for (i = right + 1; i < end; i++) {
        bstInsert(pDistTree, NO_DATA_INDEX, points[i] - dist);
    }
}

int reconstructTurnpikeBody(Package *package, int left, int right) {
    DistanceType max, isSuccessful = 0;
    int end;

    if(left > right)
        return 1;

    max = package->distances[bstFindMax(package->pDistTree)];

    if (package->numOfPoints ==
        (end = RT_Delete(package->pDistTree, package->points, max, left, right, package->numOfPoints))) {
        package->points[right] = max;
        isSuccessful = reconstructTurnpikeBody(package, left, right - 1);
    }

    if (!isSuccessful) {
        RT_Insert(package->pDistTree, package->points, max, left, right, end);

        if (package->numOfPoints ==
            (end = RT_Delete(package->pDistTree, package->points, max=package->points[package->numOfPoints - 1] - max,
                             left, right, package->numOfPoints))){
            package->points[left] = max;
            isSuccessful = reconstructTurnpikeBody(package, left + 1, right);
        }

        if(!isSuccessful) {
            RT_Insert(package->pDistTree, package->points, max, left, right, end);
        }
    }

    return isSuccessful;
}

void reconstructTurnpike(DistanceType distances[], DistanceType points[], int numOfPoints) {
    int numOfDistances = numOfPoints * (numOfPoints - 1) / 2;
    BSTPtr pTree = buildBST(distances, numOfDistances, numOfDistances, 0);
    Package package = (Package) {pTree, distances, points, numOfPoints};

    points[0] = 0;
    points[numOfPoints - 1] = distances[bstDeleteMax(pTree)];
    points[numOfPoints - 2] = distances[bstDeleteMax(pTree)];

    if (bstDelete(pTree, points[numOfPoints - 1] - points[numOfPoints - 2])) {
        if (reconstructTurnpikeBody(&package, 1, numOfPoints - 3)) {
            puts("reconstruct successfully!");
            goto END;
        }
    }
    puts("reconstruct failed!");
    END:
    DeleteBinaryTree(pTree);
}