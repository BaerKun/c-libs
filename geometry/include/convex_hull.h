#ifndef ALL4ALGORITHM_CONVEX_HULL_H
#define ALL4ALGORITHM_CONVEX_HULL_H

#include "geometry.h"

void convexHull(Point2f pts[], Point2f hullpts[], int npts, int *nhullpts);

void convexHullDiameter(Point2f hullpts[], Point2f *outpts, int nhullpts);

void ptsMaxDist(Point2f pts[], Point2f *maxPoint, int npts);

#endif //ALL4ALGORITHM_CONVEX_HULL_H
