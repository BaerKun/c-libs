#include "graphical_debug.h"
#include <math.h>
#include "2d_proc.h"
#include <string.h>
#include <time.h>

#define SORT_ELEMENT_TYPE float*
#define SORT_LESS_THAN(p, q) (*p < *q)
#include "sort.h"

#define STACK_ELEMENT_TYPE Point2f
#include "stack.h"


float randFloat(float a, float b){
    static unsigned count = 0;
    ++count;
    return (a + b + (b - a) * sinf((float)((time(NULL) & 0xffff) + count * count))) / 2.f;
}

float distance2f(Point2f p1, Point2f p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrtf(dx * dx + dy * dy);
}

void cartesian2polar_pt(Point2f inpoints[], Point2f outpoints[], int nPoints, Point2f origin) {
    float dx, dy;
    for(int i = 0; i < nPoints; ++i) {
        dx = inpoints[i].x - origin.x;
        dy = inpoints[i].y - origin.y;
        outpoints[i].x = atan2f(dy, dx);
        outpoints[i].y = sqrtf(dx * dx + dy * dy);
    }
}

void cartesian2polar_vec(Vector2f inVectors[], Point2f outVectors[], int nVectors) {
    for(int i = 0; i < nVectors; ++i) {
        outVectors[i].x = atan2f(inVectors[i].y, inVectors[i].x);
        outVectors[i].y = sqrtf(inVectors[i].x * inVectors[i].x + inVectors[i].y * inVectors[i].y);
    }
}

void polar2cartesian_pt(Point2f inpoints[], Point2f outpoints[], int nPoints, Point2f origin) {
    float theta, r;
    for(int i = 0; i < nPoints; ++i) {
        theta = inpoints[i].x;
        r = inpoints[i].y;
        outpoints[i].x = origin.x + r * cosf(theta);
        outpoints[i].y = origin.y + r * sinf(theta);
    }
}

float cross(Vector2f vec1, Vector2f vec2) {
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

void convexHullHelper(StackPtr stack, Point2f point) {
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

int main() {
    Point2f points[20];
    Point2f hullPoints[20];
    int nHullPoints;
    Window window = getNewWindow("Convex Hull", 1000, 800);

    for(int j = 0; j < 10; ++j){
        for (int i = 0; i < 20; ++i) {
            points[i].x = randFloat(-450.f, 450.f);
            points[i].y = randFloat(-350.f, 350.f);
        }
        convexHull(points, hullPoints, 20, &nHullPoints);

        hullPoints[nHullPoints] = hullPoints[0];

        drawCircle(window, points, 20, 2, -1);
        drawPoly(window, hullPoints, nHullPoints + 1, 1);

        showWindow(window);

        fillWindow(window, 255, 255, 255);
    }

    destroyWindow(window);
    return 1;
}