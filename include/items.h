#ifndef ITEMS_H
#define ITEMS_H

#include "raylib.h"
#include <stdbool.h>

#define COIN_PARTICLE_COUNT 40

typedef struct {
    Vector2 position;
    Vector2 velocity;
} CoinParticle;

typedef struct {
    Vector2 position;
    bool collected;
    int frame;
    float timer;
    CoinParticle particles[COIN_PARTICLE_COUNT];
    bool done_particles;
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
