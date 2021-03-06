/*
//  tilesheet.h
//  zelda
//
//  Created by Grant Butler on 10/25/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef __zelda__tilesheet__
#define __zelda__tilesheet__

#include <stdio.h>
#include <glib.h>

#include "spritesheet.h"

extern const int tilesheet_frame_width;
extern const int tilesheet_frame_height;

enum {
    TILE_COLLISION_TYPE_NONE = 0,
    TILE_COLLISION_TYPE_FULL_TILE = 1,
    TILE_COLLISION_TYPE_LEFT_UP = 2,
    TILE_COLLISION_TYPE_RIGHT_UP = 3,
    TILE_COLLISION_TYPE_RIGHT_DOWN = 4,
    TILE_COLLISION_TYPE_LEFT_DOWN = 5
};

typedef struct tile_s {
    int collision_type;
} tile_t;

typedef struct tilesheet_s {
    spritesheet_t *spritesheet;
    GHashTable *tiles;
} tilesheet_t;

tilesheet_t *tilesheet_create(char *filename);
void tilesheet_free(tilesheet_t *tilesheet);

#endif /* defined(__zelda__tilesheet__) */
