/*
//  sprite.h
//  zelda
//
//  Created by Grant Butler on 9/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef __zelda__sprite__
#define __zelda__sprite__

#include <stdlib.h>
#include "sdl.h"
#include "resource.h"

typedef struct sprite_s {
    resource_t *resource;
    int texture_width;
    int texture_height;
} sprite_t;

sprite_t *sprite_create(char *filename);
void sprite_render(sprite_t *sprite, SDL_Rect srcRect, SDL_Rect destRect);
void sprite_free(sprite_t *sprite);

#endif /* defined(__zelda__sprite__) */
