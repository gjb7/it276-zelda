//
//  animated_sprite.c
//  zelda
//
//  Created by Grant Butler on 10/6/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "animated_sprite.h"
#include "graphics.h"
#include <yaml.h>
#include <assert.h>
#include <stdio.h>

bool load_animations(yaml_parser_t *parser, animated_sprite_t *sprite);
bool load_frames(yaml_parser_t *parser, animation_t *animation);
bool load_animated_sprite_from_yaml_file(char *filename, animated_sprite_t *sprite);

animated_sprite_t *animated_sprite_create(char *filename) {
    assert(filename != NULL);
    
    animated_sprite_t *sprite = malloc(sizeof(animated_sprite_t));
    if (sprite == NULL) {
        return NULL;
    }
    
    memset(sprite, 0, sizeof(animated_sprite_t));
    
    sprite->animations = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
    
    if (!load_animated_sprite_from_yaml_file(filename, sprite)) {
        goto error;
    }
    
    return sprite;

error:
    animated_sprite_free(sprite);
    
    return NULL;
}

void animated_sprite_free(animated_sprite_t *sprite) {
    if (sprite->texture) {
        SDL_DestroyTexture(sprite->texture);
    }
    
    free(sprite);
}

/**
 * Parsing
 */

bool load_animated_sprite_from_yaml_file(char *filename, animated_sprite_t *sprite) {
    yaml_parser_t parser;
    yaml_event_t  event;
    
    yaml_parser_initialize(&parser);
    
    FILE *input = fopen(filename, "rb");
    yaml_parser_set_input_file(&parser, input);
    
    SDL_Renderer *renderer = graphics_get_global_renderer();
    char *currentKey = NULL;
    
    do {
        int handledValue = 0;
        
        if (!yaml_parser_parse(&parser, &event)) {
            break;
        }
        
        switch (event.type) {
            case YAML_SEQUENCE_START_EVENT:
                if (strcmp(currentKey, "animations") == 0) {
                    load_animations(&parser, sprite);
                    
                    handledValue = 1;
                }
                
                break;
            case YAML_SCALAR_EVENT:
                if (!currentKey) {
                    char *aKey = (char *)event.data.scalar.value;
                    
                    currentKey = malloc((strlen(aKey) + 1) * sizeof(char));
                    strlcpy(currentKey, aKey, strlen(aKey) + 1);
                }
                else {
                    if (strcmp(currentKey, "sprite") == 0) {
                        sprite->texture = IMG_LoadTexture(renderer, (char *)event.data.scalar.value);
                        if (sprite->texture == NULL) {
                            goto error;
                        }
                        
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

bool load_animations(yaml_parser_t *parser, animated_sprite_t *sprite) {
    yaml_event_t event;
    char *currentKey = NULL;
    char *animationName = NULL;
    animation_t *animation = NULL;
    
    do {
        int handledValue = 0;
        
        if (!yaml_parser_parse(parser, &event)) {
            break;
        }
        
        switch (event.type) {
            case YAML_MAPPING_START_EVENT:
                animation = malloc(sizeof(animation_t));
                memset(animation, 0, sizeof(animation_t));
                
                animation->loops = true;
                animation->frames = g_array_new(TRUE, FALSE, sizeof(SDL_Rect));
                
                break;
            
            case YAML_MAPPING_END_EVENT:
                g_hash_table_insert(sprite->animations, animationName, animation);
                
                free(animation);
                
                break;
            
            case YAML_SEQUENCE_START_EVENT:
                if (strcmp(currentKey, "frames") == 0) {
                    load_frames(parser, animation);
                    
                    handledValue = 1;
                }
                
                break;
            case YAML_SCALAR_EVENT:
                if (!currentKey) {
                    char *aKey = (char *)event.data.scalar.value;
                    
                    currentKey = malloc((strlen(aKey) + 1) * sizeof(char));
                    strlcpy(currentKey, aKey, strlen(aKey) + 1);
                }
                else {
                    if (strcmp(currentKey, "name") == 0) {
                        if (animationName) {
                            free(animationName);
                        }
                        
                        char *aName = (char *)event.data.scalar.value;
                        
                        animationName = malloc((strlen(aName) + 1) * sizeof(char));
                        strlcpy(animationName, aName, strlen(aName) + 1);
                        
                        handledValue = 1;
                    }
                    else if (strcmp(currentKey, "loops") == 0) {
                        char *value = (char *)event.data.scalar.value;
                        
                        if (strcmp(value, "true") == 0) {
                            animation->loops = true;
                        }
                        else if (strcmp(value, "false") == 0) {
                            animation->loops = false;
                        }
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
        
        if (event.type != YAML_SEQUENCE_END_EVENT) {
            yaml_event_delete(&event);
        }
    } while (event.type != YAML_SEQUENCE_END_EVENT);
    
    yaml_event_delete(&event);
    
    return true;
}

bool load_frames(yaml_parser_t *parser, animation_t *animation) {
    yaml_event_t event;
    char *currentKey = NULL;
    SDL_Rect currentRect;
    
    do {
        int handledValue = 0;
        
        if (!yaml_parser_parse(parser, &event)) {
            break;
        }
        
        switch (event.type) {
            case YAML_SEQUENCE_START_EVENT:
                currentRect.x = 0;
                currentRect.y = 0;
                currentRect.w = 0;
                currentRect.h = 0;
                
                break;
            case YAML_SEQUENCE_END_EVENT:
                g_array_append_val(animation->frames, currentRect);
                
                break;
            case YAML_SCALAR_EVENT:
                if (!currentKey) {
                    char *aKey = (char *)event.data.scalar.value;
                    
                    currentKey = malloc((strlen(aKey) + 1) * sizeof(char));
                    strlcpy(currentKey, aKey, strlen(aKey) + 1);
                }
                else {
                    if (strcmp(currentKey, "x") == 0) {
                        currentRect.x = atoi((char *)event.data.scalar.value);
                        
                        handledValue = 1;
                    }
                    else if (strcmp(currentKey, "y") == 0) {
                        currentRect.y = atoi((char *)event.data.scalar.value);
                        
                        handledValue = 1;
                    }
                    else if (strcmp(currentKey, "w") == 0) {
                        currentRect.w = atoi((char *)event.data.scalar.value);
                        
                        handledValue = 1;
                    }
                    else if (strcmp(currentKey, "h") == 0) {
                        currentRect.h = atoi((char *)event.data.scalar.value);
                        
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
        
        if (event.type != YAML_SEQUENCE_END_EVENT) {
            yaml_event_delete(&event);
        }
    } while (event.type != YAML_SEQUENCE_END_EVENT);
    
    yaml_event_delete(&event);
    
    return true;
}
