//
//  drop.c
//  zelda
//
//  Created by Grant Butler on 11/25/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "drop.h"
#include "graphics.h"
#include "str.h"
#include <yaml.h>

bool _load_drop_config_from_yaml_file(char *path, entity_t *entity);
char *drop_config_file_for_type(drop_type_t drop_type);

void _drop_dealloc(entity_t *drop);
void _drop_think(entity_t *drop);
void _drop_update(entity_t *drop);
void _drop_render(entity_t *drop);

char *drop_config_file_for_type(drop_type_t drop_type) {
    switch (drop_type) {
        case ZELDA_DROP_TYPE_HEART:
            return "res/entities/drops/heart.yaml";
            
        case ZELDA_DROP_TYPE_GREEN_RUPEE:
            return "res/entities/drops/green-rupee.yaml";
            
        case ZELDA_DROP_TYPE_BLUE_RUPEE:
            return "res/entities/drops/blue-rupee.yaml";
            
        case ZELDA_DROP_TYPE_RED_RUPEE:
            return "res/entities/drops/red-rupee.yaml";
            
        default:
            return "";
    }
}

entity_t *drop_create(drop_type_t drop_type, entity_t *source) {
    drop_t *drop_data;
    entity_t *drop = entity_create();
    if (!drop) {
        return NULL;
    }
    
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
    
    drop_data->state = ZELDA_DROP_STATE_DROPPED;
    drop_data->type = drop_type;
    
    drop->entity_data = drop_data;
    
    if (!_load_drop_config_from_yaml_file(drop_config_file_for_type(drop_type), drop)) {
        entity_release(drop);
        
        return NULL;
    }
    
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

bool entity_is_drop(entity_t *source) {
    return str_starts_with(source->class_name, "drop");
}

drop_target_t drop_get_target(entity_t *drop) {
    drop_t *drop_data = (drop_t *)drop->entity_data;
    
    return drop_data->target;
}

int drop_think_interval(drop_type_t drop_type, drop_state_t drop_state) {
    switch (drop_state) {
        case ZELDA_DROP_STATE_DROPPED:
            return 6000;
        case ZELDA_DROP_STATE_DYING:
            return 60;
        default:
            return 0;
    }
}

/**
 * Drop Config Parsing
 */

bool _load_drop_config_from_yaml_file(char *path, entity_t *entity) {
    yaml_parser_t parser;
    yaml_event_t  event;
    FILE *input;
    char *currentKey = NULL;
    drop_t *drop_data = (drop_t *)entity->entity_data;
    
    yaml_parser_initialize(&parser);
    
    input = fopen(path, "rb");
    if (input == NULL) {
        goto error;
    }
    
    yaml_parser_set_input_file(&parser, input);
    
    do {
        int handledValue = 0;
        
        if (!yaml_parser_parse(&parser, &event)) {
            break;
        }
        
        switch (event.type) {
            case YAML_SCALAR_EVENT:
                if (!currentKey) {
                    char *aKey = (char *)event.data.scalar.value;
                    int length = (strlen(aKey) + 1);
                    
                    currentKey = malloc(length * sizeof(char));
                    strncpy(currentKey, aKey, length);
                    currentKey[length - 1] = '\0';
                }
                else {
                    if (strcmp(currentKey, "name") == 0) {
                        snprintf(entity->class_name, ENTITY_CLASS_NAME_LENGTH, "drop:%s", event.data.scalar.value);
                    }
                    else if (strcmp(currentKey, "sprite") == 0) {
                        char *spriteFile = (char *)event.data.scalar.value;
                        drop_data->sprite = animated_sprite_create(spriteFile);
                        animated_sprite_set_current_animation(drop_data->sprite, "idle");
                    }
                    else if (strcmp(currentKey, "target") == 0) {
                        drop_data->target = atoi((char *)event.data.scalar.value);
                    }
                    
                    handledValue = 1;
                }
                
                break;
            default:
                break;
        }
        
        if (handledValue) {
            free(currentKey);
            currentKey = NULL;
            
            handledValue = 0;
        }
        
        if (event.type != YAML_STREAM_END_EVENT) {
            yaml_event_delete(&event);
        }
    } while (event.type != YAML_STREAM_END_EVENT);
    
    yaml_event_delete(&event);
    
    yaml_parser_delete(&parser);
    
    fclose(input);
    
    return true;
    
error:
    yaml_event_delete(&event);
    yaml_parser_delete(&parser);
    fclose(input);
    
    return false;
}
