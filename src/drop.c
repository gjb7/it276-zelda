//
//  drop.c
//  zelda
//
//  Created by Grant Butler on 11/25/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "drop.h"
#include "graphics.h"

char *drop_name(drop_type_t drop_type);
char *drop_sprite_file(drop_type_t drop_type);

void _drop_dealloc(entity_t *drop);
void _drop_think(entity_t *drop);
void _drop_update(entity_t *drop);
void _drop_render(entity_t *drop);

entity_t *drop_create(drop_type_t drop_type, entity_t *source) {
    drop_t *drop_data;
    entity_t *drop = entity_create();
    if (!drop) {
        return NULL;
    }
    
    snprintf(drop->class_name, ENTITY_CLASS_NAME_LENGTH, "drop:%s", drop_name(drop_type));
    drop->dealloc = _drop_dealloc;
    drop->thinkRate = drop_think_interval(drop_type, ZELDA_DROP_STATE_DROPPED);
    drop->think = _drop_think;
    drop->update = _drop_update;
    drop->render = _drop_render;
    
    /** TODO: Revisit this to better position drop. */
    drop->position = source->position;
    
    drop_data = malloc(sizeof(drop_t));
    if (!drop_data) {
        entity_release(drop);
        
        return NULL;
    }
    
    drop_data->sprite = animated_sprite_create(drop_sprite_file(drop_type));
    animated_sprite_set_current_animation(drop_data->sprite, "idle");
    
    drop_data->state = ZELDA_DROP_STATE_DROPPED;
    drop_data->type = drop_type;
    
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
    
    if (drop->thinkRate != drop_think_interval(drop_data->type, drop_data->state)) {
        return;
    }
    
    if (drop_data->state == ZELDA_DROP_STATE_DROPPED) {
        drop_data->state = ZELDA_DROP_STATE_DYING;
        
        drop->thinkRate = drop_think_interval(drop_data->type, ZELDA_DROP_STATE_DYING);
    }
    else if (drop_data->state == ZELDA_DROP_STATE_DYING) {
        drop_data->showing = !drop_data->showing;
        
        if (++drop_data->flicker_count > 45) {
            entity_remove_from_parent(drop);
        }
    }
}

void _drop_update(entity_t *drop) {
    drop_t *drop_data = (drop_t *)drop->entity_data;
    
    if (drop_data->sprite) {
        animated_sprite_update(drop_data->sprite);
    }
}

void _drop_render(entity_t *drop) {
    drop_t *drop_data = (drop_t *)drop->entity_data;
    
    if (drop_data->state == ZELDA_DROP_STATE_DYING) {
        if (!drop_data->showing) {
            return;
        }
    }
    
    if (drop_data->sprite) {
        animated_sprite_render_frame(drop_data->sprite, graphics_point_make(0, 0));
    }
}

#pragma mark - 

int drop_think_interval(drop_type_t drop_type, drop_state_t drop_state) {
    switch (drop_state) {
        case ZELDA_DROP_STATE_DROPPED:
            return 6000;
            break;
        case ZELDA_DROP_STATE_DYING:
            return 60;
            break;
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
