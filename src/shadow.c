#include "shadow.h"
#include "game.h"
#include "platform.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

#define SHADOW_DISTANCE 20
#define SHADOW_AMPLITUDE 12
#define SHADOW_SPEED 5
#define SHADOW_FALL_SPEED 150

int shadow_frame_counts[] = {4, 9};
float shadow_frame_lengths[] = {0.15, 0.13};
Texture2D shadow_texture;
Sound shadow_sound;

void load_shadow() {
    shadow_texture = LoadTexture("assets/shadow.png");
    shadow_sound = LoadSound("assets/shadow_death.wav");
}

void unload_shadow() {
    UnloadTexture(shadow_texture);
    UnloadSound(shadow_sound);
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
    if (!shadow->gone) {
        DrawTextureRec(shadow_texture,
                       (Rectangle){shadow->frame * SHADOW_FRAME_WIDTH,
                                   shadow->hit * SHADOW_FRAME_HEIGHT, SHADOW_FRAME_WIDTH,
                                   SHADOW_FRAME_HEIGHT},
                       shadow->position, WHITE);
    }
}

void update_shadow(Shadow *shadow) {
    shadow->timer += GetFrameTime();

    if (shadow->hit) {
        shadow->position.y += SHADOW_FALL_SPEED * GetFrameTime();

        if (shadow->timer >= shadow_frame_lengths[1]) {
            shadow->timer = 0;
            shadow->frame++;
            if (shadow->frame >= shadow_frame_counts[1]) {
                shadow->gone = true;
            }
        }
    } else {
        shadow->position.y = shadow->platform_y - SHADOW_FRAME_HEIGHT - SHADOW_DISTANCE +
                             sin(GetTime() * SHADOW_SPEED + shadow->platform_y) * SHADOW_AMPLITUDE;
        if (shadow->timer >= shadow_frame_lengths[0]) {
            shadow->timer = 0;
            shadow->frame++;
            shadow->frame %= shadow_frame_counts[0];
        }
    }
}

void kill_shadow(Shadow *shadow) {
    if (shadow->hit) {
        return;
    }

    PlaySound(shadow_sound);

    score += 100;

    shadow->hit = true;
    shake_screen(1, 10);
}
