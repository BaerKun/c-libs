#ifndef ALL4ALGORITHM_TOOLS_H
#define ALL4ALGORITHM_TOOLS_H

#include "2d_proc.h"
#include <math.h>

float randFloat(float a, float b);

static inline float distance2f(Point2f p1, Point2f p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrtf(dx * dx + dy * dy);
}

static inline float cross(Vector2f vec1, Vector2f vec2) {
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

void cartesian2polar_pt(Point2f inpoints[], Point2f outpoints[], int nPoints, Point2f origin);

void cartesian2polar_vec(Vector2f inVectors[], Point2f outVectors[], int nVectors);

void polar2cartesian_pt(Point2f inpoints[], Point2f outpoints[], int nPoints, Point2f origin);

#endif //ALL4ALGORITHM_TOOLS_H
