#include "super_tree//OBST.h"
#include "utils/queue.h"
#include <stdlib.h>

BSTPtr optimalBST(const DataType data[], const WeightType weight[], int number) {
    int left, root, right, treeWidth;
    WeightType minTreeWeight;
    WeightType (*treeWeight)[number] = malloc(number * number * sizeof(WeightType));
    int (*treeRoot)[number] = malloc(number * number * sizeof(BinaryTreeNodePtr));

    for (root = 0; root < number; root++) {
        treeWeight[root][root] = weight[root];
        treeRoot[root][root] = root;
    }

    for (treeWidth = 1; treeWidth < number; treeWidth++) {
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

    BSTPtr tree = createBinaryTree_fixedCapacity(number);
    QueuePtr pLeftQueue = CreateQueue(number);
    QueuePtr pRightQueue = CreateQueue(number);
    BinaryTreeNodePtr rootNode;

    Enqueue(pLeftQueue, 0);
    Enqueue(pRightQueue, number - 1);

    tree->root = tree->memoryPool + treeRoot[0][number - 1];
    tree->nodeNum = number;

    while (pLeftQueue->front != pLeftQueue->rear) {
        left = Dequeue(pLeftQueue);
        right = Dequeue(pRightQueue);
        root = treeRoot[left][right];
        rootNode = tree->memoryPool + root;

        rootNode->isEmpty = 0;
        rootNode->data = data[root];
        rootNode->next = NULL;

        if (root > left) {
            Enqueue(pLeftQueue, left);
            Enqueue(pRightQueue, root - 1);
            left = treeRoot[left][root - 1];
            rootNode->left = tree->memoryPool + left;
        }else
            rootNode->left = NULL;

        if (root < right) {
            Enqueue(pLeftQueue, root + 1);
            Enqueue(pRightQueue, right);
            right = treeRoot[root + 1][right];
            rootNode->right = tree->memoryPool + right;
        }else
            rootNode->right = NULL;
    }

    tree->nodeNum = number;

    free(treeRoot);
    DeleteQueue(pLeftQueue);
    DeleteQueue(pRightQueue);

    return  tree;
}

