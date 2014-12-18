/*
//  player.h
//  zelda
//
//  Created by Grant Butler on 10/4/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef __zelda__player__
#define __zelda__player__

#include <stdio.h>
#include <glib.h>
#include "entity.h"
#include "animated_sprite.h"

typedef struct player_s {
    animated_sprite_t *sprite;
    GList *input_list;
    bool is_swinging;
    SDL_Rect swing_rect;
    
    entity_t *sword;
} player_t;

entity_t *player_create();

#endif /* defined(__zelda__player__) */
