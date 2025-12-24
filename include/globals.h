#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"

#define GAME_WIDTH 368
#define GAME_HEIGHT 208
#define COLOR_BLACK (Color){0x22, 0x23, 0x23, 0xFF}
#define COLOR_WHITE (Color){0xF0, 0xF6, 0xF0, 0xFF}

typedef enum { SCENE_GAME, SCENE_MENU } Scene;

extern float scale;
extern float screen_ratio;
extern float output_ratio;
extern float offset_x;
extern float offset_y;

extern Font font;
extern bool window_should_close;
extern Scene scene;

extern int high_score;

#endif
