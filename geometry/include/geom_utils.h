#ifndef ALL4ALGORITHM_GEOM_UTILS_H
#define ALL4ALGORITHM_GEOM_UTILS_H

#include "geometry.h"
#include <math.h>

static inline float minf(float a, float b){
    return a < b ? a : b;
}

static inline float maxf(float a, float b){
    return a > b ? a : b;
}

static inline Point2f midpoint(Point2f a, Point2f b){
    return (Point2f){(a.x + b.x) / 2, (a.y + b.y) / 2};
}

static inline float sqrdist(Point2f p1, Point2f p2){
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

static inline float dist2f(Point2f p1, Point2f p2) {
    return sqrtf(sqrdist(p1, p2));
}

static inline float cross_vec(Vector2f vec1, Vector2f vec2) {
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

static inline int parallel_vec(Vector2f vec1, Vector2f vec2){
    return cross_vec(vec1, vec2) == 0;
}

static inline float vec3_dot(Vector3f vec1, Vector3f vec2){
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

static inline float sum_square(float a, float b){
    return a * a + b * b;
}

void cartesian2polar_pt(Point2f inpts[], Point2f outpts[], int nPoints, Point2f origin);

void cartesian2polar_vec(Vector2f inVectors[], Point2f outVectors[], int nVectors);

void polar2cartesian_pt(Point2f pts[], Point2f outpts[], int nPoints, Point2f origin);

void triCircumcircle(Point2f pt1, Point2f pt2, Point2f pt3, Point2f *center, float *radius);

#endif //ALL4ALGORITHM_GEOM_UTILS_H
