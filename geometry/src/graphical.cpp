#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utils/logger.hpp"
#include "geometry.h"
#include "graphical.h"

#define WINDOW_COUNT 8

struct WindowData {
    cv::Mat image{};
    char *name{};
    int used = 0;
};

WindowData windows[WINDOW_COUNT];

static inline cv::Scalar toScalar(const int &rgb) {
    return {(double) (rgb & 0xff), (double) ((rgb >> 8) & 0xff), (double) (rgb >> 16)};
}

extern "C" {
void graphicalInit() {
#ifdef WIN32
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
#endif
}

Window getNewWindow(const char *name, const int width, const int height) {
    static int windowCount = 0;
    int count = windowCount;

    do {
        if (++count == WINDOW_COUNT)
            count = 0;
        if (windows[count].used == 0)
            break;
    } while (count != windowCount);

    WindowData *window = windows + count;

    if (window->used)
        return -1;

    windowCount = count;
    window->image = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
    window->name = new char[strlen(name) + 1];
    strcpy(window->name, name);

    window->used = 1;

    return count;
}

void windowFill(const Window window, const uchar r, const uchar g, const uchar b) {
    windows[window].image = cv::Scalar(b, g, r);
}

void drawPoint(const Window window, const Point2i point, const int rgb) {
    cv::Mat img = windows[window].image;

    cv::circle(img, reinterpret_cast<const cv::Point2i &>(point), 3, toScalar(rgb), -1);
}

void drawPointSet(const Window window, const Point2i *points, const int nponts, const int rgb) {
    cv::Mat img = windows[window].image;
    const cv::Scalar color = toScalar(rgb);

    for (int i = 0; i < nponts; ++i)
        cv::circle(img, *(cv::Point2i *)(points + i), 3, color, -1);
}

void drawLine(const Window window, const Point2i p1, const Point2i p2, const int rgb, const int thickness) {
    cv::Mat img = windows[window].image;

    cv::line(img, reinterpret_cast<const cv::Point2i&>(p1), reinterpret_cast<const cv::Point2i&>(p2),
             toScalar(rgb), thickness);
}

void drawRect(const Window window, const Point2i lefttop, const int width, const int height, const int rgb,
              const int thickness) {
    cv::Mat img = windows[window].image;
    const cv::Rect rect(lefttop.x, lefttop.y, width, height);

    cv::rectangle(img, rect, toScalar(rgb), thickness);
}

void drawPoly(const Window window, const Point2i *points, const int nponts, const int rgb, const int thickness,
              const int connect) {
    cv::Mat img = windows[window].image;
    const cv::Scalar color = toScalar(rgb);

    if (thickness < 0) {
        auto *pts = new cv::Point[nponts];
        for (int i = 0; i < nponts; ++i)
            pts[i] = *(cv::Point2i *)(points + i);

        cv::fillConvexPoly(img, pts, nponts, color);

        delete[] pts;
        return;
    }


    for (int i = 0, j = 1; j < nponts; ++i, ++j) {
        cv::line(img, *(cv::Point2i *)(points + i), *(cv::Point2i *)(points + j),
                 color, thickness);
    }

    if (connect)
        cv::line(img, *(cv::Point2i *)(points + nponts - 1), *(cv::Point2i *)points,
                 color, thickness);
}

void drawCircle(const Window window, const Point2i center, const int radius, const int rgb, const int thickness) {
    cv::Mat img = windows[window].image;
    const cv::Scalar color = toScalar(rgb);

    cv::circle(img, reinterpret_cast<const cv::Point2i&>(center), radius,
               color, thickness);
}

void drawText(const Window window, const char *text, const Point2i leftbottom, const int rgb, const int fontsize) {
    cv::Mat img = windows[window].image;

    cv::putText(img, text, reinterpret_cast<const cv::Point2i&>(leftbottom),
                cv::FONT_HERSHEY_SIMPLEX, fontsize / 20.0, toScalar(rgb));
}

char waitKey(const int ms) {
    return (char)cv::waitKey(ms);
}

void showWindow(const Window window) {
    const WindowData *w = windows + window;
    cv::imshow(w->name, w->image);
}

void destroyWindow(const Window window) {
    WindowData *w = windows + window;

    cv::destroyWindow(w->name);
    w->used = 0;

    delete[] w->name;
}
}
