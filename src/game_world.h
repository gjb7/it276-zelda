//
//  game_world.h
//  zelda
//
//  Created by Grant Butler on 9/20/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__game_world__
#define __zelda__game_world__

#include <stdlib.h>
#include "entity.h"

typedef struct game_world_s {
    // This is temporarily a void * until we get an actual map object implemented.
    void *current_map;
} game_world_t;

entity_t *game_world_create();

#endif /* defined(__zelda__game_world__) */
