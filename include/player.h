#ifndef PLAYER_H
#define PLAYER_H

#include "platform.h"
#include "raylib.h"
#include <stdbool.h>

#define PLAYER_FRAME_WIDTH 13
#define PLAYER_FRAME_HEIGHT 19

typedef enum { PLAYER_IDLE, PLAYER_RUN, PLAYER_DASH } PlayerAnimation;

typedef struct {
    Vector2 position;
    Vector2 velocity;

    int animation_frame;
    float animation_timer;

    bool is_on_floor;
    bool is_dashing;
    bool is_flashing;
    bool can_dash;

    float dash_timer;
    float flash_timer;

    PlayerAnimation animation;

    Vector2 flashlight_position;
} Player;

void load_player();
void unload_player();

void init_player(Player *player);
void draw_player(Player *player);
void update_player(Player *player, Platform platforms[], Camera2D *game_camera);

#endif
