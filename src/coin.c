#include "coin.h"
#include "platform.h"
#include "raylib.h"
#include <stdio.h>

#define COIN_FRAME_WIDTH 14
#define COIN_FRAME_LENGTH 0.15
#define COIN_FRAME_COUNT 6
#define COIN_PLATFORM_GAP 10

Texture2D coin_texture;

void load_coin() {
    coin_texture = LoadTexture("assets/coin.png");
}

void unload_coin() {
    UnloadTexture(coin_texture);
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
    DrawTextureRec(
        coin_texture,
        (Rectangle){coin->frame * COIN_FRAME_WIDTH, 0, COIN_FRAME_WIDTH, coin_texture.height},
        coin->position, WHITE);
}

void update_coin(Coin *coin) {
    coin->timer += GetFrameTime();
    if (coin->timer >= COIN_FRAME_LENGTH) {
        coin->timer = 0;

        coin->frame++;
        coin->frame %= COIN_FRAME_COUNT;
    }
}
