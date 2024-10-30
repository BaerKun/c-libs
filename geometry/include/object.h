#ifndef OBJECT_H
#define OBJECT_H

#include "geom_utils.h"
#include "graphical.h"

typedef enum {
    POINT, LINE, CIRCLE
} ObjectType;

typedef struct {
    float A, B, C;
} Line;

typedef struct {
    Point2f c;
    float r;
} Circle;

typedef struct {
    ObjectType type;

    union {
        Point2f point;
        Line line;
        Circle circle;
    } object;

    int show;
    int color;
} GeomObject;

char *create(int argc, const char **argv);

#endif //OBJECT_H
