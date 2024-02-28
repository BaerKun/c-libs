#include <stdio.h>

int calculate24(int array[], int len) {
    if (--len == 0 && array[0] == 24)
        return 1;
    int i, j, numi, numj;
    for (i = 0; i <= len; i++) {
        numi = array[i];
        array[i] = array[len];
        for (j = 0; j < len; j++) {
            numj = array[j];
            array[j] = numi + numj;
            if (calculate24(array, len)) {
                printf("%d + %d = %d\n", numi, numj, array[j]);
                array[j] = numj;
                array[i] = numi;
                return 1;
            }
            if(numi > numj) {
                array[j] = numi - numj;
                if (calculate24(array, len)) {
                    printf("%d - %d = %d\n", numi, numj, array[j]);
                    array[j] = numj;
                    array[i] = numi;
                    return 1;
                }
            }
            array[j] = numi * numj;
            if (calculate24(array, len)) {
                printf("%d * %d = %d\n", numi, numj, array[j]);
                array[j] = numj;
                array[i] = numi;
                return 1;
            }
            if (numi > numj && numj != 0 && numi % numj == 0) {
                array[j] = numi / numj;
                if (calculate24(array, len)) {
                    printf("%d / %d = %d\n", numi, numj, array[j]);
                    array[j] = numj;
                    array[i] = numi;
                    return 1;
                }
            }
            array[j] = numj;
        }
        array[i] = numi;
    }
    return 0;
}

void Calculate24(int array[], int len) {
    if(!calculate24(array, len))
        fputs("Calculate24:NoSolution\n", stderr);
}
int main() {
    int n = 10;
    int i, array[n];
    for (i = 0; i < n; i++)
        scanf("%d", array + i);
    Calculate24(array, n);
    for(i = 0; i < n; i++)
        printf("%d ", array[i]);
    return 0;
}
