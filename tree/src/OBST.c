#include "tree/OBST.h"
#include "queue.h"
#include <stdlib.h>

#define MAX_COUNT_INPUT_SIZE 64

BSTPtr optimalBST(const DataType data[], const WeightType weight[], const int number) {
    int left, root, right;
    WeightType minTreeWeight;
    WeightType (*treeWeight)[MAX_COUNT_INPUT_SIZE] = malloc(number * MAX_COUNT_INPUT_SIZE * sizeof(WeightType));
    int (*treeRoot)[MAX_COUNT_INPUT_SIZE] = malloc(number * MAX_COUNT_INPUT_SIZE * sizeof(BinaryTreeNodePtr));

    for (root = 0; root < number; root++) {
        treeWeight[root][root] = weight[root];
        treeRoot[root][root] = root;
    }

    for (int treeWidth = 1; treeWidth < number; treeWidth++) {
        for (left = 0; (right = treeWidth + left) < number; left++) {

            treeWeight[right][left] = treeWeight[left][left] + treeWeight[right][left + 1];

            if (treeWeight[left + 1][right] < treeWeight[left][right - 1]) {
                minTreeWeight = treeWeight[left + 1][right];
                treeRoot[left][right] = left;
            } else {
                minTreeWeight = treeWeight[left][right - 1];
                treeRoot[left][right] = right;
            }

            for (root = left + 1; root < right; root++) {
                if (treeWeight[left][root - 1] + treeWeight[root + 1][right] < minTreeWeight) {
                    minTreeWeight = treeWeight[left][root - 1] + treeWeight[root + 1][right];
                    treeRoot[left][right] = root;
                }
            }

            treeWeight[left][right] = minTreeWeight + treeWeight[right][left];
        }
    }

    free(treeWeight);

    const BSTPtr tree = newBinaryTree_fixedCapacity(number);
    const QueuePtr pLeftQueue = newQueue(number);
    const QueuePtr pRightQueue = newQueue(number);

    enqueue(pLeftQueue, 0);
    enqueue(pRightQueue, number - 1);

    tree->root = tree->memoryPool + treeRoot[0][number - 1];
    tree->nodeNum = number;

    while (pLeftQueue->front != pLeftQueue->rear) {
        left = dequeue(pLeftQueue);
        right = dequeue(pRightQueue);
        root = treeRoot[left][right];
        const BinaryTreeNodePtr rootNode = tree->memoryPool + root;

        rootNode->isEmpty = 0;
        rootNode->data = data[root];
        rootNode->next = NULL;

        if (root > left) {
            enqueue(pLeftQueue, left);
            enqueue(pRightQueue, root - 1);
            left = treeRoot[left][root - 1];
            rootNode->left = tree->memoryPool + left;
        }else
            rootNode->left = NULL;

        if (root < right) {
            enqueue(pLeftQueue, root + 1);
            enqueue(pRightQueue, right);
            right = treeRoot[root + 1][right];
            rootNode->right = tree->memoryPool + right;
        }else
            rootNode->right = NULL;
    }

    tree->nodeNum = number;

    free(treeRoot);
    queue_destroy(pLeftQueue);
    queue_destroy(pRightQueue);

    return  tree;
}

