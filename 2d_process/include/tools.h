#ifndef ALL4ALGORITHM_TOOLS_H
#define ALL4ALGORITHM_TOOLS_H

#include "2d_proc.h"
#include <math.h>

static inline float minf(float a, float b){
    return a < b ? a : b;
}

static inline float maxf(float a, float b){
    return a > b ? a : b;
}

static inline float sqdist(Point2f p1, Point2f p2){
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

static inline float distance2f(Point2f p1, Point2f p2) {
    return sqrtf(sqdist(p1, p2));
}

static inline float cross_vec(Vector2f vec1, Vector2f vec2) {
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

static inline int parallel_vec(Vector2f vec1, Vector2f vec2){
    return cross_vec(vec1, vec2) == 0;
}

void cartesian2polar_pt(Point2f inpoints[], Point2f outpoints[], int nPoints, Point2f origin);

void cartesian2polar_vec(Vector2f inVectors[], Point2f outVectors[], int nVectors);

void polar2cartesian_pt(Point2f inpoints[], Point2f outpoints[], int nPoints, Point2f origin);

#endif //ALL4ALGORITHM_TOOLS_H
