#ifndef ALL4ALGORITHM_GRAPHICAL_DEBUG_H
#define ALL4ALGORITHM_GRAPHICAL_DEBUG_H

#include "2d_proc.h"

typedef unsigned Window;

Window getNewWindow(const char *name, int width, int height);

void fillWindow(const Window window, unsigned char r, unsigned char g, unsigned char b);

void drawPoly(Window window, const Point2f *points, int nponts, int thickness);

void drawCircle(Window window, const Point2f *points, int nponts, int radius, int thickness);

void showWindow(Window window);

void destroyWindow(Window window);

#endif //ALL4ALGORITHM_GRAPHICAL_DEBUG_H
