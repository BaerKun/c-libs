#include <stdio.h>
#include <string.h>

typedef unsigned char Digit;

void LargeNumberMultiply(Digit a[], Digit b[], Digit c[], int la, int lb){
    int i, j, k;
    for(k = la + lb - 2; k >= 0; k--){
        for (i = la - 1; i >= 0; i--) {
            for (j = lb - 1; j >= 0; j--) {
                if(i + j == k) {
                    c[k] += a[i] * b[j];
                    c[k - 1] += c[k] / 10;
                    c[k] %= 10;
                }
            }
        }
    }
}
int main(){
    Digit a[100], b[100], c[101];
    memset(c, 0, sizeof(c));
    Digit digit;
    int la, lb;
    la = lb = 0;
    while((digit = getchar()) != '\n'){
        a[la++] = digit - '0';
    }
    while ((digit = getchar()) != '\n'){
        b[lb++] = digit - '0';
    }
    LargeNumberMultiply(a, b, c, la, lb);
    int end = la + lb - 2;
    int j = end % 3;
    for(int i = 0; i <= end; i++) {
        putchar(c[i] + '0');
        if(i % 3 == j && i != end)
            putchar(',');
    }
    return 0;
}