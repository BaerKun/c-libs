#ifndef ALL4ALGORITHM_GRAPHICAL_H
#define ALL4ALGORITHM_GRAPHICAL_H

#include "geometry.h"

typedef int Window;

void graphicalInit();

Window getNewWindow(char *name, int width, int height);

void windowFill(Window window, unsigned char r, unsigned char g, unsigned char b);

void drawPoint(int window, Point2f p, int rgb);

void drawPointSet(int window, const Point2f *points, int npoints, int rgb);

void drawLine(int window, Point2f p1, Point2f p2, int rgb, int thickness);

void drawRect(Window window, Point2f lefttop, int width, int height, int rgb, int thickness);

void drawPoly(Window window, const Point2f *points, int nponts, int rgb, int thickness, int connect);

void drawCircle(Window window, Point2f center, int radius, int rgb, int thickness);

void drawText(Window window, const char *text, Point2f leftbottom, int rgb, int fontsize);

char waitKey(int ms);

void showWindow(Window window);

void destroyWindow(Window window);

#endif //ALL4ALGORITHM_GRAPHICAL_H
