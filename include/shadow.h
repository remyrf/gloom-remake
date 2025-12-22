#ifndef SHADOW_H
#define SHADOW_H

#include "items.h"
#include "platform.h"

void load_shadow();
void unload_shadow();
void init_shadow(Shadow *shadow, Platform *platform);
void draw_shadow(Shadow *shadow);
void update_shadow(Shadow *shadow);

#endif
