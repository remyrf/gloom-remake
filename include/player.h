#ifndef PLAYER_H
#define PLAYER_H

#include "platform.h"
#include "raylib.h"
#include <stdbool.h>

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
void unload_player(Player *player);
void draw_player(Player *player);
void update_player(Player *player, Platform platforms[], int platform_count);

#endif
