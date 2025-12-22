#include "game.h"
#include "coin.h"
#include "constants.h"
#include "platform.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "shadow.h"
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define BACKGROUND_COUNT 2

#define CAMERA_SPEED 120
#define PILLARS_PARALLAX 0.75
#define BACKGROUND_PARALLAX 0.5

#define SHAKE_LERP_AMOUNT 0.2

Player player;
Platform platforms[PLATFORM_COUNT];

Texture2D background_texture;
Texture2D pillar_texture;

Music music;

Camera2D background_camera;
Camera2D pillars_camera;
Camera2D game_camera;

bool shaking = false;
float shake_timer;
float shake_length;
float shake_amount;
float shake_offset = 0;

int background_positions[BACKGROUND_COUNT];
int pillar_position;

void load_game() {
    background_texture = LoadTexture("assets/background.png");
    pillar_texture = LoadTexture("assets/pillar.png");
    music = LoadMusicStream("assets/music.mp3");

    load_platforms();
    load_player();
    load_coin();
    load_shadow();
}

void unload_game() {
    UnloadTexture(background_texture);
    UnloadTexture(pillar_texture);
    UnloadMusicStream(music);

    unload_platforms();
    unload_player();
    unload_coin();
    unload_shadow();
}

void reset_game() {
    memset(platforms, 0, sizeof(platforms));
    platforms[0] = (Platform){
        .position = (Vector2){175, 150},
        .size = PLATFORM_BIG,
        .type = PLATFORM_SHADOW,
    };
    init_shadow(&platforms[0].item.shadow, &platforms[0]);

    for (int i = 1; i < PLATFORM_COUNT; i++) {
        Platform *platform = &platforms[i];
        Platform *rightmost = platform;

        for (int j = 0; j < PLATFORM_COUNT; j++) {
            if (platforms[j].position.x > rightmost->position.x) {
                rightmost = &platforms[j];
            }
        }

        init_platform(platform,
                      rightmost->position.x + platform_textures[rightmost->size].width +
                          GetRandomValue(PLATFORM_MIN_OFFSET, PLATFORM_MAX_OFFSET),
                      GetRandomValue(fmax(rightmost->position.y - PLATFORM_Y_RANGE, PLATFORM_MIN_Y),
                                     PLATFORM_MAX_Y));
    }

    init_player(&player);

    background_camera = (Camera2D){(Vector2){0, 0}, (Vector2){0, 0}, 0, 1};
    pillars_camera = (Camera2D){(Vector2){0, 0}, (Vector2){0, 0}, 0, 1};
    game_camera = (Camera2D){(Vector2){0, 0}, (Vector2){0, 0}, 0, 1};

    background_positions[0] = 0;
    background_positions[1] = GAME_WIDTH;
    pillar_position = 200;

    SeekMusicStream(music, 0);
    PlayMusicStream(music);
}

void update_game() {
    UpdateMusicStream(music);
    if (!IsMusicStreamPlaying(music)) {
        PlayMusicStream(music);
    }

    for (int i = 0; i < BACKGROUND_COUNT; i++) {
        if (background_positions[i] + GAME_WIDTH < background_camera.target.x) {
            background_positions[i] = background_camera.target.x + GAME_WIDTH;
        }
    }
    if (pillar_position + pillar_texture.width < pillars_camera.target.x) {
        pillar_position = pillars_camera.target.x + GAME_WIDTH;
    }

    background_camera.target.x += CAMERA_SPEED * BACKGROUND_PARALLAX * GetFrameTime();
    pillars_camera.target.x += CAMERA_SPEED * PILLARS_PARALLAX * GetFrameTime();
    game_camera.target.x += CAMERA_SPEED * GetFrameTime();

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        update_platform(platforms, i, PLATFORM_COUNT, &game_camera);
    }

    update_player(&player, platforms, &game_camera);

    if (shaking) {
        shake_timer += GetFrameTime();
        shake_offset += GetRandomValue(0, 100) / 100.0 * shake_amount - 0.5 * shake_amount;
        if (shake_timer >= shake_length) {
            shaking = false;
        }
    } else {
        shake_offset = Lerp(shake_offset, 0, 0.2);
    }
}

void draw_game() {
    float scale;
    float screen_ratio = (float)GetScreenWidth() / (float)GetScreenHeight();
    float output_ratio = (float)GAME_WIDTH / (float)GAME_HEIGHT;
    float offset_x = 0;
    float offset_y = 0;

    if (screen_ratio > output_ratio) {
        scale = (float)GetScreenHeight() / (float)GAME_HEIGHT;
        offset_x = 0.5 * (GetScreenWidth() - GAME_WIDTH * scale);
    } else {
        scale = (float)GetScreenWidth() / (float)GAME_WIDTH;
        offset_y = 0.5 * (GetScreenHeight() - GAME_HEIGHT * scale);
    }

    background_camera.zoom = scale;
    pillars_camera.zoom = scale;
    game_camera.zoom = scale;

    background_camera.offset = (Vector2){offset_x, offset_y + shake_offset};
    pillars_camera.offset = (Vector2){offset_x, offset_y + shake_offset};
    game_camera.offset = (Vector2){offset_x, offset_y + shake_offset};

    BeginDrawing();

    ClearBackground(COLOR_BLACK);

    BeginMode2D(background_camera);
    for (int i = 0; i < BACKGROUND_COUNT; i++) {
        DrawTexture(background_texture, background_positions[i], 0, WHITE);
    }
    EndMode2D();

    BeginMode2D(pillars_camera);
    DrawTexture(pillar_texture, pillar_position, 0, WHITE);
    EndMode2D();

    BeginMode2D(game_camera);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        draw_platform(&platforms[i]);
    }

    draw_player(&player);

    EndMode2D();

    DrawRectangle(0, 0, offset_x, GetScreenHeight(), BLACK);
    DrawRectangle(scale * GAME_WIDTH + offset_x, 0, INT_MAX, GetScreenHeight(), BLACK);

    DrawRectangle(0, 0, GetScreenWidth(), offset_y, BLACK);
    DrawRectangle(0, scale * GAME_HEIGHT + offset_y, GetScreenWidth(), INT_MAX, BLACK);

    EndDrawing();
}

void shake_screen(float time, float amount) {
    shaking = true;
    shake_timer = 0;
    shake_length = time;
    shake_amount = amount;
}
