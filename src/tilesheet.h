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

#include "sprite.h"

extern const int tilesheet_frame_width;
extern const int tilesheet_frame_height;

enum {
    COLLISION_TYPE_NONE = 0,
    COLLISION_TYPE_FULL_TILE = 1,
    COLLISION_TYPE_LEFT_UP = 2,
    COLLISION_TYPE_RIGHT_UP = 3,
    COLLISION_TYPE_RIGHT_DOWN = 4,
    COLLISION_TYPE_LEFT_DOWN = 5
};

typedef struct tile_s {
    int collision_type;
} tile_t;

typedef struct tilesheet_s {
    sprite_t *sprite;
    GHashTable *tiles;
} tilesheet_t;

tilesheet_t *tilesheet_create(char *filename);
void tilesheet_free(tilesheet_t *tilesheet);

#endif /* defined(__zelda__tilesheet__) */
