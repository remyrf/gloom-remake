#ifndef PLATFORM_H
#define PLATFORM_H

#include "raylib.h"

#define PLATFORM_HEIGHT 15

extern Texture2D platform_textures[];

typedef enum { PLATFORM_BIG, PLATFORM_MEDIUM, PLATFORM_SMALL } PlatformSize;

typedef struct {
    Vector2 position;
    PlatformSize size;
} Platform;

void draw_platform(Platform *platform);
void update_platform(Platform platforms[], int index, int platform_count,
                     Camera2D *camera);
void load_platform_textures();
void unload_platforms();

#endif
