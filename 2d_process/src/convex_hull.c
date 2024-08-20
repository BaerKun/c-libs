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
        if (cross(vec1, vec2) > 0.f)
            break;
        stack_pop(stack);
    }
    stack_push(stack, point);
}

void convexHull(Point2f points[], Point2f hullPoints[], int nPoints, int *nHullPoints) {
    if(nPoints <= 3) {
        memcpy(hullPoints, points, sizeof(Point2f) * nPoints);
    }

    int i;
    Point2f origin = points[0];

    for(i = 1; i < nPoints; ++i) {
        if(points[i].y < origin.y || (points[i].y == origin.y && points[i].x < origin.x))
            origin = points[i];
    }

    float theta[nPoints];
    float * pTheta[nPoints];

    for(i = 0; i < nPoints; ++i) {
        theta[i] = atan2f(points[i].y - origin.y, points[i].x - origin.x);
        pTheta[i] = theta + i;
    }

    insertionSort(pTheta, nPoints);

    StackPtr stack = newStack(nPoints);
    stack_push(stack, points[pTheta[0] - theta]);
    stack_push(stack, points[pTheta[1] - theta]);

    for(i = 2; i < nPoints; ++i)
        convexHullHelper(stack, points[pTheta[i] - theta]);

    convexHullHelper(stack, origin);

    *nHullPoints = stack->top - 1;
    memcpy(hullPoints, stack->elements, sizeof(Point2f) * *nHullPoints);

    stack_destroy(stack);
}
