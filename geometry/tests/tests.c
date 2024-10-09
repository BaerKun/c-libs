#include "graphical.h"
#include "point_set.h"
#include "random.h"
#include "timer.h"
#include "geom_utils.h"
#include <stdlib.h>

# define  WINDOW_WIDTH  1000
# define  WINDOW_HEIGHT  800
static const Point2i origin = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};

static void toImageCoordArray(const Point2f *src, Point2i *dst, Point2i center, int npts) {
    for(int i = 0; i < npts; ++i)
        dst[i] = toImageCoord(src[i], center);
}

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
    Point2f *memory = malloc(npoint * 3 * sizeof(Point2f));
    Point2f *points = memory;
    Point2f *hullPoints = points + npoint;
    Point2i *hullPointsImg = (Point2i *)(memory + npoint * 2);

    int nHullPoints;
    const Window window = getNewWindow("testConvexHull", WINDOW_WIDTH, WINDOW_HEIGHT);

    for(int i = 0; i < count; ++i) {
        randomCirclePoints(points, npoint, WINDOW_HEIGHT / 2 - 50);

        TEST_TIME(convexHull(points, hullPoints, npoint, &nHullPoints));

        windowFill(window, 255, 255, 255);

        toImageCoordArray(hullPoints, hullPointsImg, origin, nHullPoints);
        drawPoly(window, hullPointsImg, nHullPoints, 0xff, 1, 1);
        drawPointSet(window, hullPointsImg, nHullPoints, 0xff0000);

        showWindow(window);
        waitKey(0);
    }

    destroyWindow(window);
    free(memory);
}

void testPtsMaxDist(const int npts, const int count) {
    const Window window = getNewWindow("testPtsMaxDist", WINDOW_WIDTH, WINDOW_HEIGHT);
    Point2f maxpts[2],
    *pts = malloc(npts * 2 * sizeof(Point2f));
    Point2i *imgpts = (Point2i *) pts + npts;

    for(int i = 0; i < count; ++i) {
        randomRectPoints(pts, npts, WINDOW_WIDTH - 100, WINDOW_HEIGHT - 100);

        TEST_TIME(ptsMaxDist(pts, maxpts, npts));

        windowFill(window, 255, 255, 255);
        toImageCoordArray(pts, imgpts, origin, npts);
        drawPointSet(window, imgpts, npts, 0xff0000);
        drawLine(window, toImageCoord(maxpts[0], origin), toImageCoord(maxpts[1], origin), 0xff, 2);

        showWindow(window);
        waitKey(0);
    }

    destroyWindow(window);
    free(pts);
}

void testMinEnclosingCircle(const int npts, const int count) {
    const Window w = getNewWindow("testMinEnclosingCircle", WINDOW_WIDTH, WINDOW_HEIGHT);
    Point2f *pts = malloc(npts * 2 * sizeof(Point2f));
    Point2i *imgpts = (Point2i *) pts + npts;
    Point2f center;
    float radius;

    for(int i = 0; i < count; ++i) {
        randomCirclePoints(pts, npts, WINDOW_HEIGHT / 2 - 50);

        TEST_TIME(minEnclosingCircle(pts, npts, &center, &radius));

        toImageCoordArray(pts, imgpts, origin, npts);

        windowFill(w, 255, 255, 255);
        drawCircle(w, toImageCoord(center, origin), (int) radius, 0xff0000, 1);
        drawPointSet(w, imgpts, npts, 0x0000ff);

        showWindow(w);
        waitKey(0);
    }

    destroyWindow(w);
    free(pts);
}