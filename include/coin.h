#ifndef COIN_H
#define COIN_H

#include "items.h"
#include "platform.h"

void load_coin();
void unload_coin();

void init_coin(Coin *coin, Platform *platform);
void draw_coin(Coin *coin);
void update_coin(Coin *coin);

#endif
