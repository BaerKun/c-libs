#include "tools.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define SORT_ELEMENT_TYPE Point2f
#define SORT_LESS_THAN(p, q) ((p).x < (q).x)
#include "sort.h"

static float ptsMinDistHelper_force(Point2f *ptsleft, Point2f *ptsright, Point2f *outleft, Point2f *outright, float minsqd) {
    Point2f *p, *q;
    float sqd;
    for (p = ptsleft; p != ptsright; ++p) {
        for (q = p + 1; q <= ptsright; ++q) {
            sqd = sqdist(*p, *q);
            if (sqd < minsqd) {
                minsqd = sqd;
                *outleft = *p;
                *outright = *q;
            }
        }
    }
    return minsqd;
}

static float ptsMinDistHelper(Point2f pts[], Point2f outpts[], int left, int right) {
    if (right - left < 8)
        return ptsMinDistHelper_force(pts + left, pts + right, outpts + left, outpts + right, FLT_MAX);

    int center = (left + right) / 2;
    float leftMinSqd, rightMinSqd, midMinSqd;

    leftMinSqd = ptsMinDistHelper(pts, outpts, left, center);
    rightMinSqd = ptsMinDistHelper(pts, outpts, center + 1, right);

    if (leftMinSqd < rightMinSqd) {
        midMinSqd = leftMinSqd;
        outpts[right] = outpts[center];
    } else {
        midMinSqd = rightMinSqd;
        outpts[left] = outpts[center + 1];
    }

    float borderWidth = sqrtf(midMinSqd);
    int borderLeft = center, borderRight = center;

    while (borderLeft > left && pts[center].x - pts[borderLeft - 1].x < borderWidth)
        --borderLeft;

    while (borderRight < right && pts[borderRight + 1].x - pts[center].x < borderWidth)
        ++borderRight;

    return ptsMinDistHelper_force(pts + borderLeft, pts + borderRight, outpts + left, outpts + right, midMinSqd);
}

float ptsMinDist(Point2f pts[], Point2f outpts[2], int npts) {
    Point2f *tmpOutpts = malloc(sizeof(Point2f) * npts);
    if (tmpOutpts == NULL)
        return -1.f;

    sort(pts, npts);

    float mindist = sqrtf(ptsMinDistHelper(pts, tmpOutpts, 0, npts - 1));

    outpts[0] = tmpOutpts[0];
    outpts[1] = tmpOutpts[npts - 1];

    free(tmpOutpts);
    return mindist;
}
