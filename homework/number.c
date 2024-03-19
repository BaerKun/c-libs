#include <stdio.h>

int main(){
    int n, m;
    scanf("%d", &n);
    if(n < 10)
        printf("%d位数", m = 1);
    else if(n < 100)
        printf("%d位数", m = 2);
    else if(n < 1000)
        printf("%d位数", m = 3);
    else if(n < 10000)
        printf("%d位数", m = 4);
    else if(n < 100000)
        printf("%d位数", m = 5);

    printf("\n%d", n % 10);
    n /= 10;

    if(--m > 0){
        printf("%d", n % 10);
        n /= 10;
        if(--m > 0){
            printf("%d", n % 10);
            n /= 10;
            if(--m > 0){
                printf("%d", n % 10);
                n /= 10;
                if(--m > 0){
                    printf("%d", n % 10);
                    n /= 10;
                }
            }
        }
    }
    printf("\n");
    return 0;
}
