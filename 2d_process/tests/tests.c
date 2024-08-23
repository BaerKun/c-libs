#include "graphical_debug.h"
#include "convex_hull.h"
#include "random.h"


void testConvexHull(int npoint) {
    Point2f points[npoint];
    Point2f hullPoints[npoint];

    int nHullPoints;
    Window window = getNewWindow("Convex Hull", 1000, 800);

    for (int i = 0; i < npoint; ++i) {
        points[i].x = randfloat(-450.f, 450.f);
        points[i].y = randfloat(-350.f, 350.f);
    }

    convexHull(points, hullPoints, npoint, &nHullPoints);

    drawCircle(window, points, npoint, 0xff0000, 2, -1);
    drawPoly(window, hullPoints, nHullPoints, 0xff, 1, 1);

    showWindow(window);

    destroyWindow(window);
}

void testConvexHullDiameter(int npts) {
    Point2f pts[npts], maxpts[2];
    Window window = getNewWindow("Convex Hull", 1000, 800);

    pcg32_srandom((uint64_t) testConvexHullDiameter, (uint64_t) testConvexHullDiameter);


    for (int i = 0; i < npts; ++i) {
        pts[i] = (Point2f) {randfloat(-450.0f, 450.0f), randfloat(-350.0f, 350.0f)};
    }

    drawCircle(window, pts, npts, 2, 0xff0000, -1);

    convexHullDiameter(pts, maxpts, npts);

    drawLine(window, maxpts[0], maxpts[1], 0xff, 2);

    showWindow(window);

    destroyWindow(window);
}
