//
//  drop.c
//  zelda
//
//  Created by Grant Butler on 11/25/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "drop.h"

char *drop_name(drop_type_t drop_type);

void _drop_dealloc(entity_t *drop);
void _drop_think(entity_t *drop);
void _drop_touch(entity_t *drop, entity_t *other);
void _drop_update(entity_t *drop);

entity_t *spawn_drop(drop_type_t drop_type, entity_t *source) {
    drop_t *drop_data;
    entity_t *drop = entity_create();
    if (!drop) {
        return NULL;
    }
    
    snprintf(drop->class_name, ENTITY_CLASS_NAME_LENGTH, "drop:%s", drop_name(drop_type));
    drop->dealloc = _drop_dealloc;
    drop->thinkRate = drop_think_interval(drop_type, ZELDA_DROP_STATE_DROPPED);
    drop->think = _drop_think;
    drop->touch = _drop_touch;
    drop->update = _drop_update;
    
    drop_data = malloc(sizeof(drop_t));
    if (!drop_data) {
        entity_release(drop);
        
        return NULL;
    }
    
    drop_state->state = ZELDA_DROP_STATE_DROPPED;
    drop_state->type = drop_type;
    
    drop->entity_data = drop_data;
    
    return drop;
}

void _drop_dealloc(entity_t *drop) {
    drop_t *drop_data = (drop_t *)drop->entity_data;
    
    if (drop_data->sprite) {
        animated_sprite_free(drop_data->sprite);
    }
    
    free(drop_data);
}

void _drop_think(entity_t *drop) {
    drop_t *drop_data = (drop_t *)drop->entity_data;
    
    if (drop_data->state == ZELDA_DROP_STATE_DROPPED) {
        drop_data->state = ZELDA_DROP_STATE_DYING;
        
        drop->thinkRate = drop_think_interval(drop_data->type, ZELDA_DROP_STATE_DYING);
    }
    else if (drop_data->state == ZELDA_DROP_STATE_DYING) {
        /** TODO: Delete this from the world. */
    }
}

void _drop_touch(entity_t *drop, entity_t *other) {
//    drop_t *drop_data = (drop_t *)drop->entity_data;
    
    if (strcmp(other->class_name, "player") != 0) {
        return;
    }
}

void _drop_update(entity_t *drop) {
    drop_t *drop_data = (drop_t *)drop->entity_data;
    
//    if (drop_data->)
}

#pragma mark - 

int drop_think_interval(drop_type_t drop_type, drop_state_t drop_state) {
    switch (drop_state) {
        case ZELDA_DROP_STATE_DROPPED:
            return 200;
        case ZELDA_DROP_STATE_DYING:
            return 20;
    }
}

char *drop_name(drop_type_t drop_type) {
    switch (drop_type) {
        case ZELDA_DROP_TYPE_HEART:
            return "heart";
        
        case ZELDA_DROP_TYPE_GREEN_RUPEE:
            return "rupee:green";
            
        case ZELDA_DROP_TYPE_BLUE_RUPEE:
            return "rupee:blue";
            
        case ZELDA_DROP_TYPE_RED_RUPEE:
            return "rupee:red";
        
        default:
            return "";
    }
}
