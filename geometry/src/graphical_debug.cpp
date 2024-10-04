#include "opencv2/opencv.hpp"
#include "geometry.h"

#define WINDOW_COUNT 8

struct Window {
    char *name{};
    cv::Mat image{};
    int used = 0;
};

Window windows[WINDOW_COUNT];

static inline cv::Point to2dPoint(Point2f point, int halfWidth, int halfHeight) {
    return {halfWidth + (int) point.x, halfHeight - (int) point.y};
}

static inline cv::Scalar toScalar(int rgb) {
    return {(double)(rgb & 0xff), (double)((rgb >> 8) & 0xff), (double)(rgb >> 16)};
}

extern "C" {
int getNewWindow(char *name, int width, int height) {
    static int windowCount = 0;
    int count = windowCount;

    do {
        if (++count == WINDOW_COUNT)
            count = 0;
        if (windows[count].used == 0)
            break;
    } while (count != windowCount);

    Window *window = windows + count;

    if (window->used)
        return -1;

    windowCount = count;
    window->image = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
    window->name = new char[strlen(name) + 1];
    strcpy(window->name, name);

    window->used = 1;

    return count;
}

void fillWindow(int window, uchar r, uchar g, uchar b) {
    windows[window].image = cv::Scalar(b, g, r);
}

void drawLine(int window, const Point2f p1, const Point2f p2, int rgb, int thickness) {
    cv::Mat img = windows[window].image;
    int halfHeight = img.rows / 2,
            halfWidth = img.cols / 2;

    cv::line(img, to2dPoint(p1, halfWidth, halfHeight), to2dPoint(p2, halfWidth, halfHeight),
             toScalar(rgb), thickness);
}

void drawPoly(int window, const Point2f *points, int nponts, int rgb, int thickness, int connect) {
    cv::Mat img = windows[window].image;
    int halfHeight = img.rows / 2,
            halfWidth = img.cols / 2;
    cv::Scalar color = toScalar(rgb);

    if (thickness < 0) {
        auto *pts = new cv::Point[nponts];
        for (int i = 0; i < nponts; ++i)
            pts[i] = to2dPoint(points[i], halfWidth, halfHeight);

        cv::fillConvexPoly(img, pts, nponts, color);

        delete[] pts;
        return;
    }


    for (int i = 0, j = 1; j < nponts; ++i, ++j) {
        cv::line(img, to2dPoint(points[i], halfWidth, halfHeight),
                 to2dPoint(points[j], halfWidth, halfHeight),
                 color, thickness);
    }

    if (connect)
        cv::line(img, to2dPoint(points[nponts - 1], halfWidth, halfHeight),
                 to2dPoint(points[0], halfWidth, halfHeight),
                 color, thickness);
}

void drawCircle(int window, const Point2f *points, int nponts, int rgb, int radius, int thickness) {
    cv::Mat img = windows[window].image;
    int halfHeight = img.rows / 2,
            halfWidth = img.cols / 2;
    cv::Scalar color = toScalar(rgb);

    for (int i = 0; i < nponts; ++i)
        cv::circle(img, to2dPoint(points[i], halfWidth, halfHeight), radius,
                   color, thickness);
}

void showWindow(int window) {
    Window *w = windows + window;
    cv::imshow(w->name, w->image);
    cv::waitKey(0);
}

void destroyWindow(int window) {
    Window *w = windows + window;

    cv::destroyWindow(w->name);
    w->used = 0;

    delete[] w->name;
}

}