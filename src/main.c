#include "game.h"
#include "globals.h"
#include "menu.h"
#include "raylib.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 1472
#define WINDOW_HEIGHT 832

float scale;
float screen_ratio;
float output_ratio;
float offset_x;
float offset_y;

Scene scene = SCENE_MENU;
Font font;

bool window_should_close;

int high_score;

int load_high_score() {
    FILE *file = fopen("highscore.txt", "r");
    if (file == NULL) {
        return 0;
    }

    char buffer[20];
    fgets(buffer, 20, file);
    fclose(file);

    return atoi(buffer);
}

void write_high_score() {
    FILE *file = fopen("highscore.txt", "w");
    fprintf(file, "%d", high_score);
    fclose(file);
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gloom");
    InitAudioDevice();
    SetRandomSeed(time(NULL));
    SetMasterVolume(0.5);

    high_score = load_high_score();

    font = LoadFont("assets/font.ttf");

    load_game();
    load_menu();
    reset_menu();

    while (!window_should_close) {
        screen_ratio = (float)GetScreenWidth() / (float)GetScreenHeight();
        output_ratio = (float)GAME_WIDTH / (float)GAME_HEIGHT;
        offset_x = 0;
        offset_y = 0;

        if (screen_ratio > output_ratio) {
            scale = (float)GetScreenHeight() / (float)GAME_HEIGHT;
            offset_x = 0.5 * (GetScreenWidth() - GAME_WIDTH * scale);
        } else {
            scale = (float)GetScreenWidth() / (float)GAME_WIDTH;
            offset_y = 0.5 * (GetScreenHeight() - GAME_HEIGHT * scale);
        }

        switch (scene) {
            case SCENE_MENU:
                update_menu();
                break;
            case SCENE_GAME:
                update_game();
                break;
        }

        BeginDrawing();
        ClearBackground(COLOR_BLACK);

        switch (scene) {
            case SCENE_MENU:
                draw_menu();
                break;
            case SCENE_GAME:
                draw_game();
                break;
        }

        DrawRectangle(0, 0, offset_x, GetScreenHeight(), BLACK);
        DrawRectangle(scale * GAME_WIDTH + offset_x, 0, INT_MAX, GetScreenHeight(), BLACK);

        DrawRectangle(0, 0, GetScreenWidth(), offset_y, BLACK);
        DrawRectangle(0, scale * GAME_HEIGHT + offset_y, GetScreenWidth(), INT_MAX, BLACK);

        EndDrawing();

        window_should_close = window_should_close || WindowShouldClose();
    }

    unload_game();
    unload_menu();
    CloseAudioDevice();
    CloseWindow();

    write_high_score();

    return 0;
}
