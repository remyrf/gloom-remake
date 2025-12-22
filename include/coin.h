#ifndef COIN_H
#define COIN_H

#include "items.h"
#include "platform.h"

#define COIN_FRAME_WIDTH 14

extern Texture2D coin_texture;

void load_coin();
void unload_coin();

void init_coin(Coin *coin, Platform *platform);
void draw_coin(Coin *coin);
void update_coin(Coin *coin);
void collect_coin(Coin *coin);

#endif
