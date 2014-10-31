/*
//  sword.c
//  zelda
//
//  Created by Grant Butler on 10/29/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "sword.h"
#include "graphics.h"

void _sword_touch(entity_t *sword, entity_t *enemy);

entity_t *sword_create() {
    entity_t *sword = entity_create();
    if (!sword) {
        return NULL;
    }
    
    strcpy(sword->class_name, "sword");
    sword->bounding_box = graphics_rect_make(0, 0, 34, 12);
    sword->touch = _sword_touch;
    
    return sword;
}

void _sword_touch(entity_t *sword, entity_t *enemy) {
    printf("COLLIDE");
}