#ifndef ALL4ALGORITHM_GRAPHICAL_DEBUG_H
#define ALL4ALGORITHM_GRAPHICAL_DEBUG_H

#include "2d_proc.h"

typedef int Window;

Window getNewWindow(char *name, int width, int height);

void fillWindow(Window window, unsigned char r, unsigned char g, unsigned char b);

void drawLine(int window, Point2f p1, Point2f p2, int rgb, int thickness);

void drawPoly(Window window, const Point2f *points, int nponts, int rgb, int thickness, int connect);

void drawCircle(Window window, const Point2f *points, int nponts, int rgb, int radius, int thickness);

void showWindow(Window window);

void destroyWindow(Window window);

#endif //ALL4ALGORITHM_GRAPHICAL_DEBUG_H
