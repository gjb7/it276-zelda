/*
//  entity.c
//  zelda
//
//  Created by Grant Butler on 9/17/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "entity.h"
#include "sdl.h"
#include "graphics.h"
#include "logging.h"
#include "debug.h"
#include "drop.h"
#include <assert.h>
#include <yaml.h>

bool _load_entity_config_from_yaml_file(char *path, entity_t *entity);
void load_drops(yaml_parser_t *parser, entity_t *entity);

entity_t *entity_spawn_drop_from_entity(entity_t *source);

entity_t *entity_create() {
    entity_t *e = malloc(sizeof(entity_t));
    if (e == NULL) {
        return NULL;
    }
    
    memset(e, 0, sizeof(entity_t));
    e->retain_count = 1;
    
    e->position.x = 0;
    e->position.y = 0;
    
    e->drop_frequency = -1;
    e->drops = NULL;
    
    return e;
}

entity_t *entity_create_from_file(char *path) {
    entity_t *entity = entity_create();
    
    _load_entity_config_from_yaml_file(path, entity);
    
    return entity;
}

void entity_retain(entity_t *e) {
    assert(e != NULL);
    
    e->retain_count++;
}

void _entity_free_children(entity_t *e) {
    e->parent = NULL;
    entity_release(e);
}

void entity_dealloc(entity_t *e) {
    assert(e != NULL);
    
    g_slist_free_full(e->children, (GDestroyNotify)_entity_free_children);
    
    if (e->dealloc != NULL) {
        e->dealloc(e);
    }
}

void entity_release(entity_t *e) {
    assert(e != NULL);
    
    if (--e->retain_count == 0) {
        entity_dealloc(e);
        
        free(e);
    }
}

void entity_add_child(entity_t *parent, entity_t *child) {
    assert(parent != NULL);
    assert(child != NULL);
    
    entity_retain(child);
    
    if (child->parent != NULL) {
        entity_remove_from_parent(child);
    }
    
    parent->children = g_slist_append(parent->children, child);
    child->parent = parent;
    
    if (child->thinkRate > 0) {
        child->thinkNext = SDL_GetTicks() + child->thinkRate;
    }
}

void entity_insert_child_below_child(entity_t *parent, entity_t *newChild, entity_t *existingChild) {
    assert(parent != NULL);
    assert(newChild != NULL);
    assert(existingChild != NULL);
    assert(existingChild->parent == parent);
    
    entity_retain(newChild);
    
    if (newChild->parent != NULL) {
        entity_remove_from_parent(newChild);
    }
    
    GSList *existingChildList = g_slist_find(parent->children, existingChild);
    parent->children = g_slist_insert_before(parent->children, existingChildList, newChild);
    newChild->parent = parent;
}

void entity_remove_from_parent(entity_t *e) {
    entity_t *parent;
    
    assert(e != NULL);
    
    if (e->parent == NULL) {
        return;
    }
    
    parent = e->parent;
    parent->children = g_slist_remove(parent->children, e);
    e->parent = NULL;
    entity_release(e);
}

void _entity_think_iterator(gpointer data, gpointer user_data) {
    entity_t *e = (entity_t *)data;
    
    entity_think(e);
}

void entity_think(entity_t *e) {
    assert(e != NULL);
    
    g_slist_foreach(e->children, _entity_think_iterator, NULL);
    
    if (e->is_hit) {
        if (e->knockback_cooldown % e->knockback_step == 0) {
            e->position.x += e->knockback.x;
            e->position.y += e->knockback.y;
        }
        
        e->knockback_cooldown -= 1;
        
        if (e->knockback_cooldown == 0) {
            e->is_hit = false;
            
            if (e->health <= 0) {
                e->die(e);
                
                entity_t *drop = entity_spawn_drop_from_entity(e);
                if (drop) {
                    entity_add_child(e->parent, drop);
                }
                
                entity_remove_from_parent(e);
            }
        }
        
        return;
    }
    
    if (e->think != NULL) {
        if (SDL_GetTicks() > e->thinkNext) {
            e->think(e);
            
            if (e->thinkRate > 0) {
                e->thinkNext += e->thinkRate;
            }
        }
    }
}

void _entity_render_iterator(gpointer data, gpointer user_data) {
    entity_t *e = (entity_t *)data;
    
    entity_render(e);
}

void entity_render(entity_t *e) {
    assert(e != NULL);
    
    SDL_Point absolutePosition = entity_get_absolute_position(e);
    SDL_Point rendererSize = graphics_global_renderer_size();
    SDL_Rect viewportSize = graphics_rect_make(absolutePosition.x, absolutePosition.y, rendererSize.x, rendererSize.y);
    
    SDL_RenderSetViewport(graphics_get_global_renderer(), &viewportSize);
    
    if (e->render != NULL) {
        e->render(e);
    }
    
    if (debug_get_render_bounding_boxes()) {
        if (e->bounding_box.w > 0 && e->bounding_box.h > 0) {
            SDL_Renderer *renderer = graphics_get_global_renderer();
            
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 96);
            SDL_RenderFillRect(renderer, &(e->bounding_box));
        }
    }
    
    g_slist_foreach(e->children, _entity_render_iterator, NULL);
}

void _entity_update_iterator(gpointer data, gpointer user_data) {
    entity_t *e = (entity_t *)data;
    
    entity_update(e);
}

void entity_update(entity_t *e) {
    assert(e != NULL);
    
    g_slist_foreach(e->children, _entity_update_iterator, NULL);
    
    if (e->update != NULL) {
        e->update(e);
    }
}

SDL_Point entity_get_absolute_position(entity_t *e) {
    SDL_Point position = e->position;
    
    if (e->parent) {
        entity_t *parent = e->parent;
        
        do {
            position.x += parent->position.x;
            position.y += parent->position.y;
            
            parent = parent->parent;
        } while(parent != NULL);
    }
    
    return position;
}

SDL_Rect entity_get_bounding_box(entity_t *e) {
    SDL_Point absolutePosition = entity_get_absolute_position(e);
    SDL_Rect boundingBox = graphics_rect_make(absolutePosition.x + e->bounding_box.x, absolutePosition.y + e->bounding_box.y, e->bounding_box.w, e->bounding_box.h);
    return boundingBox;
}

entity_t *entity_spawn_drop_from_entity(entity_t *source) {
    bool should_spawn_drop = (rand() % source->drop_frequency) == 0;
    if (!should_spawn_drop) {
        return NULL;
    }
    
    int drop_count = g_list_length(source->drops);
    int weights[drop_count];
    int max_weight = 0;
    
    for (int i = 0; i < drop_count; i++) {
        entity_drop_t *drop_info = g_list_nth_data(source->drops, i);
        max_weight += drop_info->weight;
        weights[i] = max_weight - 1;
    }
    
    drop_type_t drop_type = ZELDA_DROP_TYPE_NONE;
    int selected_weight = rand() % max_weight;
    
    for (int i = 0; i < drop_count; i++) {
        int weight = weights[i];
        if (weight >= selected_weight) {
            entity_drop_t *drop_info = g_list_nth_data(source->drops, i);
            drop_type = drop_info->type;
            
            break;
        }
    }
    
    if (drop_type == ZELDA_DROP_TYPE_NONE) {
        return NULL;
    }
    
    return drop_create(drop_type, source);
}

/**
 * Entity config parsing
 */

