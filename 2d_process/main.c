#include "graphical_debug.h"
#include "convex_hull.h"
#include "tools.h"

void convexHullTest(int npoint) {
    Point2f points[npoint];
    Point2f hullPoints[npoint];
    int nHullPoints;
    Window window = getNewWindow("Convex Hull", 1000, 800);

    for (int i = 0; i < npoint; ++i) {
        points[i].x = randFloat(-450.f, 450.f);
        points[i].y = randFloat(-350.f, 350.f);
    }
    convexHull(points, hullPoints, npoint, &nHullPoints);

    hullPoints[nHullPoints] = hullPoints[0];

    drawCircle(window, points, npoint, 2, -1);
    drawPoly(window, hullPoints, nHullPoints + 1, 1);

    showWindow(window);

    destroyWindow(window);
}

int main() {
    return 0;
}