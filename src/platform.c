#include "platform.h"
#include "raylib.h"
#include <math.h>

#define PLATFORM_MIN_OFFSET 50
#define PLATFORM_MAX_OFFSET 200

#define PLATFORM_MIN_Y 75
#define PLATFORM_MAX_Y 185
#define PLATFORM_Y_RANGE 50

#define PLATFORM_TEXTURES_COUNT 3

Texture2D platform_textures[PLATFORM_TEXTURES_COUNT];

void load_platform_textures() {
    platform_textures[0] = LoadTexture("assets/platform_big.png");
    platform_textures[1] = LoadTexture("assets/platform_medium.png");
    platform_textures[2] = LoadTexture("assets/platform_small.png");
}

void unload_platforms() {
    for (int i = 0; i < PLATFORM_TEXTURES_COUNT; i++) {
        UnloadTexture(platform_textures[i]);
    }
}

void draw_platform(Platform *platform) {
    DrawTexture(platform_textures[platform->size], platform->position.x,
                platform->position.y, WHITE);
}

void update_platform(Platform platforms[], int index, int platform_count,
                     Camera2D *camera) {
    Platform *platform = &platforms[index];

    if (platform->position.x + platform_textures[platform->size].width <
        camera->target.x) {
        Platform *rightmost = platform;
        for (int i = 0; i < platform_count; i++) {
            if (platforms[i].position.x > rightmost->position.x) {
                rightmost = &platforms[i];
            }
        }

        platform->position.x =
            rightmost->position.x + platform_textures[rightmost->size].width +
            GetRandomValue(PLATFORM_MIN_OFFSET, PLATFORM_MAX_OFFSET);
        platform->position.y = GetRandomValue(
            fmax(rightmost->position.y - PLATFORM_Y_RANGE, PLATFORM_MIN_Y),
            PLATFORM_MAX_Y);
        platform->size = GetRandomValue(PLATFORM_BIG, PLATFORM_SMALL);
    }
}
