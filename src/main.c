#include "game.h"
#include "raylib.h"
#include <time.h>

#define WINDOW_WIDTH 1472
#define WINDOW_HEIGHT 832

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gloom");
    InitAudioDevice();
    SetRandomSeed(time(NULL));

    load_game();
    reset_game();

    while (!WindowShouldClose()) {
        update_game();
        draw_game();
    }

    unload_game();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
