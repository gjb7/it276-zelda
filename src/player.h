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
#include "entity.h"
#include "animated_sprite.h"
#include "inventory.h"

typedef struct player_s {
    animated_sprite_t *sprite;
    
    inventory_t *inventory;
} player_t;

entity_t *player_create();

inventory_t *player_get_inventory(entity_t *player);

#endif /* defined(__zelda__player__) */
