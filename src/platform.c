#include "platform.h"
#include "coin.h"
#include "raylib.h"
#include "shadow.h"
#include <math.h>

#define PLATFORM_TEXTURES_COUNT 3

#define PLATFORM_ITEM_CASE(NAME, UP, LOW, ...)                                                     \
    case PLATFORM_##UP:                                                                            \
        NAME##_##LOW(&platform->item.LOW __VA_OPT__(, ) __VA_ARGS__);                              \
        break;

#define PLATFORM_ITEM_FUNC(NAME, ...)                                                              \
    do {                                                                                           \
        switch (platform->type) {                                                                  \
            PLATFORM_ITEM_CASE(NAME, COIN, coin, __VA_ARGS__)                                      \
            PLATFORM_ITEM_CASE(NAME, SHADOW, shadow, __VA_ARGS__)                                  \
        }                                                                                          \
    } while (0)

Texture2D platform_textures[PLATFORM_TEXTURES_COUNT];

void load_platforms() {
    platform_textures[0] = LoadTexture("assets/platform_big.png");
    platform_textures[1] = LoadTexture("assets/platform_medium.png");
    platform_textures[2] = LoadTexture("assets/platform_small.png");
}

void unload_platforms() {
    for (int i = 0; i < PLATFORM_TEXTURES_COUNT; i++) {
        UnloadTexture(platform_textures[i]);
    }
}

void init_platform(Platform *platform, float x, float y) {
    platform->position.x = x;
    platform->position.y = y;
    platform->size = GetRandomValue(PLATFORM_BIG, PLATFORM_SMALL);
    platform->type = platform->size == PLATFORM_SMALL ? PLATFORM_COIN : PLATFORM_SHADOW;

    PLATFORM_ITEM_FUNC(init, platform);
}

void draw_platform(Platform *platform) {
    DrawTexture(platform_textures[platform->size], platform->position.x, platform->position.y,
                WHITE);

    PLATFORM_ITEM_FUNC(draw);
}

void update_platform(Platform platforms[], int index, int platform_count, Camera2D *camera) {
    Platform *platform = &platforms[index];

    if (platform->position.x + platform_textures[platform->size].width < camera->target.x &&
        (platform->type == PLATFORM_SHADOW || platform->item.coin.done_particles ||
         !platform->item.coin.collected)) {
        Platform *rightmost = platform;
        for (int i = 0; i < platform_count; i++) {
            if (platforms[i].position.x > rightmost->position.x) {
                rightmost = &platforms[i];
            }
        }

        init_platform(platform,
                      rightmost->position.x + platform_textures[rightmost->size].width +
                          GetRandomValue(PLATFORM_MIN_OFFSET, PLATFORM_MAX_OFFSET),
                      GetRandomValue(fmax(rightmost->position.y - PLATFORM_Y_RANGE, PLATFORM_MIN_Y),
                                     PLATFORM_MAX_Y));
    }

    PLATFORM_ITEM_FUNC(update);
}
