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
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

typedef struct sprite_s {
    SDL_Texture *texture;
    SDL_Rect frame_size;
} sprite_t;

sprite_t *sprite_create(char *filename, SDL_Renderer *renderer, int column_count, int row_count);

#endif /* defined(__zelda__sprite__) */