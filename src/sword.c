/*
//  sword.c
//  zelda
//
//  Created by Grant Butler on 10/29/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "sword.h"
#include "graphics.h"
#include <assert.h>

#include "logging.h"

void _sword_touch(entity_t *sword, entity_t *enemy);
void _sword_dealloc(entity_t *sword);

entity_t *sword_create() {
    entity_t *sword = entity_create();
    if (!sword) {
        return NULL;
    }
    
    sword_t *sword_data = malloc(sizeof(sword_t));
    if (!sword_data) {
        entity_release(sword);
        
        return NULL;
    }
    
    sword->entity_data = sword_data;
    
    strcpy(sword->class_name, "sword");
    sword->bounding_box = graphics_rect_make(0, 0, 34, 12);
    sword->touch = _sword_touch;
    sword->dealloc = _sword_dealloc;
    
    return sword;
}

void _sword_touch(entity_t *sword, entity_t *enemy) {
    if (enemy->is_hit) {
        return;
    }
    
    sword_t *sword_data = (sword_t *)sword->entity_data;
    int xKnockback = 0;
    int yKnockback = 0;
    
    assert(sword_data->owner != NULL);
    
    if (sword->facing == ENTITY_DIRECTION_DOWN) {
        yKnockback = 2;
    }
    else if (sword->facing == ENTITY_DIRECTION_UP) {
        yKnockback = -2;
    }
    
    enemy->knockback = graphics_point_make(xKnockback, yKnockback);
    enemy->knockback_step = 3;
    enemy->knockback_cooldown = 60;
    enemy->is_hit = true;
}

void _sword_dealloc(entity_t *sword) {
    free(sword->entity_data);
}

entity_t *sword_get_owner(entity_t *sword) {
    sword_t *sword_data = (sword_t *)sword->entity_data;
    
    return sword_data->owner;
}

void sword_set_owner(entity_t *sword, entity_t *owner) {
    sword_t *sword_data = (sword_t *)sword->entity_data;
    
    sword_data->owner = owner;
}

int sword_bounding_box_height_for_direction(entity_t *sword, entity_direction facing) {
    switch (facing) {
        case ENTITY_DIRECTION_DOWN:
            return 12;
        
        case ENTITY_DIRECTION_UP:
            return 19;
            
        default:
            return 12;
    }
}
