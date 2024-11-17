#ifndef ALL4ALGORITHM_GEOM_UTILS_H
#define ALL4ALGORITHM_GEOM_UTILS_H

#include "geometry.h"
#include <math.h>

static inline Point2i toImageCoord(const Point2f p, const Point2i origin) {
    return (Point2i){(int) roundf(p.x) + origin.x, origin.y - (int) roundf(p.y)};
}

static inline Point2f toMathCoord(const Point2i p, const Point2i origin) {
    return (Point2f){p.x - origin.x, origin.y - p.y};
}

static inline float minf(const float a, const float b) {
    return a < b ? a : b;
}

static inline float maxf(const float a, const float b) {
    return a > b ? a : b;
}

static inline float sqr(const float a) {
    return a * a;
}

static inline float sum_sqr(const float a, const float b) {
    return a * a + b * b;
}

static inline float sqr_vec2(const Vector2f vec) {
    return sum_sqr(vec.x, vec.y);
}

static inline float sqrdist(const Point2f p1, const Point2f p2) {
    return sum_sqr(p1.x - p2.x, p1.y - p2.y);
}

static inline Point2f midpt(const Point2f a, const Point2f b) {
    return (Point2f){(a.x + b.x) / 2, (a.y + b.y) / 2};
}

static inline float norm_vec2(const Vector2f vec) {
    return sqrtf(sqr_vec2(vec));
}

static inline float dist2f(const Point2f p1, const Point2f p2) {
    return sqrtf(sqrdist(p1, p2));
}

static inline float cross_vec(const Vector2f vec1, const Vector2f vec2) {
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

static inline int parallel_vec(const Vector2f vec1, const Vector2f vec2) {
    return cross_vec(vec1, vec2) == 0;
}

static inline float vec2_dot(const Vector2f vec1, const Vector2f vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

static inline float vec3_dot(const Vector3f vec1, const Vector3f vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

#endif //ALL4ALGORITHM_GEOM_UTILS_H
