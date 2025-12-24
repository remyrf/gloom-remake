#include "coin.h"
#include "game.h"
#include "globals.h"
#include "items.h"
#include "platform.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define COIN_FRAME_LENGTH 0.15
#define COIN_FRAME_COUNT 6
#define COIN_PLATFORM_GAP 10

#define COIN_PARTICLE_MIN_SPEED 50
#define COIN_PARTICLE_MAX_SPEED 200
#define COIN_PARTICLE_LIFETIME 4
#define COIN_PARTICLE_FRICTION 0.9995
#define COIN_PARTICLE_SIZE 1.5

Texture2D coin_texture;
Sound coin_sound;

void load_coin() {
    coin_texture = LoadTexture("assets/coin.png");
    coin_sound = LoadSound("assets/coin.wav");
}

void unload_coin() {
    UnloadTexture(coin_texture);
    UnloadSound(coin_sound);
}

void init_coin(Coin *coin, Platform *platform) {
    coin->frame = 0;
    coin->timer = 0;
    coin->collected = false;

    coin->position.x = platform->position.x + 0.5 * platform_textures[platform->size].width -
                       0.5 * COIN_FRAME_WIDTH;
    coin->position.y = platform->position.y - coin_texture.height - COIN_PLATFORM_GAP;
}

void draw_coin(Coin *coin) {
    if (coin->collected) {
        if (!coin->done_particles) {
            for (int i = 0; i < COIN_PARTICLE_COUNT; i++) {
                DrawRectangleV(coin->particles[i].position,
                               (Vector2){COIN_PARTICLE_SIZE, COIN_PARTICLE_SIZE}, COLOR_WHITE);
            }
        }
    } else {
        DrawTextureRec(
            coin_texture,
            (Rectangle){coin->frame * COIN_FRAME_WIDTH, 0, COIN_FRAME_WIDTH, coin_texture.height},
            coin->position, WHITE);
    }
}

void collect_coin(Coin *coin) {
    if (coin->collected) {
        return;
    }

    PlaySound(coin_sound);
    score += 200;

    coin->collected = true;
    coin->done_particles = false;
    coin->timer = 0;

    for (int i = 0; i < COIN_PARTICLE_COUNT; i++) {
        float angle = GetRandomValue(0, 360) / 360.0 * PI * 2;
        float speed = GetRandomValue(COIN_PARTICLE_MIN_SPEED, COIN_PARTICLE_MAX_SPEED);
        float x = cos(angle) * speed;
        float y = sin(angle) * speed;

        coin->particles[i] =
            (CoinParticle){Vector2Add(coin->position,
                                      (Vector2){COIN_FRAME_WIDTH * 0.5, coin_texture.height * 0.5}),
                           {x, y}};
    }
}

void update_coin(Coin *coin) {
    coin->timer += GetFrameTime();

    if (coin->collected) {
        if (!coin->done_particles) {
            if (coin->timer >= COIN_PARTICLE_LIFETIME) {
                coin->done_particles = true;
            }

            for (int i = 0; i < COIN_PARTICLE_COUNT; i++) {
                coin->particles[i].position.x += coin->particles[i].velocity.x * GetFrameTime();
                coin->particles[i].position.y += coin->particles[i].velocity.y * GetFrameTime();
                coin->particles[i].velocity.x *= COIN_PARTICLE_FRICTION;
                coin->particles[i].velocity.y *= COIN_PARTICLE_FRICTION;
            }
        }
    } else {
        if (coin->timer >= COIN_FRAME_LENGTH) {
            coin->timer = 0;

            coin->frame++;
            coin->frame %= COIN_FRAME_COUNT;
        }
    }
}
