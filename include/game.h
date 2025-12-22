#ifndef GAME_H
#define GAME_H

#define PLATFORM_COUNT 10

void load_game();
void unload_game();
void close_game();
void reset_game();
void update_game();
void draw_game();
void shake_screen(float time, float amount);

#endif
