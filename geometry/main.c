#include "tests/tests.h"
#include "geometry.h"

int main(){
    initTest();

    for(int i = 0; i < 50; i++)
        testConvexHull(9999);
//    testMinEnclosingCircle(100000);

    return 0;
}