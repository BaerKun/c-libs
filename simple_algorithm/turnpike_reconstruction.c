#include "turnpike_reconstruction.h"
#include "super_tree/binary_search_tree.h"
#include <stdio.h>
#include "stack.h"

typedef struct {
    BSTPtr tree;
    DistanceType *points;
    StackPtr stack;
    int npoints;
} Package;

int RT_Delete(BSTPtr tree, DistanceType points[], DistanceType point, int left, int right, int end, StackPtr stack) {
    int i;
    BinaryTreeNodePtr node;
    for (i = 0; i < left; i++) {
        node = bstDelete_data(tree, point - points[i]);
        if (node == NULL)
            return i;
        stackPush(stack, node - tree->memoryPool);
    }

    for (i = right + 1; i < end; i++) {
        node = bstDelete_data(tree, points[i] - point);
        if (node == NULL)
            return i;
        stackPush(stack, node - tree->memoryPool);
    }

    return end;
}

void RT_Insert(BSTPtr tree, int left, int right, int end, StackPtr stack) {
    int i;

    if (end < left)
        left = end;

    for (i = right + 1; i < end; i++)
        bstInsert_node(tree, tree->memoryPool + stackPop(stack));

    for (i = 0; i < left; i++)
        bstInsert_node(tree, tree->memoryPool + stackPop(stack));

}

int reconstructTurnpikeBody(Package *package, int left, int right) {
    DistanceType max, isSuccessful = 0;
    int end;

    if(left > right)
        return 1;

    max = bstFindMax(package->tree)->data;

    if (package->npoints ==
        (end = RT_Delete(package->tree, package->points, max, left, right, package->npoints, package->stack))) {
        package->points[right] = max;
        isSuccessful = reconstructTurnpikeBody(package, left, right - 1);
    }

    if (!isSuccessful) {
        RT_Insert(package->tree, left, right, end, package->stack);

        if (package->npoints ==
            (end = RT_Delete(package->tree, package->points, max=package->points[package->npoints - 1] - max,
                             left, right, package->npoints, package->stack))){
            package->points[left] = max;
            isSuccessful = reconstructTurnpikeBody(package, left + 1, right);
        }

        if(!isSuccessful) {
            RT_Insert(package->tree, left, right, end, package->stack);
        }
    }

    return isSuccessful;
}

void reconstructTurnpike(DistanceType distances[], DistanceType points[], int npoints) {
    int numOfDistances = npoints * (npoints - 1) / 2;
    BSTPtr tree = buildBST(distances, numOfDistances, 1);
    StackPtr stack = newStack(numOfDistances);
    Package package = (Package) {tree, points, stack, npoints};

    points[0] = 0;
    points[npoints - 1] = bstDeleteMax(tree)->data;
    points[npoints - 2] = bstDeleteMax(tree)->data;

    if (bstDelete_data(tree, points[npoints - 1] - points[npoints - 2]) != NULL) {
        if (reconstructTurnpikeBody(&package, 1, npoints - 3)) {
            puts("reconstruct successfully!");
            goto END;
        }
    }
    puts("reconstruct failed!");
    END:
    deleteBinaryTree(tree);
    deleteStack(stack);
}