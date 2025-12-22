#ifndef ITEMS_H
#define ITEMS_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Vector2 position;
    bool collected;
    int frame;
    float timer;
} Coin;

typedef struct {
    Vector2 position;
    float platform_y;
    bool hit;
    bool gone;
    int frame;
    float timer;
} Shadow;

#endif
