#include "object.h"
#include "random.h"
#include "console.h"
#include "geom_utils.h"
#include "geom_errors.h"

#include <stdlib.h>
#include <string.h>

extern Window *imageWindow;
extern Point2i origin;

// private
static GeomObject *pointSet = NULL, *lineSet = NULL, *circleSet = NULL;

static uint64_t getDefaultId();

static GeomObject *findObject(GeomObject *head, uint64_t id);

static const char *invalidColor();

static float sqrdist_lp(const GeomObject *line, Point2f p);

static int getArgs(ObjectType type, const char *str1, const char *str2, ObjectSelector *arg);

static void showObject(const GeomObject *obj, int color);

static void hideObject(GeomObject *obj);

static GeomObject *createGeomObject(ObjectType type, const ObjectSelector *arg, uint64_t id, int show, int rgb);


// public
const GeomObject *mouseSelect(const int x, const int y) {
    const Point2f mouse = toMathCoord((Point2i){x, y}, origin);
    const float theshold = 25.f;

    for (const GeomObject *pt = pointSet; pt != NULL; pt = pt->next)
        if (pt->show && sqrdist(mouse, pt->ptr->point.coord) < theshold)
            return pt;

    for (const GeomObject *ln = lineSet; ln != NULL; ln = ln->next)
        if (ln->show && sqrdist_lp(ln, mouse) < theshold)
            return ln;

    for (const GeomObject *cr = circleSet; cr != NULL; cr = cr->next)
        if (cr->show && dist2f(mouse, cr->ptr->circle.center->coord) - cr->ptr->circle.radus < 5.f)
            return cr;

    return NULL;
}

int create(const int argc, const char **argv) {
    const char **endptr = argv + argc;
    if (argc == 1)
        return throwError(ERROR_NO_ARG_GIVEN, noArgGiven(*argv));

    ObjectType type;
    ObjectSelector arg;
    switch (strhash64(*++argv)) {
        case STR_HASH64('p', 'o', 'i', 'n', 't', 0, 0, 0):
            type = POINT;
            break;
        case STR_HASH64('l', 'i', 'n', 'e', 0, 0, 0, 0):
            type = LINE;
            break;
        case STR_HASH64('r', 'a', 'y', 0, 0, 0, 0, 0):
            type = RAY;
            break;
        case STR_HASH64('s', 'e', 'g', 0, 0, 0, 0, 0):
            type = SEG;
            break;
        case STR_HASH64('c', 'i', 'r', 'c', 'l', 'e', 0, 0):
            type = CIRCLE;
            break;
        case STR_HASH64('h', 'e', 'l', 'p', 0, 0, 0, 0):
            return throwError(ERROR_HELP, "Usage: create <object> <arg1> <arg2> [as <name>]");
        default:
            return throwError(ERROR_INVALID_ARG, invalidArg("object-type", "Please point/line/ray/seg/circle"));
    } {
        const char *arg1 = ++argv == endptr ? NULL : *argv,
                *arg2 = ++argv == endptr ? NULL : *argv;
        const int error = getArgs(type, arg1, arg2, &arg);
        if (error != 0)
            return error;
    }

    int show = 1, rgb = -1;
    uint64_t id = 0;

    ++argv;
    while (argv != endptr) {
        const char *end;
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
                    return throwError(ERROR_INVALID_ARG, invalidArg("show", "Please true/false"));
                break;

            case STR_HASH64('-', '-', 'c', 'o', 'l', 'o', 'r', 0):
                if (++argv == endptr)
                    break;
                rgb = strtol(*argv++, (char **) &end, 16);
                if (*end != '\0')
                    return throwError(ERROR_INVALID_ARG, invalidColor());
                break;

            default:
                return throwError(ERROR_UNKOWN_ARG, unkownArg(*argv));
        }
    }

    if (id == 0)
        id = getDefaultId();

    const GeomObject *newObj = createGeomObject(type, &arg, id, show, rgb);

    if (show && newObj != NULL)
        showObject(newObj, rgb);
    return 0;
}

