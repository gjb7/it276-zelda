//
//  spritesheet.h
//  zelda
//
//  Created by Grant Butler on 12/18/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__spritesheet__
#define __zelda__spritesheet__

#include "sprite.h"

typedef struct spritesheet_s {
    sprite_t *sprite;
    SDL_Point frame_size;
    int column_count;
    int row_count;
} spritesheet_t;

spritesheet_t *spritesheet_create(char *filename, SDL_Point frame_size);
void spritesheet_render(spritesheet_t *spritesheet, int frame, SDL_Rect destRect);
void spritesheet_free(spritesheet_t *spritesheet);

#endif /* defined(__zelda__spritesheet__) */
