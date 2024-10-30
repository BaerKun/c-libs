#include "object.h"
#include "random.h"
#include "console.h"
#include <stdlib.h>

#define MAX_OBJECTS 64

extern Window *imageWindow;
extern Point2i origin;

GeomObject *pointSet = NULL, *lineSet = NULL, *circleSet = NULL;

static inline int randomColor() {
    return pcg32_random() & 0xffffff;
}

static inline float getRadius(const CircleObject circle) {
    return circle.pt == NULL ? circle.radus : dist2f(circle.center->coord, circle.pt->coord);
}

static void strmove8(const char *src, char *dst) {
    for (int i = 0; i < 8 && *src != '\0'; ++i) {
        *dst = *src;
        ++dst, ++src;
    }
    *dst = '\0';
}

static GeomObject *getNewObject(const ObjectType type) {
    GeomObject *obj;
    switch (type) {
        case POINT:
            obj = malloc(sizeof(GeomObject) + sizeof(PointObject));
            obj->next = pointSet;
            pointSet = obj;
            return obj;
        case CIRCLE:
            obj = malloc(sizeof(GeomObject) + sizeof(CircleObject));
            obj->next = circleSet;
            circleSet = obj;
            return obj;
        case LINE:
        case RAY:
        case SEG:
            obj = malloc(sizeof(GeomObject) + sizeof(LineObject));
            obj->next = lineSet;
            lineSet = obj;
            return obj;
        default:
            return NULL;
    }
}

static GeomObject *findObject(GeomObject *head, const uint64_t id) {
    while (head != NULL && id != head->id)
        head = head->next;
    return head;
}

GeomObject *createGeomObject(const ObjectType type, const char *name, const ObjectSelector *arg, const int show,
                             const int rgb) {
    GeomObject *obj = getNewObject(type);
    if (obj == NULL)
        return NULL;

    obj->id = strhash64(name);
    obj->type = type;
    obj->show = show;
    obj->color = rgb == -1 ? randomColor() : rgb;

    switch (type) {
        case POINT:
            obj->ptr->point = arg->point;
            break;
        case CIRCLE:
            obj->ptr->circle = arg->circle;
            break;
        case LINE:
        case RAY:
        case SEG:
            obj->ptr->line = arg->line;
    }

    return obj;
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

    if (vec.x == 0.f || fabs(vec.y / vec.x) > (float) height / width) {
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

void showObject(const GeomObject *obj, int color) {
    if (!obj->show)
        return;

    if (color == -1)
        color = obj->color;

    const ObjectSelector *ptr = obj->ptr;
    switch (obj->type) {
        case POINT:
            drawPoint(imageWindow, toImageCoord(ptr->point.coord, origin), color);
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

char *getPointArg(const char *str1, const char *str2, PointObject *arg) {
    arg->numCite = 0;
    arg->fixed = 0;

    char *end;
    arg->coord.x = strtof(str1, &end);
    if (*end != '\0')
        return "Invalid x coordinate.";

    arg->coord.y = strtof(str2, &end);
    return *end != '\0' ? "Invalid y coordinate." : NULL;
}

char *objectNotFoundError(const char *name) {
    static char error[32] = "Object not found: ";
    strmove8(name, error + 19);
    return error;
}

char *getLineArg(const char *str1, const char *str2, LineObject *arg) {
    const uint64_t id1 = strhash64(str1);
    arg->pt1 = findObject(pointSet, id1)->ptr;
    if (arg->pt1 == NULL)
        return objectNotFoundError(str1);

    const uint64_t id2 = strhash64(str2);
    arg->pt2 = findObject(pointSet, id2)->ptr;
    if (arg->pt2 == NULL)
        return objectNotFoundError(str2);

    return NULL;
}

char *getCircleArg(const char *str1, const char *str2, CircleObject *arg) {
    const uint64_t id1 = strhash64(str1);
    arg->center = findObject(pointSet, id1)->ptr;
    if (arg->center == NULL)
        return objectNotFoundError(str1);

    if (*str2 >= '0' && *str2 <= '9') {
        char *end;

        arg->pt = NULL;
        arg->radus = strtof(str2, &end);
        return *end != '\0' ? "Invalid radius." : NULL;
    }

    const uint64_t id2 = strhash64(str2);
    arg->pt = findObject(pointSet, id2)->ptr;
    if (arg->pt == NULL)
        return objectNotFoundError(str2);

    return NULL;
}

char *create(int argc, const char **argv) {
    if (argc == 1)
        return "No arguments given.";

    if (argc <= 3)
        return "Not enough arguments.";

    char *error = NULL;
    ObjectType type;
    ObjectSelector arg;
    switch (strhash64(argv[1])) {
        case STR_HASH64('p', 'o', 'i', 'n', 't', 0, 0, 0):
            type = POINT;
            error = getPointArg(argv[2], argv[3], &arg.point);
            break;
        case STR_HASH64('l', 'i', 'n', 'e', 0, 0, 0, 0):
            type = LINE;
            error = getLineArg(argv[2], argv[3], &arg.line);
            break;
        case  STR_HASH64('r', 'a', 'y', 0, 0, 0, 0, 0):
            type = RAY;
            error = getLineArg(argv[2], argv[3], &arg.line);
            break;
        case STR_HASH64('s', 'e', 'g', 0, 0, 0, 0, 0):
            type = SEG;
            error = getLineArg(argv[2], argv[3], &arg.line);
            break;
        case STR_HASH64('c', 'i', 'r', 'c', 'l', 'e', 0, 0):
            type = CIRCLE;
            error = getCircleArg(argv[2], argv[3], &arg.circle);
            break;
        case STR_HASH64('h', 'e', 'l', 'p', 0, 0, 0, 0):
            return "Usage: create <object> <args> [show_if] [color_rgb]";
        default:
            return "Invalid object type.";
    }

    if (error != NULL)
        return error;

    int show = 1, rgb = -1;
    static char *name = "#0";
    const GeomObject *newObj = createGeomObject(type, name, &arg, show, rgb);
    ++name[1];
    if (show)
        showObject(newObj, -1);

    return NULL;
}