int show(const int argc, const char **argv) {
    if (argc == 1)
        return throwError(ERROR_NO_ARG_GIVEN, noArgGiven(*argv));

    const uint64_t id = strhash64(argv[1]);
    GeomObject *obj = findObject(pointSet, id);
    if (obj == NULL) {
        obj = findObject(lineSet, id);
        if (obj == NULL)
            obj = findObject(circleSet, id);
    }

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
    GeomObject *obj = findObject(pointSet, id);
    if (obj == NULL) {
        obj = findObject(lineSet, id);
        if (obj == NULL)
            obj = findObject(circleSet, id);
    }

    if (obj == NULL)
        return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(argv[1]));

    hideObject(obj);
    return 0;
}


// private
static inline int randomColor() {
    return pcg32_random() & 0xffffff;
}

static inline float getRadius(const CircleObject circle) {
    return circle.pt == NULL ? circle.radus : dist2f(circle.center->coord, circle.pt->coord);
}

static const char *invalidColor() {
    const char *tips = "Please use hexadecimal.";
    static char error[40] = {0};

    if (*error == 0)
        strcpy(error, invalidArg("color", tips));
    return error;
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


static int getPointArg(const char *str1, const char *str2, PointObject *arg) {
    char *end;

    if (str1 == NULL) return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg("create"));
    arg->coord.x = strtof(str1, &end);
    if (*end != '\0')
        return throwError(ERROR_INVALID_ARG, invalidArg("x-coord", NULL));

    if (str2 == NULL) return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg("create"));
    arg->coord.y = strtof(str2, &end);
    if (*end != '\0')
        return throwError(ERROR_INVALID_ARG, invalidArg("y-coord", NULL));

    arg->firstChild = NULL;
    arg->derive = NULL;
    return 0;
}

static int getLineArg(const char *str1, const char *str2, LineObject *arg) {
    if (str1 == NULL) return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg("create"));
    const uint64_t id1 = strhash64(str1);
    const GeomObject *obj = findObject(pointSet, id1);
    if (obj == NULL)
        return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(str1));
    arg->pt1 = (PointObject *) obj->ptr;

    if (str2 == NULL) return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg("create"));
    const uint64_t id2 = strhash64(str2);
    obj = findObject(pointSet, id2);
    if (obj == NULL)
        return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(str2));
    arg->pt2 = (PointObject *) obj->ptr;

    return 0;
}

static int getCircleArg(const char *str1, const char *str2, CircleObject *arg) {
    if (str1 == NULL) return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg("create"));
    const uint64_t id1 = strhash64(str1);
    const GeomObject *obj = findObject(pointSet, id1);
    if (obj == NULL)
        return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(str1));
    arg->center = (PointObject *) obj->ptr;

    if (str2 == NULL) return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg("create"));
    if (*str2 >= '0' && *str2 <= '9') {
        char *end;
        arg->pt = NULL;
        arg->radus = strtof(str2, &end);
        if (*end != '\0')
            return throwError(ERROR_INVALID_ARG, invalidArg("radius", NULL));
        return 0;
    }

    const uint64_t id2 = strhash64(str2);
    obj = findObject(pointSet, id2);
    if (obj == NULL)
        return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(str2));
    arg->pt = (PointObject *) obj->ptr;

    return 0;
}

static int getArgs(const ObjectType type, const char *str1, const char *str2, ObjectSelector *arg) {
    switch (type) {
        case POINT:
            return getPointArg(str1, str2, &arg->point);
        case CIRCLE:
            return getCircleArg(str1, str2, &arg->circle);
        default:
            return getLineArg(str1, str2, &arg->line);
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


static float sqrdist_lv(const Vector2f line_dir, const Vector2f vec) {
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
