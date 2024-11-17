#ifndef BOARD_H
#define BOARD_H

#include "object.h"

void showObject(const GeomObject *obj, int color);

GeomObject *mouseSelect(const int x, const int y);

int show(int argc, const char **argv);

int hide(int argc, const char **argv);

#endif //BOARD_H
