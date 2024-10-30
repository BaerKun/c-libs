#ifndef OBJECT_H
#define OBJECT_H

#include "geom_utils.h"
#include "graphical.h"

typedef enum {
    POINT, CIRCLE, LINE, RAY, SEG
} ObjectType;

typedef struct PointObject_ PointObject;
typedef struct LineObject_ LineObject;
typedef struct CircleObject_ CircleObject;
typedef struct GeomObject_ GeomObject;
typedef union ObjectSelector_ ObjectSelector;

struct PointObject_ {
    Point2f coord;
    int fixed;
    int numCite;
};

struct LineObject_ {
    PointObject *pt1, *pt2;
};

struct CircleObject_ {
    PointObject *center, *pt;
    float radus;
};

union ObjectSelector_ {
    PointObject point;
    LineObject line;
    CircleObject circle;
};

struct GeomObject_ {
    long long id;
    int show, color;
    ObjectType type;
    GeomObject *next;
    ObjectSelector ptr[0];
};





char *create(int argc, const char **argv);

#endif //OBJECT_H
