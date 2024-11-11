#ifndef OBJECT_H
#define OBJECT_H

#include "graphical.h"

typedef enum {
    POINT, CIRCLE, LINE, RAY, SEG
} ObjectType;

typedef struct PointObject_ PointObject;
typedef struct LineObject_ LineObject;
typedef struct CircleObject_ CircleObject;
typedef struct GeomObject_ GeomObject;
typedef union ObjectSelector_ ObjectSelector;
typedef struct SubPointList_ SubPointList;

struct SubPointList_ {
    PointObject *pt;
    SubPointList *next;
};

struct PointObject_ {
    Point2f coord;
    SubPointList *firstChild;

    void (*derive)(PointObject *);

    PointObject *parents[0];
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
    unsigned long long id;
    int show, color;
    ObjectType type;
    GeomObject *next;
    ObjectSelector ptr[0];
};

const GeomObject *mouseSelect(int x, int y);

int create(int argc, const char **argv);

int show(int argc, const char **argv);

int hide(int argc, const char **argv);

#endif //OBJECT_H
