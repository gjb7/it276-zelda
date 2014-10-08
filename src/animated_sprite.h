//
//  animated_sprite.h
//  zelda
//
//  Created by Grant Butler on 10/6/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__animated_sprite__
#define __zelda__animated_sprite__

#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include "sdl.h"

typedef struct animation_s {
    GArray *frames;
    int current_frame;
    bool loops;
    bool reverses;
} animation_t;

typedef struct animated_sprite_s {
    SDL_Texture *texture;
    GHashTable *animations;
} animated_sprite_t;

animated_sprite_t *animated_sprite_create(char *filename);
void animated_sprite_free(animated_sprite_t *sprite);

#endif /* defined(__zelda__animated_sprite__) */
