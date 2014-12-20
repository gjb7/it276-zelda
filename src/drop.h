//
//  drop.h
//  zelda
//
//  Created by Grant Butler on 11/25/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__drop__
#define __zelda__drop__

#include <stdbool.h>
#include "entity.h"
#include "animated_sprite.h"

typedef enum {
    ZELDA_DROP_TYPE_NONE = 0,
    ZELDA_DROP_TYPE_HEART = 1,
    ZELDA_DROP_TYPE_GREEN_RUPEE = 2,
    ZELDA_DROP_TYPE_BLUE_RUPEE = 3,
    ZELDA_DROP_TYPE_RED_RUPEE = 4,
} drop_type_t;

typedef enum {
    ZELDA_DROP_STATE_DROPPED = 1,
    ZELDA_DROP_STATE_DYING = 2 /** The drop should start flashing to say that it's going to expire soon. */
} drop_state_t;

typedef enum {
    ZELDA_DROP_TARGET_INVENTORY = 1,
    ZELDA_DROP_TARGET_ENTITY = 2,
} drop_target_t;

typedef struct drop_s {
    animated_sprite_t *sprite;
    drop_state_t state;
    drop_type_t type;
    drop_target_t target;
    bool showing;
    int flicker_count;
} drop_t;

typedef struct entity_drop_s {
    drop_type_t type;
    int weight;
} entity_drop_t;

entity_t *drop_create(drop_type_t drop_type, entity_t *source);

int drop_think_interval(drop_type_t drop_type, drop_state_t drop_state);

char *drop_name(drop_type_t drop_type);

#endif /* defined(__zelda__drop__) */
