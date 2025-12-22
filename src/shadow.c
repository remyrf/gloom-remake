#include "shadow.h"
#include "platform.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

#define SHADOW_FRAME_WIDTH 14
#define SHADOW_FRAME_HEIGHT 33

#define SHADOW_DISTANCE 20
#define SHADOW_AMPLITUDE 12
#define SHADOW_SPEED 5

int shadow_frame_counts[] = {4, 9};
float shadow_frame_lengths[] = {0.15, 0.2};
Texture2D shadow_texture;

void load_shadow() {
    shadow_texture = LoadTexture("assets/shadow.png");
}

void unload_shadow() {
    UnloadTexture(shadow_texture);
}

void init_shadow(Shadow *shadow, Platform *platform) {
    shadow->frame = 0;
    shadow->timer = 0;
    shadow->gone = false;
    shadow->hit = false;
    shadow->platform_y = platform->position.y;

    shadow->position.x =
        platform->position.x +
        (GetRandomValue(0, 9) * 0.05 + 0.5) * platform_textures[platform->size].width -
        0.5 * SHADOW_FRAME_WIDTH;
    shadow->position.y = platform->position.y - SHADOW_FRAME_HEIGHT;
}

void draw_shadow(Shadow *shadow) {
    DrawTextureRec(
        shadow_texture,
        (Rectangle){shadow->frame * SHADOW_FRAME_WIDTH, 0, SHADOW_FRAME_WIDTH, SHADOW_FRAME_HEIGHT},
        shadow->position, WHITE);
}

void update_shadow(Shadow *shadow) {
    // HACK: using platform_y as "random" offset
    shadow->position.y = shadow->platform_y - SHADOW_FRAME_HEIGHT - SHADOW_DISTANCE +
                         sin(GetTime() * SHADOW_SPEED + shadow->platform_y) * SHADOW_AMPLITUDE;

    shadow->timer += GetFrameTime();
    // TODO: change to work for death animation
    if (shadow->timer >= shadow_frame_lengths[0]) {
        shadow->timer = 0;
        shadow->frame++;
        shadow->frame %= shadow_frame_counts[0];
    }
}
