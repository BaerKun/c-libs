#include "object.h"
#include "random.h"
#include "console.h"
#include <stdlib.h>

#define MAX_OBJECTS 64

extern Window *imageWindow;
extern Point2i origin;
GeomObject objects[MAX_OBJECTS];

static inline int randomColor() {
    return pcg32_random() & 0xffffff;
}

static GeomObject *getNewObject() {
    static int index = 0;
    if (index == MAX_OBJECTS)
        return NULL;
    return objects + index++;
}

GeomObject *createGeomObject(const ObjectType type, const float arg1, const float arg2, const float arg3,
                             const int show, const int rgb) {
    GeomObject *obj = getNewObject();
    if (obj != NULL) {
        obj->type = type;
        obj->show = show;
        obj->color = rgb == -1 ? randomColor() : rgb;
        obj->object.line = (Line){arg1, arg2, arg3};
    }
    return obj;
}

static void drawLineHelper(const Line line, const int color) {
    const int width = imageWindow->width;
    const int height = imageWindow->height;
    if (line.B == 0.f || fabs(line.A / line.B) > (float) height / width) {
        const int x = origin.x - (int) (line.C / line.A);
        const int dx = height / 2 * line.B / line.A;
        drawLine(imageWindow, (Point2i){x - dx, 0}, (Point2i){x + dx, height}, color, 2);
        return;
    }

    const int y = origin.y - (int) (line.C / line.B);
    const int dy = width / 2 * line.A / line.B;
    drawLine(imageWindow, (Point2i){0, y + dy}, (Point2i){width, y - dy}, color, 2);
}

void showObject(const GeomObject *obj, int color) {
    if (!obj->show)
        return;

    if (color == -1)
        color = obj->color;

    switch (obj->type) {
        case POINT:
            drawPoint(imageWindow, toImageCoord(obj->object.point, origin), color);
            break;
        case LINE:
            drawLineHelper(obj->object.line, color);
            break;
        case CIRCLE:
            drawCircle(imageWindow, toImageCoord(obj->object.circle.c, origin), obj->object.circle.r, color, 2);
    }
}


char *create(int argc, const char **argv) {
    if (--argc == 0)
        return "No arguments given.";

    ObjectType type;
    switch (strhash64(argv[1])) {
        case STR_HASH64('c', 'i', 'r', 'c', 'l', 'e', 0, 0):
            type = CIRCLE;
            break;
        case STR_HASH64('l', 'i', 'n', 'e', 0, 0, 0, 0):
            type = LINE;
            break;
        case STR_HASH64('p', 'o', 'i', 'n', 't', 0, 0, 0):
            type = POINT;
            break;
        case STR_HASH64('h', 'e', 'l', 'p', 0, 0, 0, 0):
            return "Usage: create <object> <args> [show_if] [color_rgb]";
        default:
            return "Invalid object type.";
    }

    static const char tooFewArg[] = "Not enough arguments for points.";
    if (--argc == 0)
        return tooFewArg;

    char *end, **arg = argv + 2;

    const float arg1 = strtof(*arg++, &end);
    if (*end != '\0')
        return "Invalid first argument.";
    if (--argc == 0)
        return tooFewArg;

    const float arg2 = strtof(*arg++, &end);
    if (*end != '\0')
        return "Invalid second argument.";

    float arg3 = 0.f;
    if (type != POINT) {
        if (--argc == 0)
            return tooFewArg;

        arg3 = strtof(*arg++, &end);
        if (*end != '\0')
            return "Invalid third argument.";
    }

    int show = 1, rgb = -1;
    if (--argc != 0) {
        show = strtobool(*arg++, &end);
        if (*end != '\0')
            return "Invalid show value. Please true/false";

        if (--argc != 0) {
            rgb = strtol(*arg++, &end, 16);
            if (*end != '\0')
                return "Invalid color value. Please give a hex value.";
            --argc;
        }
    }

    if (argc != 0)
        return "Too many arguments.";

    const GeomObject *newObj = createGeomObject(type, arg1, arg2, arg3, show, rgb);
    if (newObj == NULL)
        return "Too many objects.";

    if (show)
        showObject(newObj, -1);

    return NULL;
}
