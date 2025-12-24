#include "menu.h"
#include "game.h"
#include "globals.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define TITLE "Gloom"
#define TITLE_SIZE 24
#define TITLE_AMPLITUDE 3
#define TITLE_SPEED 8
#define TITLE_OFFSET 40

#define RECT_DISTANCE 10
#define RECT_WIDTH 2

#define BUTTON_BORDER_WIDTH 1
#define BUTTON_PADDING 10
#define BUTTON_HEIGHT 25
#define BUTTON_GAP 2
#define BUTTON_OFFSET 90
#define BUTTON_COUNT 2
#define BUTTON_FONT_SIZE 18
#define BUTTON_WIDTH (GAME_WIDTH - RECT_DISTANCE * 2 - BUTTON_PADDING * 2)

Texture2D menu_background_texture;
Camera2D camera;

Music menu_music;

typedef struct {
    Rectangle rect;
    const char *text;
    void (*on_clicked)();
    bool hovering;
} Button;

void play() {
    scene = SCENE_GAME;
    reset_game();
}

void quit() {
    window_should_close = true;
}

Button buttons[] = {
    {{RECT_DISTANCE + BUTTON_PADDING, BUTTON_HEIGHT + BUTTON_GAP + BUTTON_OFFSET, BUTTON_WIDTH,
      BUTTON_HEIGHT},
     "Play",
     play,
     false},
    {{RECT_DISTANCE + BUTTON_PADDING, (BUTTON_HEIGHT + BUTTON_GAP) * 2 + BUTTON_OFFSET,
      BUTTON_WIDTH, BUTTON_HEIGHT},
     "Quit",
     quit,
     false},
};

float title_y;

void load_menu() {
    menu_background_texture = LoadTexture("assets/menu_background.png");
    menu_music = LoadMusicStream("assets/menu_music.mp3");
}

void unload_menu() {
    UnloadTexture(menu_background_texture);
    UnloadMusicStream(menu_music);
}

void reset_menu() {
    SeekMusicStream(menu_music, 0);
    PlayMusicStream(menu_music);
}

void draw_menu() {
    BeginMode2D(camera);

    DrawTextureEx(menu_background_texture, (Vector2){-8, -5}, 0, 2, WHITE);

    Vector2 title_text_size = MeasureTextEx(font, TITLE, TITLE_SIZE, 1);
    DrawTextEx(font, TITLE, (Vector2){0.5 * GAME_WIDTH - 0.5 * title_text_size.x, title_y},
               TITLE_SIZE, 1, COLOR_WHITE);

    for (int i = 0; i < BUTTON_COUNT; i++) {
        DrawRectangleRec(buttons[i].rect, buttons[i].hovering ? COLOR_WHITE : COLOR_BLACK);
        DrawRectangleLinesEx(buttons[i].rect, BUTTON_BORDER_WIDTH, COLOR_WHITE);

        Vector2 button_text_size = MeasureTextEx(font, buttons[i].text, BUTTON_FONT_SIZE, 1);

        DrawTextEx(font, buttons[i].text,
                   (Vector2){0.5 * BUTTON_WIDTH - 0.5 * button_text_size.x + buttons[i].rect.x,
                             0.5 * BUTTON_HEIGHT - 0.5 * button_text_size.y + buttons[i].rect.y},
                   BUTTON_FONT_SIZE, 1, buttons[i].hovering ? COLOR_BLACK : COLOR_WHITE);
    }

    DrawRectangle(182, 118, 2, 3, buttons[0].hovering ? COLOR_WHITE : COLOR_BLACK);
    DrawTexturePro(
        menu_background_texture,
        (Rectangle){0, 87.5, menu_background_texture.width, menu_background_texture.height - 85},
        (Rectangle){-8, 170, menu_background_texture.width * 2,
                    (menu_background_texture.height - 85) * 2},
        (Vector2){0, 0}, 0, WHITE);

    DrawRectangleLinesEx((Rectangle){RECT_DISTANCE, RECT_DISTANCE, GAME_WIDTH - RECT_DISTANCE * 2,
                                     GAME_HEIGHT - RECT_DISTANCE * 2},
                         RECT_WIDTH, COLOR_WHITE);

    EndMode2D();
}

void update_menu() {
    UpdateMusicStream(menu_music);
    if (!IsMusicStreamPlaying(menu_music)) {
        PlayMusicStream(menu_music);
    }

    camera.offset.x = offset_x;
    camera.offset.y = offset_y;
    camera.zoom = scale;

    title_y = sin(GetTime() * TITLE_SPEED) * TITLE_AMPLITUDE + TITLE_OFFSET;

    for (int i = 0; i < BUTTON_COUNT; i++) {
        buttons[i].hovering =
            CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), buttons[i].rect);
        if (buttons[i].hovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            buttons[i].on_clicked();
        }
    }
}
