#ifndef PLATFORM_H
#define PLATFORM_H

#include "items.h"
#include "raylib.h"

#define PLATFORM_MIN_OFFSET 50
#define PLATFORM_MAX_OFFSET 180

#define PLATFORM_MIN_Y 75
#define PLATFORM_MAX_Y 185
#define PLATFORM_Y_RANGE 50

#define PLATFORM_HEIGHT 15

extern Texture2D platform_textures[];

typedef enum { PLATFORM_BIG, PLATFORM_MEDIUM, PLATFORM_SMALL } PlatformSize;
typedef enum { PLATFORM_COIN, PLATFORM_SHADOW } PlatformType;

typedef struct {
    Vector2 position;
    PlatformSize size;
    union {
        Coin coin;
        Shadow shadow;
    } item;
    PlatformType type;
} Platform;

void load_platforms();
void unload_platforms();

void init_platform(Platform *platform, float x, float y);
void draw_platform(Platform *platform);
void update_platform(Platform platforms[], int index, int platform_count, Camera2D *camera);

#endif
