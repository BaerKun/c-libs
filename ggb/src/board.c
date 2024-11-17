#include <stdlib.h>

#include "board.h"
#include "geom_errors.h"
#include "graphical.h"
#include "object.h"
#include "geom_utils.h"
#include "console.h"


extern Window *imageWindow;
extern Point2i origin;
extern GeomObject *pointSet, *lineSet, *circleSet;

const char *invalidColor();

static inline float getRadius(const CircleObject circle) {
    return dist2f(circle.center->coord, circle.pt->coord);
}

static inline float sqrdist_lv(const Vector2f line_dir, const Vector2f vec) {
    return sqr_vec2(vec) - sqr(vec2_dot(vec, line_dir)) / sqr_vec2(line_dir);
}

static float sqrdist_lp(const GeomObject *line, const Point2f p) {
    const LineObject lineObj = line->ptr->line;
    const Vector2f vec1 = {p.x - lineObj.pt1->coord.x, p.y - lineObj.pt1->coord.y},
            vec2 = {p.x - lineObj.pt2->coord.x, p.y - lineObj.pt2->coord.y},
            lineDir = {lineObj.pt2->coord.x - lineObj.pt1->coord.x, lineObj.pt2->coord.y - lineObj.pt1->coord.y};

    if (line->type == LINE)
        return sqrdist_lv(lineDir, vec1);

    if (line->type == SEG)
        switch ((vec2_dot(vec1, lineDir) > 0.f) | ((vec2_dot(vec2, lineDir) > 0.f) << 1)) {
            case 1:
                return sqrdist_lv(lineDir, vec1);
            case 0:
                return sqrdist(lineObj.pt1->coord, p);
            case 3:
                return sqrdist(lineObj.pt2->coord, p);
            default:
                return 9.f;
        }

    switch ((vec2_dot(vec1, lineDir) > 0.f) | ((vec2_dot(vec2, lineDir) > 0.f) << 1)) {
        case 1:
        case 3:
            return sqrdist_lv(lineDir, vec1);
        case 0:
            return sqrdist(lineObj.pt1->coord, p);
        default:
            return 9.f;
    }
}

static void drawLineHelper(const LineObject line, const ObjectType type, const int color) {
    const Point2f pt1 = line.pt1->coord;
    const Point2f pt2 = line.pt2->coord;

    if (type == SEG) {
        drawLine(imageWindow, toImageCoord(pt1, origin), toImageCoord(pt2, origin), color, 2);
        return;
    }

    const int width = imageWindow->width;
    const int height = imageWindow->height;

    const Vector2f vec = {pt2.x - pt1.x, pt2.y - pt1.y};
    const Point2i imagePt = toImageCoord(pt1, origin);

    if (vec.x == 0.f || fabsf(vec.y / vec.x) > (float) height / width) {
        const float kxy = vec.x / vec.y;
        const int x0 = imagePt.x + (int) (imagePt.y * kxy);
        const int xh = imagePt.x - (int) ((height - imagePt.y) * kxy);
        if (type == LINE) {
            drawLine(imageWindow, (Point2i){x0, 0}, (Point2i){xh, height}, color, 2);
            return;
        }
        if (vec.y < 0) {
            drawLine(imageWindow, imagePt, (Point2i){xh, height}, color, 2);
        } else
            drawLine(imageWindow, imagePt, (Point2i){x0, 0}, color, 2);
        return;
    }

    const float kyx = vec.y / vec.x;
    const int yw = imagePt.y - (int) ((width - imagePt.x) * kyx);
    const int y0 = imagePt.y + (int) (imagePt.x * kyx);

    if (type == LINE) {
        drawLine(imageWindow, (Point2i){0, y0}, (Point2i){width, yw}, color, 2);
        return;
    }
    if (vec.x > 0) {
        drawLine(imageWindow, imagePt, (Point2i){width, yw}, color, 2);
    } else
        drawLine(imageWindow, imagePt, (Point2i){0, y0}, color, 2);
}

static void reflashBorad() {
    windowFill(imageWindow, 255, 255, 255);

    for (const GeomObject *cr = circleSet; cr != NULL; cr = cr->next)
        if (cr->show)
            showObject(cr, cr->color);
    for (const GeomObject *ln = lineSet; ln != NULL; ln = ln->next)
        if (ln->show)
            showObject(ln, ln->color);
    for (const GeomObject *pt = pointSet; pt != NULL; pt = pt->next)
        if (pt->show)
            showObject(pt, pt->color);
}


void showObject(const GeomObject *obj, int color) {
    if (!obj->show)
        return;

    if (color == -1)
        color = obj->color;

    const ObjectSelector *ptr = obj->ptr;
    switch (obj->type) {
        case POINT:
            drawPoint(imageWindow, toImageCoord(ptr->point->coord, origin), color);
            break;

        case CIRCLE:
            drawCircle(imageWindow, toImageCoord(ptr->circle.center->coord, origin),
                       getRadius(ptr->circle), color, 2);
            break;
        default:
            drawLineHelper(ptr->line, obj->type, color);
            break;
    }
}

GeomObject *mouseSelect(const int x, const int y) {
    const Point2f mouse = toMathCoord((Point2i){x, y}, origin);
    const float theshold = 25.f;

    for (GeomObject *pt = pointSet; pt != NULL; pt = pt->next)
        if (pt->show && sqrdist(mouse, pt->ptr->point->coord) < theshold)
            return pt;

    for (GeomObject *ln = lineSet; ln != NULL; ln = ln->next)
        if (ln->show && sqrdist_lp(ln, mouse) < theshold)
            return ln;

    for (GeomObject *cr = circleSet; cr != NULL; cr = cr->next)
        if (cr->show && dist2f(mouse, cr->ptr->circle.center->coord) - getRadius(cr->ptr->circle) < 5.f)
            return cr;

    return NULL;
}

int show(const int argc, const char **argv) {
    if (argc == 1)
        return throwError(ERROR_NO_ARG_GIVEN, noArgGiven(*argv));

    const unsigned long long id = strhash64(argv[1]);
    GeomObject *obj = findObject(ANY, id);

    if (obj == NULL)
        return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(argv[1]));

    if (argc == 2) {
        obj->show = 1;
        showObject(obj, obj->color);
        return 0;
    }

    char *end;
    const int color = strtol(argv[2], &end, 16);
    if (*end != '\0')
        return throwError(ERROR_INVALID_ARG, invalidColor());

    obj->show = 1;
    obj->color = color;
    showObject(obj, color);
    return 0;
}

int hide(const int argc, const char **argv) {
    if (argc == 1)
        return throwError(ERROR_NO_ARG_GIVEN, noArgGiven(*argv));

    const uint64_t id = strhash64(argv[1]);
    GeomObject *obj = findObject(ANY, id);

    if (obj == NULL)
        return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(argv[1]));

    obj->show = 0;
    reflashBorad();
    return 0;
}
