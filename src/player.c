#include "player.h"
#include "coin.h"
#include "constants.h"
#include "game.h"
#include "items.h"
#include "platform.h"
#include "raylib.h"
#include "shadow.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define PLAYER_ACCELERATION 1800
#define PLAYER_MAX_SPEED 165
#define PLAYER_FRICTION 0.85

#define PLAYER_DASH_SPEED 520
#define PLAYER_DASH_LENGTH 0.2

#define PLAYER_GRAVITY 1300
#define PLAYER_JUMP_VELOCITY 420
#define PLAYER_JUMP_CUT 0.5

#define PLAYER_FLASH_LENGTH 0.2

int player_frame_counts[] = {1, 2, 6};
float player_frame_lengths[] = {1, 0.17, 0.017};
Texture2D player_texture;
Texture2D flashlight_texture;

static void die() {
    // TODO:
    reset_game();
}

void set_animation(Player *player, PlayerAnimation animation) {
    if (player->animation != animation) {
        player->animation_timer = 0;
        player->animation_frame = 0;
        player->animation = animation;
    }
}

void load_player() {
    player_texture = LoadTexture("assets/player.png");
    flashlight_texture = LoadTexture("assets/flashlight.png");
}

void unload_player() {
    UnloadTexture(player_texture);
    UnloadTexture(flashlight_texture);
}

void init_player(Player *player) {
    player->position = (Vector2){200, 50};
    player->velocity = (Vector2){0, 0};
    player->animation_frame = 0;
    player->animation_timer = 0;
    player->animation = PLAYER_RUN;
    player->is_on_floor = false;
    player->is_dashing = false;
    player->is_flashing = false;
}

void draw_player(Player *player) {
    DrawTexturePro(player_texture,
                   (Rectangle){player->animation_frame * PLAYER_FRAME_WIDTH,
                               player->animation * PLAYER_FRAME_HEIGHT, PLAYER_FRAME_WIDTH,
                               PLAYER_FRAME_HEIGHT},
                   (Rectangle){player->position.x, player->position.y, PLAYER_FRAME_WIDTH,
                               PLAYER_FRAME_HEIGHT},
                   (Vector2){0, 0}, 0, WHITE);

    if (player->is_flashing) {
        DrawTextureV(flashlight_texture, player->flashlight_position, WHITE);
    }
}

void update_player(Player *player, Platform platforms[], Camera2D *game_camera) {
    player->flashlight_position.x = player->position.x + PLAYER_FRAME_WIDTH + 2;
    player->flashlight_position.y = player->position.y + 2;

    player->animation_timer += GetFrameTime();
    if (player->animation_timer >= player_frame_lengths[player->animation]) {
        player->animation_timer = 0;
        player->animation_frame++;
        if (player->animation == PLAYER_DASH) {
            if (player->animation_frame >= player_frame_counts[PLAYER_DASH]) {
                player->animation_frame = player_frame_counts[PLAYER_DASH] - 1;
            }
        } else {
            player->animation_frame %= player_frame_counts[player->animation];
        }
    }

    if (!player->is_on_floor && IsKeyPressed(KEY_C) && player->can_dash) {
        player->is_dashing = true;
        player->can_dash = false;
        player->dash_timer = 0;
    }

    if (player->is_dashing) {
        player->dash_timer += GetFrameTime();
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
            player->velocity.x += speed_change * GetFrameTime();
            set_animation(player, PLAYER_RUN);
            if (fabs(player->velocity.x) >= PLAYER_MAX_SPEED * GetFrameTime()) {
                player->velocity.x = PLAYER_MAX_SPEED * (2 * IsKeyDown(KEY_RIGHT) - 1);
            }
        }
    }

    player->position.x += player->velocity.x * GetFrameTime();

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        int platform_width = platform_textures[platforms[i].size].width;
        if (CheckCollisionRecs((Rectangle){player->position.x, player->position.y,
                                           PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT},
                               (Rectangle){platforms[i].position.x, platforms[i].position.y,
                                           platform_width, PLATFORM_HEIGHT})) {
            player->velocity.x = 0;
            if (fabs(player->position.x + PLAYER_FRAME_WIDTH - platforms[i].position.x) <
                fabs(platforms[i].position.x + platform_width - player->position.x)) {
                player->position.x = platforms[i].position.x - PLAYER_FRAME_WIDTH;
            } else {
                player->position.x = platforms[i].position.x + platform_width;
            }
        }
    }

    if (player->is_dashing) {
        player->velocity.y = 0;
    } else {
        player->velocity.y += PLAYER_GRAVITY * GetFrameTime();
    }

    if (player->is_on_floor) {
        if (IsKeyPressed(KEY_UP)) {
            player->velocity.y = -PLAYER_JUMP_VELOCITY;
        }
    } else if (IsKeyReleased(KEY_UP) && player->velocity.y < 0) {
        player->velocity.y *= PLAYER_JUMP_CUT;
    }

    player->position.y += player->velocity.y * GetFrameTime();

    player->is_on_floor = false;
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        if (CheckCollisionRecs((Rectangle){player->position.x, player->position.y,
                                           PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT},
                               (Rectangle){platforms[i].position.x, platforms[i].position.y,
                                           platform_textures[platforms[i].size].width,
                                           PLATFORM_HEIGHT})) {
            player->velocity.y = 0;
            player->is_on_floor = true;
            player->can_dash = true;
            player->position.y = platforms[i].position.y - PLAYER_FRAME_HEIGHT;
        }

        switch (platforms[i].type) {
            case PLATFORM_COIN:
                {
                    Coin *coin = &platforms[i].item.coin;
                    if (CheckCollisionRecs((Rectangle){player->position.x, player->position.y,
                                                       PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT},
                                           (Rectangle){coin->position.x, coin->position.y,
                                                       COIN_FRAME_WIDTH, coin_texture.height})) {
                        collect_coin(coin);
                    }
                    break;
                }

            case PLATFORM_SHADOW:
                {
                    Shadow *shadow = &platforms[i].item.shadow;
                    if (CheckCollisionRecs((Rectangle){player->position.x, player->position.y,
                                                       PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT},
                                           (Rectangle){shadow->position.x, shadow->position.y,
                                                       SHADOW_FRAME_WIDTH, SHADOW_FRAME_HEIGHT})) {
                        if (!shadow->hit) {
                            die();
                        }
                    } else if (player->is_flashing &&
                               CheckCollisionRecs(
                                   (Rectangle){shadow->position.x, shadow->position.y,
                                               SHADOW_FRAME_WIDTH, SHADOW_FRAME_HEIGHT},
                                   (Rectangle){
                                       player->flashlight_position.x, player->flashlight_position.y,
                                       flashlight_texture.width, flashlight_texture.height})) {
                        kill_shadow(shadow);
                    }
                }
                break;
        }
    }

    if ((IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT)) && !player->is_flashing) {
        player->is_flashing = true;
        player->flash_timer = 0;
    }

    if (player->is_flashing) {
        player->flash_timer += GetFrameTime();
        if (player->flash_timer >= PLAYER_FLASH_LENGTH) {
            player->is_flashing = false;
        }
    }

    if (player->position.x + PLAYER_FRAME_WIDTH < game_camera->target.x ||
        player->position.x > game_camera->target.x + GAME_WIDTH ||
        player->position.y > game_camera->target.y + GAME_HEIGHT) {
        die();
    }
}
