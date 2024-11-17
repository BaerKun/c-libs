#include "object.h"
#include "random.h"
#include "console.h"
#include "geom_utils.h"
#include "geom_errors.h"
#include "board.h"

#include <stdlib.h>
#include <string.h>


// private
static PointObject *pointDataSet = NULL;
GeomObject *pointSet = NULL, *lineSet = NULL, *circleSet = NULL;


static int getArgs(ObjectType type, const char *str1, const char *str2, ObjectSelector *arg);

static PointObject *createPointData(Point2f pt, PointObject **parents, int numParents, Point2f (*derive)(PointObject *));

static GeomObject *createGeomObject(ObjectType type, const ObjectSelector *arg, uint64_t id, int show, int rgb);

static int getOptionalObjectArgs(const char **argv, const char **endptr, int64_t *id, int *show, int *rgb);

static GeomObject *findObjectHelper(GeomObject *head, uint64_t id);

static Point2f midpointCallback(const PointObject *pt);


// public
const char *invalidColor() {
    static const char *tips = "Please use hexadecimal.";
    static char error[40] = {0};

    if (*error == 0)
        strcpy(error, invalidArg("color", tips));
    return error;
}

GeomObject *findObject(const ObjectType type, const uint64_t id) {
    switch (type) {
            GeomObject *obj;
        case POINT:
            return findObjectHelper(pointSet, id);
        case CIRCLE:
            return findObjectHelper(circleSet, id);
        case ANY:
            obj = findObjectHelper(pointSet, id);
            if (obj != NULL)
                return obj;
            obj = findObjectHelper(lineSet, id);
            if (obj != NULL)
                return obj;
            obj = findObjectHelper(circleSet, id);
            return obj;
        default:
            obj = findObjectHelper(lineSet, id);
            if (obj != NULL && obj->type != type)
                return NULL;
            return obj;
    }
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
        const char *arg1 = ++argv == endptr ? NULL : *argv;
        const char *arg2 = ++argv == endptr ? NULL : *argv;
        const int error = getArgs(type, arg1, arg2, &arg);
        if (error != 0)
            return error;
    }

    int64_t id;
    int show, rgb;
    const int error = getOptionalObjectArgs(++argv, endptr, &id, &show, &rgb);
    if (error != 0)
        return error;

    const GeomObject *newObj = createGeomObject(type, &arg, id, show, rgb);

    if (show && newObj != NULL)
        showObject(newObj, rgb);
    return 0;
}

int midpoint(const int argc, const char **argv) {
    const char **endptr = argv + argc;
    if (argc == 1)
        return throwError(ERROR_NO_ARG_GIVEN, noArgGiven(*argv));

    const GeomObject *pt2,
            *pt1 = findObject(POINT, strhash64(argv[1]));
    if (pt1 == NULL)
        return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(argv[1]));
    if (argc >= 3) {
        pt2 = findObject(POINT, strhash64(argv[2]));
        if (pt2 == NULL)
            return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(argv[2]));
    } else {
        return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg(*argv));
    }

    int64_t id;
    int show, rgb;
    const int error = getOptionalObjectArgs(argv + 3, endptr, &id, &show, &rgb);
    if (error != 0)
        return error;

    PointObject *parents[2] = {pt1->ptr->point, pt2->ptr->point};
    const PointObject *mid = createPointData(midpt(pt1->ptr->point->coord, pt2->ptr->point->coord),
                                             parents, 2, midpointCallback);

    const GeomObject *newObj = createGeomObject(POINT, (ObjectSelector *) &mid, id, show, rgb);
    if (show && newObj != NULL)
        showObject(newObj, rgb);

    return 0;
}


// private
static Point2f midpointCallback(const PointObject *pt) {
    return midpt(pt[0].coord, pt[1].coord);
}


static GeomObject *findObjectHelper(GeomObject *head, const uint64_t id) {
    while (head != NULL) {
        if (head->id == id)
            break;
        head = head->next;
    }
    return head;
}

