#include "tests/tests.h"

int main(){
    initTest();

    testConvexHull(100000);
    testPtsMaxDist(100000);
    testMinEnclosingCircle(100000);

    return 0;
}