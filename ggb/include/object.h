#ifndef OBJECT_H
#define OBJECT_H

#include "geometry.h"

typedef enum {
    ANY, POINT, CIRCLE, LINE, RAY, SEG
} ObjectType;

typedef struct PointObject_ PointObject;
typedef struct LineObject_ LineObject;
typedef struct CircleObject_ CircleObject;
typedef struct GeomObject_ GeomObject;
typedef union ObjectSelector_ ObjectSelector;
typedef struct SubPoint_ SubPoint;

struct SubPoint_ {
    PointObject *pt;
    SubPoint *next;
};

struct PointObject_ {
    Point2f coord;
    SubPoint *children;
    PointObject *next;

    Point2f (*derive)(PointObject *);

    PointObject *parents[0];
};

struct LineObject_ {
    PointObject *pt1, *pt2;
};

struct CircleObject_ {
    PointObject *center, *pt;
};

union ObjectSelector_ {
    PointObject *point;
    LineObject line;
    CircleObject circle;
};

struct GeomObject_ {
    unsigned long long id;
    int show, color;
    ObjectType type;
    GeomObject *next;
    ObjectSelector ptr[0];
};

GeomObject *findObject(ObjectType type, unsigned long long id);

int create(int argc, const char **argv);

int midpoint(int argc, const char **argv);

#endif //OBJECT_H
