#include "turnpike_reconstruction.h"
#include "utils/binary_search_tree.h"
#include <stdio.h>
#include "utils/stack.h"

typedef struct {
    BSTPtr pDistTree;
    DistanceType *distances;
    DistanceType *points;
    StackPtr distStack;
    int numOfPoints;
} Package;

int RT_Delete(BSTPtr pDistTree, DistanceType points[], DistanceType point, int left, int right, int end, StackPtr stack) {
    int i;
    DataPtr pDist;
    for (i = 0; i < left; i++) {
        pDist = bstDelete(pDistTree, point - points[i]);
        if (pDist == DATA_NULLPTR)
            return i;
        stackPush(stack, pDist);
    }

    for (i = right + 1; i < end; i++) {
        pDist = bstDelete(pDistTree, points[i] - point);
        if (pDist == DATA_NULLPTR)
            return i;
        stackPush(stack, pDist);
    }

    return end;
}

void RT_Insert(BSTPtr pDistTree, int left, int right, int end, StackPtr stack) {
    int i;

    if (end < left)
        left = end;

    for (i = right + 1; i < end; i++)
        bstInsert_ptr(pDistTree, stackPop(stack));

    for (i = 0; i < left; i++)
        bstInsert_ptr(pDistTree, stackPop(stack));

}

int reconstructTurnpikeBody(Package *package, int left, int right) {
    DistanceType max, isSuccessful = 0;
    int end;

    if(left > right)
        return 1;

    max = package->distances[bstFindMax(package->pDistTree)];

    if (package->numOfPoints ==
        (end = RT_Delete(package->pDistTree, package->points, max, left, right, package->numOfPoints, package->distStack))) {
        package->points[right] = max;
        isSuccessful = reconstructTurnpikeBody(package, left, right - 1);
    }

    if (!isSuccessful) {
        RT_Insert(package->pDistTree, left, right, end, package->distStack);

        if (package->numOfPoints ==
            (end = RT_Delete(package->pDistTree, package->points, max=package->points[package->numOfPoints - 1] - max,
                             left, right, package->numOfPoints, package->distStack))){
            package->points[left] = max;
            isSuccessful = reconstructTurnpikeBody(package, left + 1, right);
        }

        if(!isSuccessful) {
            RT_Insert(package->pDistTree, left, right, end, package->distStack);
        }
    }

    return isSuccessful;
}

void reconstructTurnpike(DistanceType distances[], DistanceType points[], int numOfPoints) {
    int numOfDistances = numOfPoints * (numOfPoints - 1) / 2;
    BSTPtr pTree = buildBST(distances, numOfDistances, numOfDistances, 0);
    StackPtr distStack = newStack(numOfDistances);
    Package package = (Package) {pTree, distances, points, distStack, numOfPoints};

    points[0] = 0;
    points[numOfPoints - 1] = distances[bstDeleteMax(pTree)];
    points[numOfPoints - 2] = distances[bstDeleteMax(pTree)];

    if (bstDelete(pTree, points[numOfPoints - 1] - points[numOfPoints - 2]) != DATA_NULLPTR) {
        if (reconstructTurnpikeBody(&package, 1, numOfPoints - 3)) {
            puts("reconstruct successfully!");
            goto END;
        }
    }
    puts("reconstruct failed!");
    END:
    DeleteBinaryTree(pTree);
    deleteStack(distStack);
}