bool _load_entity_config_from_yaml_file(char *path, entity_t *entity) {
    yaml_parser_t parser;
    yaml_event_t  event;
    FILE *input;
    char *currentKey = NULL;
    
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
            case YAML_SEQUENCE_START_EVENT:
                if (strcmp(currentKey, "drops") == 0) {
                    load_drops(&parser, entity);
                    
                    handledValue = 1;
                }
                
                break;
            case YAML_SCALAR_EVENT:
                if (!currentKey) {
                    char *aKey = (char *)event.data.scalar.value;
                    int length = (strlen(aKey) + 1);
                    
                    currentKey = malloc(length * sizeof(char));
                    strncpy(currentKey, aKey, length);
                    currentKey[length - 1] = '\0';
                }
                else {
                    if (strcmp(currentKey, "drop_frequency") == 0) {
                        entity->drop_frequency = atoi((char *)event.data.scalar.value);
                        
                        handledValue = 1;
                    }
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

void load_drops(yaml_parser_t *parser, entity_t *entity) {
    yaml_event_t event;
    char *currentKey = NULL;
    entity_drop_t *drop = NULL;
    
    do {
        int handledValue = 0;
        
        if (!yaml_parser_parse(parser, &event)) {
            break;
        }
        
        switch (event.type) {
            case YAML_MAPPING_START_EVENT:
                drop = malloc(sizeof(entity_drop_t));
                memset(drop, 0, sizeof(entity_drop_t));
                
                break;
                
            case YAML_MAPPING_END_EVENT:
                entity->drops = g_list_prepend(entity->drops, drop);
                
                break;
                
            case YAML_SCALAR_EVENT:
                if (!currentKey) {
                    char *aKey = (char *)event.data.scalar.value;
                    int length = (strlen(aKey) + 1);
                    
                    currentKey = malloc(length * sizeof(char));
                    strncpy(currentKey, aKey, length);
                    currentKey[length - 1] = '\0';
                }
                else {
                    if (strcmp(currentKey, "type") == 0) {
                        drop->type = atoi((char *)event.data.scalar.value);
                    }
                    else if (strcmp(currentKey, "weight") == 0) {
                        drop->weight = atoi((char *)event.data.scalar.value);
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
        
        if (event.type != YAML_SEQUENCE_END_EVENT) {
            yaml_event_delete(&event);
        }
    } while (event.type != YAML_SEQUENCE_END_EVENT);
    
    entity->drops = g_list_reverse(entity->drops);
    
    yaml_event_delete(&event);
}
