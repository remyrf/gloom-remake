#include "platform.h"
#include "raylib.h"
#include <math.h>

Texture2D platform_textures[3];

void load_platform_textures() {
    platform_textures[0] = LoadTexture("assets/platform_big.png");
    platform_textures[1] = LoadTexture("assets/platform_medium.png");
    platform_textures[2] = LoadTexture("assets/platform_small.png");
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
