#include "graphical_debug.h"
#include "point_set.h"
#include "random.h"
#include "timer.h"
#include "geom_utils.h"

void initTest(){
    int64_t seed = time(0) ^ (int64_t)initTest;
    pcg32_srandom(seed, seed << 1 | 1);
}

void testConvexHull(int npoint) {
    Point2f points[npoint];
    Point2f hullPoints[npoint];

    int nHullPoints;
    Window window = getNewWindow("testConvexHull", 1000, 800);

    for (int i = 0; i < npoint; ++i) {
        points[i].x = randfloat(-450.f, 450.f);
        points[i].y = randfloat(-350.f, 350.f);
    }

    TEST_TIME(convexHull(points, hullPoints, npoint, &nHullPoints));

    drawCircle(window, points, npoint, 0xff0000, 2, -1);
    drawPoly(window, hullPoints, nHullPoints, 0xff, 1, 1);

    showWindow(window);

    destroyWindow(window);
}

void testPtsMaxDist(int npts) {
    Point2f pts[npts], maxpts[2];
    Window window = getNewWindow("testPtsMaxDist", 1000, 800);

    for (int i = 0; i < npts; ++i)
        pts[i] = (Point2f) {randfloat(-450.0f, 450.0f), randfloat(-350.0f, 350.0f)};

    TEST_TIME(ptsMaxDist(pts, maxpts, npts));

    drawCircle(window, pts, npts, 0xff0000, 2, -1);
    drawLine(window, maxpts[0], maxpts[1], 0xff, 2);

    showWindow(window);
    destroyWindow(window);
}

void testMinEnclosingCircle(int npts){
    Point2f pts[npts];
    Point2f center;
    float radius;

    for(int i = 0; i < npts; ++i)
        pts[i] = (Point2f){randfloat(0, 2.f * M_PI), randfloat(0, 350)};

    polar2cartesian_pt(pts, pts, npts, (Point2f){0, 0});

    TEST_TIME(minEnclosingCircle(pts, npts, &center, &radius));

    Window w = getNewWindow("testMinEnclosingCircle", 1000, 800);
    drawCircle(w, &center, 1, 0xff0000, (int)radius, 1);
    drawCircle(w, pts, npts, 0x0000ff, 2, -1);

    showWindow(w);
    destroyWindow(w);
}