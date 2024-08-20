#include "../include/tools.h"
#include <time.h>
#include <math.h>

unsigned randSeed = 0;

static unsigned getRandSeed(){
    if(randSeed == 0)
        randSeed = (unsigned)time(NULL) & 0xffff;
    return ++randSeed;
}

float randFloat(float a, float b) {
    unsigned seed = getRandSeed();
    return (a + b + (b - a) * sinf((float)(seed * seed))) * 0.5f;
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

