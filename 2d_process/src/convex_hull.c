#include <string.h>
#include "tools.h"

#define SORT_ELEMENT_TYPE float*
#define SORT_LESS_THAN(p, q) (*(p) < *(q))
#include "sort.h"

#define STACK_ELEMENT_TYPE Point2f
#include "stack.h"

static void convexHullHelper(StackPtr stack, Point2f point) {
    Vector2f vec1, vec2;
    while (stack->top > 1) {
        vec1.x = point.x - stack->elements[stack->top - 2].x;
        vec1.y = point.y - stack->elements[stack->top - 2].y;
        vec2.x = point.x - stack->elements[stack->top - 1].x;
        vec2.y = point.y - stack->elements[stack->top - 1].y;
        if (cross_vec(vec1, vec2) > 0.f)
            break;
        stack_pop(stack);
    }
    stack_push(stack, point);
}

void convexHull(Point2f pts[], Point2f hullpts[], int npts, int *nhullpts) {
    if(npts < 3) {
        *nhullpts = npts;
        memcpy(hullpts, pts, sizeof(Point2f) * npts);
        return;
    }

    int i;
    Point2f origin = pts[0];

    for(i = 1; i < npts; ++i) {
        if(pts[i].y < origin.y || (pts[i].y == origin.y && pts[i].x < origin.x))
            origin = pts[i];
    }

    float *theta = (float *)malloc(npts * sizeof(float));
    float **thetaptr = (float **)malloc(npts * sizeof(float*));

    for(i = 0; i < npts; ++i) {
        theta[i] = atan2f(pts[i].y - origin.y, pts[i].x - origin.x);
        thetaptr[i] = theta + i;
    }

    sort(thetaptr, npts);

    StackPtr stack = newStack(npts);
    stack_push(stack, pts[thetaptr[0] - theta]);
    stack_push(stack, pts[thetaptr[1] - theta]);

    for(i = 2; i < npts; ++i)
        convexHullHelper(stack, pts[thetaptr[i] - theta]);

    convexHullHelper(stack, origin);

    *nhullpts = stack->top - 1;
    memcpy(hullpts, stack->elements, sizeof(Point2f) * *nhullpts);

    free(theta);
    free(thetaptr);
    stack_destroy(stack);
}

static void ptsMaxDist_force(Point2f hullpts[], Point2f *outpts, int nhullpts) {
    float max = 0, sqd;
    int i, j;

    for (i = 0; i < nhullpts; ++i) {
        for (j = i + 1; j < nhullpts; ++j) {
            sqd = sqdist(hullpts[i], hullpts[j]);
            if (sqd > max) {
                max = sqd;
                outpts[0] = hullpts[i];
                outpts[1] = hullpts[j];
            }
        }
    }
}

void convexHullDiameter(Point2f hullpts[], Point2f *outpts, int nhullpts) {
    if (nhullpts <= 10) {
        ptsMaxDist_force(hullpts, outpts, nhullpts);
        return;
    }

    int i = 0, j = 3;
    Point2f p1, p2, p3, p4;
    Vector2f v1, v2;
    float sqd, max = 0.f;

    p2 = hullpts[0];
    p4 = hullpts[2];
    v2 = (Vector2f) {p4.x - hullpts[1].x, p4.y - hullpts[1].y};
    do {
        ++i;
        p1 = p2;
        p2 = hullpts[i == nhullpts ? 0 : i];
        v1 = (Vector2f) {p2.x - p1.x, p2.y - p1.y};
        do {
            if (cross_vec(v1, v2) <= 0.f)
                break;

            p3 = p4;
            p4 = hullpts[j == nhullpts ? 0 : j];
            v2 = (Vector2f) {p4.x - p3.x, p4.y - p3.y};

            if (j == nhullpts)
                j = 1;
            else
                ++j;
        } while (1);

        sqd = sqdist(p1, p3);
        if (sqd > max) {
            max = sqd;
            outpts[0] = p1;
            outpts[1] = p3;
        }
    } while (i != nhullpts);
}

void ptsMaxDist(Point2f pts[], Point2f *maxPoint, int npts) {
    if (npts <= 10) {
        ptsMaxDist_force(pts, maxPoint, npts);
        return;
    }

    int nhullpts;
    Point2f *hullpts = malloc(npts* sizeof(Point2f));
    if(!hullpts)
        return;

    convexHull(pts, hullpts, npts, &nhullpts);
    convexHullDiameter(hullpts, maxPoint, nhullpts);

    free(hullpts);
}
