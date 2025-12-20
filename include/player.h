#ifndef PLAYER_H
#define PLAYER_H

#include "platform.h"
#include "raylib.h"
#include <stdbool.h>

#define PLAYER_FRAME_WIDTH 13
#define PLAYER_FRAME_HEIGHT 19

#define PLAYER_ACCELERATION 3000
#define PLAYER_MAX_SPEED 225
#define PLAYER_FRICTION 0.85

#define PLAYER_DASH_SPEED 650
#define PLAYER_DASH_LENGTH 0.15

#define PLAYER_GRAVITY 1600
#define PLAYER_JUMP_VELOCITY 500
#define PLAYER_JUMP_CUT 0.5

typedef enum { PLAYER_IDLE, PLAYER_RUN, PLAYER_DASH } PlayerAnimation;

typedef struct {
    Vector2 position;
    Vector2 velocity;

    Texture2D texture;

    int animation_frame;
    float animation_timer;

    bool is_on_floor;
    bool is_dashing;

    float dash_timer;

    PlayerAnimation animation;
} Player;

void init_player(Player *player);
void draw_player(Player *player);
void update_player(Player *player, Platform platforms[], int platform_count);

#endif
