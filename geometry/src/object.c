#include "object.h"
#include "random.h"
#include "console.h"
#include <stdlib.h>
#include "geom_utils.h"
#include "../../arithmetic/include/auto_diff.h"

#define MAX_OBJECTS 64

extern Window *imageWindow;
extern Point2i origin;

static GeomObject *pointSet = NULL, *lineSet = NULL, *circleSet = NULL;
static const char noArgGiven[] = "No arguments given.";
static const char tooFewArgs[] = "Not enough arguments.";
static const char invalidColor[] = "Invalid color argument. Please hexadecimal.";

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

static GeomObject *findObject(GeomObject *head, const uint64_t id) {
    while (head != NULL && id != head->id)
        head = head->next;
    return head;
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

static GeomObject *createGeomObject(const ObjectType type, const ObjectSelector *arg, const uint64_t id, const int show,
                                    const int rgb) {
    GeomObject *obj = getNewObject(type);
    if (obj == NULL)
        return NULL;

    obj->id = id;
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

static char *objectNotFoundError(const char *name) {
    static char error[32] = "Object not found: ";
    strmove8(name, error + 18);
    return error;
}

static char *getPointArg(const char *str1, const char *str2, PointObject *arg) {
    char *end;

    if (str1 == NULL) return tooFewArgs;
    arg->coord.x = strtof(str1, &end);
    if (*end != '\0')
        return "Invalid x coordinate.";

    if (str2 == NULL) return tooFewArgs;
    arg->coord.y = strtof(str2, &end);
    if (*end != '\0')
        return "Invalid y coordinate.";

    arg->numCite = 0;
    arg->fixed = 0;
    return NULL;
}

static char *getLineArg(const char *str1, const char *str2, LineObject *arg) {
    if (str1 == NULL) return tooFewArgs;
    const uint64_t id1 = strhash64(str1);
    arg->pt1 = (PointObject *) findObject(pointSet, id1)->ptr;
    if (arg->pt1 == NULL)
        return objectNotFoundError(str1);

    if (str2 == NULL) return tooFewArgs;
    const uint64_t id2 = strhash64(str2);
    arg->pt2 = (PointObject *) findObject(pointSet, id2)->ptr;
    if (arg->pt2 == NULL)
        return objectNotFoundError(str2);

    return NULL;
}

static char *getCircleArg(const char *str1, const char *str2, CircleObject *arg) {
    if (str1 == NULL) return tooFewArgs;
    const uint64_t id1 = strhash64(str1);
    arg->center = (PointObject *) findObject(pointSet, id1)->ptr;
    if (arg->center == NULL)
        return objectNotFoundError(str1);

    if (str2 == NULL) return tooFewArgs;
    if (*str2 >= '0' && *str2 <= '9') {
        char *end;

        arg->pt = NULL;
        arg->radus = strtof(str2, &end);
        return *end != '\0' ? "Invalid radius." : NULL;
    }

    const uint64_t id2 = strhash64(str2);
    arg->pt = (PointObject *) findObject(pointSet, id2)->ptr;
    if (arg->pt == NULL)
        return objectNotFoundError(str2);

    return NULL;
}

static uint64_t getDefaultId() {
    static uint64_t id = STR_HASH64('#', '0', '0', '0', 0, 0, 0, 0);
    char *c = (char *) &id + 3;
    do {
        if (*c != '9') {
            ++*c;
            return id;
        }
        *c = '0';
        --c;
    } while (*c != '#');

    return 0;
}

static void showObject(const GeomObject *obj, int color) {
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

static void hideObject(GeomObject *obj) {
    windowFill(imageWindow, 255, 255, 255);
    obj->show = 0;

    for (const GeomObject *pt = pointSet; pt != NULL; pt = pt->next)
        if (pt->show)
            showObject(pt, pt->color);
    for (const GeomObject *ln = lineSet; ln != NULL; ln = ln->next)
        if (ln->show)
            showObject(ln, ln->color);
    for (const GeomObject *cr = circleSet; cr != NULL; cr = cr->next)
        if (cr->show)
            showObject(cr, cr->color);
}

char *create(const int argc, const char **argv) {
    const char **endptr = argv + argc;
    if (++argv == endptr)
        return noArgGiven;

    ObjectType type;
    ObjectSelector arg; {
        char *error = NULL;
        const uint64_t hashType = strhash64(*argv);
        const char *str1 = ++argv == endptr ? NULL : *argv,
                *str2 = ++argv >= endptr ? NULL : *argv;
        switch (hashType) {
            case STR_HASH64('p', 'o', 'i', 'n', 't', 0, 0, 0):
                type = POINT;
                error = getPointArg(str1, str2, &arg.point);
                break;
            case STR_HASH64('l', 'i', 'n', 'e', 0, 0, 0, 0):
                type = LINE;
                error = getLineArg(str1, str2, &arg.line);
                break;
            case STR_HASH64('r', 'a', 'y', 0, 0, 0, 0, 0):
                type = RAY;
                error = getLineArg(str1, str2, &arg.line);
                break;
            case STR_HASH64('s', 'e', 'g', 0, 0, 0, 0, 0):
                type = SEG;
                error = getLineArg(str1, str2, &arg.line);
                break;
            case STR_HASH64('c', 'i', 'r', 'c', 'l', 'e', 0, 0):
                type = CIRCLE;
                error = getCircleArg(str1, str2, &arg.circle);
                break;
            case STR_HASH64('h', 'e', 'l', 'p', 0, 0, 0, 0):
                return "Usage: create <object> <arg1> <arg2> [show_if] [color_rgb] [as <name>]";
            default:
                return "Invalid object type.";
        }
        if (error != NULL)
            return error;
    }

    int show = 1, rgb = -1;
    long long id = 0;

    ++argv;
    while (argv != endptr) {
        char *end;
        switch (strhash64(*argv)) {
            case STR_HASH64('a', 's', 0, 0, 0, 0, 0, 0):
                if (++argv == endptr)
                    break;
                id = strhash64(*argv++);
                break;
            case STR_HASH64('-', '-', 's', 'h', 'o', 'w', 0, 0):
                if (++argv == endptr)
                    break;
                show = strtobool(*argv++, &end);
                if (*end != '\0')
                    return "Invalid show argument. Please true / false";
                break;
            case STR_HASH64('-', '-', 'c', 'o', 'l', 'o', 'r', 0):
                if (++argv == endptr)
                    break;
                rgb = strtol(*argv++, &end, 16);
                if (*end != '\0')
                    return invalidColor;
                break;
            default:
                return "Invalid argument.";
        }
    }

    if (id == 0)
        id = getDefaultId();

    const GeomObject *newObj = createGeomObject(type, &arg, id, show, rgb);

    if (show && newObj != NULL)
        showObject(newObj, rgb);

    return NULL;
}

char *show(const int argc, const char **argv) {
    if (argc == 1)
        return noArgGiven;

    const uint64_t id = strhash64(argv[1]);
    GeomObject *obj = findObject(pointSet, id);
    if (obj == NULL) {
        obj = findObject(lineSet, id);
        if (obj == NULL)
            obj = findObject(circleSet, id);
    }

    if (obj == NULL)
        return objectNotFoundError(argv[1]);

    if (argc == 2) {
        obj->show = 1;
        showObject(obj, obj->color);
        return NULL;
    }

    char *end;
    const int color = strtol(argv[2], &end, 16);
    if (*end != '\0')
        return invalidColor;

    obj->show = 1;
    obj->color = color;
    showObject(obj, color);
    return NULL;
}

char *hide(const int argc, const char **argv) {
    if (argc == 1)
        return noArgGiven;

    const uint64_t id = strhash64(argv[1]);
    GeomObject *obj = findObject(pointSet, id);
    if (obj == NULL) {
        obj = findObject(lineSet, id);
        if (obj == NULL)
            obj = findObject(circleSet, id);
    }

    if (obj == NULL)
        return objectNotFoundError(argv[1]);

    hideObject(obj);
    return NULL;
}
