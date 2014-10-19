/*
//  game_world.h
//  zelda
//
//  Created by Grant Butler on 9/20/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef __zelda__game_world__
#define __zelda__game_world__

#include <stdlib.h>
#include "entity.h"
#include "game_map.h"

typedef struct game_world_s {
    entity_t *current_map;
    entity_t *player;
} game_world_t;

entity_t *game_world_create();
void game_world_set_current_map(entity_t *e, entity_t *game_map);

#endif /* defined(__zelda__game_world__) */
