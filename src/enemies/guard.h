//
//  guard.h
//  zelda
//
//  Created by Grant Butler on 10/21/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__guard__
#define __zelda__guard__

#include "animated_sprite.h"
#include "entity.h"

typedef enum {
    GUARD_STATE_IDLE,
    GUARD_STATE_MOVING
} guard_state;

typedef struct guard_s {
    animated_sprite_t *sprite;
    
    guard_state state;
} guard_t;

entity_t *guard_create();

#endif /* defined(__zelda__guard__) */
