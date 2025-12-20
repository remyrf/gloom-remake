#include "constants.h"
#include "platform.h"
#include "player.h"
#include "raylib.h"
#include <limits.h>
#include <stdio.h>
#include <time.h>

#define BACKGROUND_COUNT 2
#define PLATFORM_COUNT 3

Player player;
Platform platforms[PLATFORM_COUNT];

Texture2D background_texture;
Texture2D pillar_texture;

Music music;

Camera2D background_camera;
Camera2D pillars_camera;
Camera2D game_camera;

int background_positions[BACKGROUND_COUNT];
int pillar_position;

void init_game() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gloom");
    InitAudioDevice();
    SetTargetFPS(FPS);
    SetRandomSeed(time(NULL));

    background_texture = LoadTexture("assets/background.png");
    pillar_texture = LoadTexture("assets/pillar.png");

    music = LoadMusicStream("assets/music.mp3");
}

void close_game() {
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
}

void reset_game() {
    platforms[0] =
        (Platform){.position = (Vector2){100, 150}, .size = PLATFORM_MEDIUM};
    platforms[1] =
        (Platform){.position = (Vector2){300, 125}, .size = PLATFORM_BIG};
    platforms[2] =
        (Platform){.position = (Vector2){600, 175}, .size = PLATFORM_SMALL};

    init_player(&player);
    load_platform_textures();

    background_camera = (Camera2D){(Vector2){0, 0}, (Vector2){0, 0}, 0, 1};
    pillars_camera = (Camera2D){(Vector2){0, 0}, (Vector2){0, 0}, 0, 1};
    game_camera = (Camera2D){(Vector2){0, 0}, (Vector2){0, 0}, 0, 1};

    background_positions[0] = 0;
    background_positions[1] = GAME_WIDTH;
    pillar_position = 200;

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

    background_camera.target.x += CAMERA_SPEED * BACKGROUND_PARALLAX;
    pillars_camera.target.x += CAMERA_SPEED * PILLARS_PARALLAX;
    game_camera.target.x += CAMERA_SPEED;

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        update_platform(platforms, i, PLATFORM_COUNT, &game_camera);
    }

    update_player(&player, platforms, PLATFORM_COUNT);
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

    background_camera.offset = (Vector2){offset_x, offset_y};
    pillars_camera.offset = (Vector2){offset_x, offset_y};
    game_camera.offset = (Vector2){offset_x, offset_y};

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
    draw_player(&player);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        draw_platform(&platforms[i]);
    }

    EndMode2D();

    DrawRectangle(0, 0, offset_x, GetScreenHeight(), BLACK);
    DrawRectangle(scale * GAME_WIDTH + offset_x, 0, INT_MAX, GetScreenHeight(),
                  BLACK);

    DrawRectangle(0, 0, GetScreenWidth(), offset_y, BLACK);
    DrawRectangle(0, scale * GAME_HEIGHT + offset_y, GetScreenWidth(), INT_MAX,
                  BLACK);

    EndDrawing();
}

int main(void) {
    init_game();
    reset_game();

    while (!WindowShouldClose()) {
        update_game();
        draw_game();
    }

    close_game();

    return 0;
}
