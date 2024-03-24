#include "cursor/OBST.h"
#include "utils/queue.h"
#include "cursor/macro.h"
#include <stdlib.h>


void OptimalBST(BSTPtr pTree, const DataType data[], const WeightType weight[], int number) {
    int left, root, right, treeWidth;
    WeightType minTreeWeight;
    WeightType (*treeWeight)[number] = malloc(number * number * sizeof(WeightType));
    TreeNodeId (*treeRoot)[number] = malloc(number * number * sizeof(TreeNodeId));

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

    QueuePtr pLeftQueue = CreateQueue(number);
    QueuePtr pRightQueue = CreateQueue(number);

    Enqueue(pLeftQueue, 0);
    Enqueue(pRightQueue, number - 1);

    pTree->root = treeRoot[0][number - 1];
    pTree->nodeNum = number;

    while (pLeftQueue->front != pLeftQueue->rear) {
        left = Dequeue(pLeftQueue);
        right = Dequeue(pRightQueue);
        root = treeRoot[left][right];

        DATAIDX(root) = createData(pTree, NO_DATA_INDEX, data[root]);

        if (root != left) {
            Enqueue(pLeftQueue, left);
            Enqueue(pRightQueue, root - 1);
            left = treeRoot[left][root - 1];
            LEFT(root) = left;
        }

        if (root != right) {
            Enqueue(pLeftQueue, root + 1);
            Enqueue(pRightQueue, right);
            right = treeRoot[root + 1][right];
            RIGHT(root) = right;
        }
    }

    pTree->nextAvailableNodeId = number;
    pTree->nextAvailableDataIdx = number;

    free(treeRoot);
    DeleteQueue(pLeftQueue);
    DeleteQueue(pRightQueue);
}

