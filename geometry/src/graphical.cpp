#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utils/logger.hpp"
#include "geometry.h"

#define WINDOW_COUNT 8

struct Window {
    char *name{};
    cv::Mat image{};
    int used = 0;
};

Window windows[WINDOW_COUNT];

static inline cv::Point to2dPoint(const Point2f &point, const int &halfWidth, const int &halfHeight) {
    return {halfWidth + (int) point.x, halfHeight - (int) point.y};
}

static inline cv::Scalar toScalar(const int &rgb) {
    return {(double) (rgb & 0xff), (double) ((rgb >> 8) & 0xff), (double) (rgb >> 16)};
}

static inline cv::Vec3b toVec3b(const int &rgb) {
    return {(uchar) (rgb & 0xff), (uchar) ((rgb >> 8) & 0xff), (uchar) (rgb >> 16)};
}

extern "C" {
void graphicalInit() {
#ifdef WIN32
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
#endif
}

int getNewWindow(const char *name, const int width, const int height) {
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

void windowFill(const int window, const uchar r, const uchar g, const uchar b) {
    windows[window].image = cv::Scalar(b, g, r);
}

void drawPoint(const int window, const Point2f point, const int rgb) {
    cv::Mat img = windows[window].image;
    const int halfHeight = img.rows / 2,
            halfWidth = img.cols / 2;

    cv::circle(img, to2dPoint(point, halfWidth, halfHeight), 3, toScalar(rgb), -1);
}

void drawPointSet(const int window, const Point2f *points, const int nponts, const int rgb) {
    cv::Mat img = windows[window].image;
    const int halfHeight = img.rows / 2,
            halfWidth = img.cols / 2;
    const cv::Scalar color = toScalar(rgb);

    for (int i = 0; i < nponts; ++i)
        cv::circle(img, to2dPoint(points[i], halfWidth, halfHeight), 3, color, -1);
}

void drawLine(const int window, const Point2f p1, const Point2f p2, const int rgb, const int thickness) {
    cv::Mat img = windows[window].image;
    const int halfHeight = img.rows / 2,
            halfWidth = img.cols / 2;

    cv::line(img, to2dPoint(p1, halfWidth, halfHeight), to2dPoint(p2, halfWidth, halfHeight),
             toScalar(rgb), thickness);
}

void drawRect(const int window, const Point2f lefttop, const int width, const int height, const int rgb,
              const int thickness) {
    cv::Mat img = windows[window].image;
    const int halfHeight = img.rows / 2,
            halfWidth = img.cols / 2;
    const cv::Rect rect(to2dPoint(lefttop, halfWidth, halfHeight), cv::Size(width, height));

    cv::rectangle(img, rect, toScalar(rgb), thickness);
}

void drawPoly(const int window, const Point2f *points, const int nponts, const int rgb, const int thickness,
              const int connect) {
    cv::Mat img = windows[window].image;
    const int halfHeight = img.rows / 2,
            halfWidth = img.cols / 2;
    const cv::Scalar color = toScalar(rgb);

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

void drawCircle(const int window, const Point2f center, const int radius, const int rgb, const int thickness) {
    cv::Mat img = windows[window].image;
    const int halfHeight = img.rows / 2,
            halfWidth = img.cols / 2;
    const cv::Scalar color = toScalar(rgb);

    cv::circle(img, to2dPoint(center, halfWidth, halfHeight), radius,
               color, thickness);
}

void drawText(const int window, const char *text, const Point2f leftbottom, const int rgb, const int fontsize) {
    cv::Mat img = windows[window].image;
    const int halfHeight = img.rows / 2,
            halfWidth = img.cols / 2;

    cv::putText(img, text, to2dPoint(leftbottom, halfWidth, halfHeight),
                cv::FONT_HERSHEY_SIMPLEX, fontsize / 20.0, toScalar(rgb));
}

char waitKey(const int ms) {
    return (char)cv::waitKey(ms);
}

void showWindow(const int window) {
    const Window *w = windows + window;
    cv::imshow(w->name, w->image);
}

void destroyWindow(const int window) {
    Window *w = windows + window;

    cv::destroyWindow(w->name);
    w->used = 0;

    delete[] w->name;
}
}
