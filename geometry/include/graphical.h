#ifndef ALL4ALGORITHM_GRAPHICAL_H
#define ALL4ALGORITHM_GRAPHICAL_H

#include "geometry.h"
#include <math.h>

typedef int Window;

#ifdef __cplusplus
extern "C" {
inline Point2i toImageCoord(const Point2f &p, const Point2i &origin) {
    return {(int) roundf(p.x) + origin.x, origin.y - (int) roundf(p.y)};
}
#else
static inline Point2i toImageCoord(const Point2f p, const Point2i origin) {
    return (Point2i) {(int)roundf(p.x) + origin.x, origin.y - (int)roundf(p.y)};
}
#endif

void graphicalInit();

Window getNewWindow(const char *name, int width, int height);

void windowFill(Window window, unsigned char r, unsigned char g, unsigned char b);

void drawPoint(int window, Point2i p, int rgb);

void drawPointSet(int window, const Point2i *points, int npoints, int rgb);

void drawLine(int window, Point2i p1, Point2i p2, int rgb, int thickness);

void drawRect(Window window, Point2i lefttop, int width, int height, int rgb, int thickness);

void drawPoly(Window window, const Point2i *points, int nponts, int rgb, int thickness, int connect);

void drawCircle(Window window, Point2i center, int radius, int rgb, int thickness);

void drawText(Window window, const char *text, Point2i leftbottom, int rgb, int fontsize);

char waitKey(int ms);

void showWindow(Window window);

void destroyWindow(Window window);

#ifdef __cplusplus
}
#endif

#endif //ALL4ALGORITHM_GRAPHICAL_H
