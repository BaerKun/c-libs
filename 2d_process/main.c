#include "graphical_debug.h"
#include "convex_hull.h"
#include "tools.h"
#include "random.h"
#include <mach/mach_time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tests/tests.h"

int main() {
    testConvexHull(100);
    return 0;
}