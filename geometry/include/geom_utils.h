#ifndef ALL4ALGORITHM_GEOM_UTILS_H
#define ALL4ALGORITHM_GEOM_UTILS_H

#include "geometry.h"
#include <math.h>

static inline float minf(const float a, const float b){
    return a < b ? a : b;
}

static inline float maxf(const float a, const float b){
    return a > b ? a : b;
}

static inline Point2f midpoint(const Point2f a, const Point2f b){
    return (Point2f){(a.x + b.x) / 2, (a.y + b.y) / 2};
}

static inline float sum_square(const float a, const float b) {
    return a * a + b * b;
}

static inline float sqrdist(const Point2f p1, const Point2f p2){
    return sum_square(p1.x - p2.x, p1.y - p2.y);
}

static inline float dist2f(const Point2f p1, const Point2f p2) {
    return sqrtf(sqrdist(p1, p2));
}

static inline float cross_vec(const Vector2f vec1, const Vector2f vec2) {
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

static inline int parallel_vec(const Vector2f vec1, const Vector2f vec2){
    return cross_vec(vec1, vec2) == 0;
}

static inline float vec3_dot(const Vector3f vec1, const Vector3f vec2){
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

void cartesian2polar_pt(Point2f inpts[], Point2f outpts[], int npts, Point2f origin);

void cartesian2polar_vec(Vector2f inVectors[], Point2f outVectors[], int nvecs);

void polar2cartesian_pt(Point2f pts[], Point2f outpts[], int npts, Point2f origin);

void polar2cartesian_vec(Vector2f invecs[], Vector2f outvecs[], int nvecs);

void triCircumcircle(Point2f pt1, Point2f pt2, Point2f pt3, Point2f *center, float *radius);

#endif //ALL4ALGORITHM_GEOM_UTILS_H
