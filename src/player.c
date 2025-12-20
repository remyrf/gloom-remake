#include "player.h"
#include "platform.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

unsigned int frame_counts[] = {1, 2, 6};
unsigned int frame_lengths[] = {1, 10, 1};

void set_animation(Player *player, PlayerAnimation animation) {
    if (player->animation != animation) {
        player->animation_timer = 0;
        player->animation_frame = 0;
        player->animation = animation;
    }
}

void init_player(Player *player) {
    player->position = (Vector2){150, 50};
    player->velocity = (Vector2){0, 0};
    player->texture = LoadTexture("assets/player.png");
    player->animation_frame = 0;
    player->animation_timer = 0;
    player->animation = PLAYER_RUN;
    player->is_on_floor = false;
    player->is_dashing = false;
}

void draw_player(Player *player) {
    DrawTexturePro(player->texture,
                   (Rectangle){player->animation_frame * PLAYER_FRAME_WIDTH,
                               player->animation * PLAYER_FRAME_HEIGHT,
                               PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT},
                   (Rectangle){player->position.x, player->position.y,
                               PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT},
                   (Vector2){0, 0}, 0, WHITE);
}

void update_player(Player *player, Platform platforms[], int platform_count) {
    player->animation_timer =
        (player->animation_timer + 1) % frame_lengths[player->animation];
    if (player->animation_timer == 0) {
        player->animation_frame++;
        if (player->animation == PLAYER_DASH) {
            if (player->animation_frame >= frame_counts[PLAYER_DASH]) {
                player->animation_frame = frame_counts[PLAYER_DASH] - 1;
            }
        } else {
            player->animation_frame %= frame_counts[player->animation];
        }
    }

    if (!player->is_on_floor && IsKeyPressed(KEY_C)) {
        player->is_dashing = true;
        player->dash_timer = 0;
    }

    if (player->is_dashing) {
        player->dash_timer++;
        player->velocity.x = PLAYER_DASH_SPEED;

        set_animation(player, PLAYER_DASH);

        if (player->dash_timer > PLAYER_DASH_LENGTH) {
            player->is_dashing = false;
        }
    } else {
        float speed_change = PLAYER_ACCELERATION * IsKeyDown(KEY_RIGHT) -
                             (PLAYER_ACCELERATION * IsKeyDown(KEY_LEFT));
        if (speed_change == 0) {
            player->velocity.x *= PLAYER_FRICTION;
            set_animation(player, PLAYER_IDLE);
        } else {
            player->velocity.x += speed_change;
            set_animation(player, PLAYER_RUN);
            if (fabs(player->velocity.x) >= PLAYER_MAX_SPEED) {
                player->velocity.x =
                    PLAYER_MAX_SPEED * (2 * (player->velocity.x > 0) - 1);
            }
        }
    }

    player->position.x += player->velocity.x;

    for (int i = 0; i < platform_count; i++) {
        int platform_width = platform_textures[platforms[i].size].width;
        if (CheckCollisionRecs(
                (Rectangle){player->position.x, player->position.y,
                            PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT},
                (Rectangle){platforms[i].position.x, platforms[i].position.y,
                            platform_width, PLATFORM_HEIGHT})) {
            player->velocity.x = 0;
            if (fabs(player->position.x + PLAYER_FRAME_WIDTH -
                     platforms[i].position.x) <
                fabs(platforms[i].position.x + platform_width -
                     player->position.x)) {
                player->position.x =
                    platforms[i].position.x - PLAYER_FRAME_WIDTH;
            } else {
                player->position.x = platforms[i].position.x + platform_width;
            }
        }
    }

    if (player->is_dashing) {
        player->velocity.y = 0;
    } else {
        player->velocity.y += PLAYER_GRAVITY;
    }

    if (player->is_on_floor) {
        if (IsKeyPressed(KEY_UP)) {
            player->velocity.y = -PLAYER_JUMP_VELOCITY;
        }
    } else if (IsKeyReleased(KEY_UP) && player->velocity.y < 0) {
        player->velocity.y *= PLAYER_JUMP_CUT;
    }

    player->position.y += player->velocity.y;

    player->is_on_floor = false;
    for (int i = 0; i < platform_count; i++) {
        if (CheckCollisionRecs(
                (Rectangle){player->position.x, player->position.y,
                            PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT},
                (Rectangle){platforms[i].position.x, platforms[i].position.y,
                            platform_textures[platforms[i].size].width,
                            PLATFORM_HEIGHT})) {
            player->velocity.y = 0;
            player->is_on_floor = true;
            player->position.y = platforms[i].position.y - PLAYER_FRAME_HEIGHT;
        }
    }
}
