#include "graphical.h"
#include "point_set.h"
#include "random.h"
#include "timer.h"
#include "geom_utils.h"
#include <stdlib.h>

void initTest() {
    const int64_t seed = time(0) ^ (int64_t) initTest;
    pcg32_srandom(seed, seed << 1 | 1);
    graphicalInit();
}

void randomRectPoints(Point2f *pts, const int npts, const float width, const float height) {
    const float halfWidth = width / 2.0f;
    const float halfHeight = height / 2.0f;
    for (int i = 0; i < npts; ++i)
        pts[i] = (Point2f) {randfloat(-halfWidth, halfWidth), randfloat(-halfHeight, halfHeight)};
}

void randomCirclePoints(Point2f *pts, const int npts, const float radius) {
    for (int i = 0; i < npts; ++i)
        pts[i] = (Point2f) {randfloat(0.f, 2.f * (float) M_PI), randfloat(0.f, radius)};
    polar2cartesian_pt(pts, pts, npts, (Point2f){0, 0});
}

void testConvexHull(const int npoint, const int count) {
    Point2f *memory = malloc(npoint * 2 * sizeof(Point2f));
    Point2f *points = memory;
    Point2f *hullPoints = points + npoint;

    int nHullPoints;
    const Window window = getNewWindow("testConvexHull", 1000, 800);

    for(int i = 0; i < count; ++i) {
        randomCirclePoints(points, npoint, 350);

        TEST_TIME(convexHull(points, hullPoints, npoint, &nHullPoints));

        windowFill(window, 255, 255, 255);
        drawPoly(window, hullPoints, nHullPoints, 0xff, 1, 1);
        drawPointSet(window, hullPoints, nHullPoints, 0xff0000);

        showWindow(window);
    }

    destroyWindow(window);
    free(memory);
}

void testPtsMaxDist(const int npts, const int count) {
    const Window window = getNewWindow("testPtsMaxDist", 1000, 800);
    Point2f maxpts[2],
    *pts = malloc(npts * sizeof(Point2f));

    for(int i = 0; i < count; ++i) {
        randomRectPoints(pts, npts, 800.f, 600.f);

        TEST_TIME(ptsMaxDist(pts, maxpts, npts));

        windowFill(window, 255, 255, 255);
        drawPointSet(window, pts, npts, 0xff0000);
        drawLine(window, maxpts[0], maxpts[1], 0xff, 2);

        showWindow(window);
    }

    destroyWindow(window);
    free(pts);
}

void testMinEnclosingCircle(const int npts, const int count) {
    const Window w = getNewWindow("testMinEnclosingCircle", 1000, 800);
    Point2f *pts = malloc(npts * sizeof(Point2f));
    Point2f center;
    float radius;

    for(int i = 0; i < count; ++i) {
        randomCirclePoints(pts, npts, 350);

        TEST_TIME(minEnclosingCircle(pts, npts, &center, &radius));

        windowFill(w, 255, 255, 255);
        drawCircle(w, center, (int) radius, 0xff0000, 1);
        drawPointSet(w, pts, npts, 0x0000ff);

        showWindow(w);
    }

    destroyWindow(w);
    free(pts);
}