static PointObject *createPointData(const Point2f pt, PointObject **parents, const int numParents,
                                    Point2f (*derive)(PointObject *)) {
    PointObject *obj = malloc(sizeof(PointObject) + sizeof(PointObject *) * numParents);
    obj->next = pointDataSet;
    pointDataSet = obj;

    obj->coord = pt;
    obj->derive = derive;

    if (numParents == 0)
        return obj;

    for (int i = 0; i < numParents; ++i) {
        PointObject *parent = parents[i];
        SubPoint *subpt = malloc(sizeof(SubPoint));

        *subpt = (SubPoint){obj, parent->children};
        parent->children = subpt;
        obj->parents[i] = parent;
    }

    return obj;
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
    obj->color = rgb;

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
        default:
            break;
    }

    return obj;
}


static inline int randomColor() {
    return pcg32_random() & 0xffffff;
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

static void setRadius(CircleObject *obj, const float radius) {
    obj->pt = createPointData((Point2f){obj->center->coord.x + radius, obj->center->coord.y}, NULL, 0, NULL);
}

static int getPointArg(const char *str1, const char *str2, PointObject **arg) {
    char *end;

    if (str1 == NULL)
        return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg("create"));
    const float x = strtof(str1, &end);
    if (*end != '\0')
        return throwError(ERROR_INVALID_ARG, invalidArg("x-coord", NULL));

    if (str2 == NULL)
        return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg("create"));
    const float y = strtof(str2, &end);
    if (*end != '\0')
        return throwError(ERROR_INVALID_ARG, invalidArg("y-coord", NULL));

    *arg = createPointData((Point2f){x, y}, NULL, 0, NULL);
    return 0;
}

static int getLineArg(const char *str1, const char *str2, LineObject *arg) {
    if (str1 == NULL) return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg("create"));
    const uint64_t id1 = strhash64(str1);
    const GeomObject *obj = findObjectHelper(pointSet, id1);
    if (obj == NULL)
        return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(str1));
    arg->pt1 = obj->ptr->point;

    if (str2 == NULL) return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg("create"));
    const uint64_t id2 = strhash64(str2);
    obj = findObjectHelper(pointSet, id2);
    if (obj == NULL)
        return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(str2));
    arg->pt2 = obj->ptr->point;

    return 0;
}

static int getCircleArg(const char *str1, const char *str2, CircleObject *arg) {
    if (str1 == NULL) return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg("create"));
    const uint64_t id1 = strhash64(str1);
    const GeomObject *obj = findObjectHelper(pointSet, id1);
    if (obj == NULL)
        return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(str1));
    arg->center = obj->ptr->point;

    if (str2 == NULL) return throwError(ERROR_NOT_ENOUGH_ARG, notEnoughArg("create"));
    if (*str2 >= '0' && *str2 <= '9') {
        char *end;
        const float radus = strtof(str2, &end);
        if (*end != '\0')
            return throwError(ERROR_INVALID_ARG, invalidArg("radius", NULL));

        setRadius(arg, radus);
        return 0;
    }

    const uint64_t id2 = strhash64(str2);
    obj = findObjectHelper(pointSet, id2);
    if (obj == NULL)
        return throwError(ERROR_NOT_FOUND_OBJECT, objectNotFound(str2));
    arg->pt = obj->ptr->point;

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

static int getOptionalObjectArgs(const char **argv, const char **endptr, int64_t *id, int *show, int *rgb) {
    *id = 0;
    *show = 1;
    *rgb = -1;
    while (argv != endptr) {
        const char *end;
        switch (strhash64(*argv)) {
            case STR_HASH64('a', 's', 0, 0, 0, 0, 0, 0):
                if (++argv == endptr)
                    break;
                *id = strhash64(*argv++);
                break;

            case STR_HASH64('-', '-', 's', 'h', 'o', 'w', 0, 0):
                if (++argv == endptr)
                    break;
                *show = strtobool(*argv++, &end);
                if (*end != '\0')
                    return throwError(ERROR_INVALID_ARG, invalidArg("show", "Please true/false"));
                break;

            case STR_HASH64('-', '-', 'c', 'o', 'l', 'o', 'r', 0):
                if (++argv == endptr)
                    break;
                *rgb = strtol(*argv++, (char **) &end, 16);
                if (*end != '\0')
                    return throwError(ERROR_INVALID_ARG, invalidColor());
                break;

            default:
                return throwError(ERROR_UNKOWN_ARG, unkownArg(*argv));
        }
    }
    if (*id == 0)
        *id = getDefaultId();
    if (*rgb)
        *rgb = randomColor();
    return 0;
}
