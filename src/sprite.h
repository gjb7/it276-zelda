//
//  sprite.h
//  zelda
//
//  Created by Grant Butler on 9/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__sprite__
#define __zelda__sprite__

#include <stdlib.h>
#include "sdl.h"

typedef struct sprite_s {
    SDL_Texture *texture;
    SDL_Point frame_size;
    int column_count;
    int row_count;
} sprite_t;

sprite_t *sprite_create(char *filename, SDL_Point frame_size);
void sprite_free(sprite_t *sprite);

#endif /* defined(__zelda__sprite__) */
