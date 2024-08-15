#include "opencv2/opencv.hpp"
#include "2d_proc.h"

typedef unsigned Window;

cv::Mat windows[8];
const char *windowNames[8];

extern "C" {
unsigned getNewWindow(const char *name, int width, int height) {
    static unsigned windowCount = 0;
    windows[windowCount] = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
    windowNames[windowCount] = name;
    return windowCount++;
}

void fillWindow(const Window window, uchar r, uchar g, uchar b) {
    windows[window] = cv::Scalar(b, g, r);
}

void drawPoly(const Window window, const Point2f *points, int nponts, int thickness) {
    cv::Mat img = windows[window];
    int halfHeight = img.rows / 2,
            halfWidth = img.cols / 2;

    for (int i = 0, j = 1; j < nponts; ++i, ++j) {
        cv::line(img, cv::Point(halfWidth + (int) points[i].x, halfHeight - (int) points[i].y),
                 cv::Point(halfWidth + (int) points[j].x, halfHeight - (int) points[j].y),
                 cv::Scalar(0, 0, 0), thickness);
    }
}

void drawCircle(const Window window, const Point2f *points, int nponts, int radius, int thickness) {
    cv::Mat img = windows[window];
    int halfHeight = img.rows / 2,
            halfWidth = img.cols / 2;

    for (int i = 0; i < nponts; ++i)
        cv::circle(img, cv::Point(halfWidth + (int) points[i].x, halfHeight - (int) points[i].y), radius,
                   cv::Scalar(0, 0, 0), thickness);

    img.at<cv::Vec3b>(0, 0) = cv::Vec3b(0, 0, 0);
}

void showWindow(const Window window) {
    cv::imshow(windowNames[window], windows[window]);
    cv::waitKey(0);
}

void destroyWindow(const Window window) {
    cv::destroyWindow(windowNames[window]);
}
}