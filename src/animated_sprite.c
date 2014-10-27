/**
//  animated_sprite.c
//  zelda
//
//  Created by Grant Butler on 10/6/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "animated_sprite.h"
#include "graphics.h"
#include <yaml.h>
#include <assert.h>
#include <stdio.h>

bool load_animations(yaml_parser_t *parser, animated_sprite_t *sprite);
bool load_frames(yaml_parser_t *parser, animation_t *animation);
bool load_animated_sprite_from_yaml_file(char *filename, animated_sprite_t *sprite);

animated_sprite_t *animated_sprite_create(char *filename) {
    animated_sprite_t *sprite;
    
    assert(filename != NULL);
    
    sprite = malloc(sizeof(animated_sprite_t));
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

void animated_sprite_update(animated_sprite_t *sprite) {
    animation_t *animation = sprite->current_animation;
    frame_t frame = g_array_index(animation->frames, frame_t, animation->current_frame);
    
    animation->time++;
    
    if (animation->time != frame.step) {
        return;
    }
    
    if (animation->loops) {
        animation->current_frame += animation->frame_step;
        
        if (animation->reverses) {
            if (animation->current_frame == animation->frame_count - 1) {
                animation->frame_step = -1;
            }
            else if (animation->current_frame == 0) {
                animation->frame_step = 1;
            }
        }
        else {
            if (animation->current_frame == animation->frame_count) {
                animation->current_frame = 0;
            }
        }
    }
    else {
        if (animation->current_frame != animation->frame_count - 1) {
            animation->current_frame += animation->frame_step;
        }
    }
    
    animation->time = 0;
}

void animated_sprite_render_frame(animated_sprite_t *sprite, SDL_Point destPoint) {
    SDL_Renderer *renderer = graphics_get_global_renderer();
    SDL_Texture *texture = sprite->resource->data.image.texture;
    animation_t *animation = sprite->current_animation;
    frame_t frame = g_array_index(animation->frames, frame_t, animation->current_frame);
    
    SDL_Rect destRect = graphics_rect_make(destPoint.x, destPoint.y, frame.rect.w, frame.rect.h);
    if (SDL_RenderCopy(renderer, texture, &frame.rect, &destRect) != 0) {
        printf("Error copying: %s\n", SDL_GetError());
        
        return;
    }
}

void animated_sprite_set_current_animation(animated_sprite_t *sprite, const char *name) {
    animation_t *animation;
    
    assert(name != NULL);
    
    animation = g_hash_table_lookup(sprite->animations, name);
    if (!animation) {
        return;
    }
    
    animation->current_frame = animation->start_frame;
    animation->frame_step = 1;
    sprite->current_animation = animation;
}

void animated_sprite_free(animated_sprite_t *sprite) {
    if (sprite->resource) {
        resource_release(sprite->resource);
    }
    
    free(sprite);
}

/**
 * Parsing
 */

bool load_animated_sprite_from_yaml_file(char *filename, animated_sprite_t *sprite) {
    yaml_parser_t parser;
    yaml_event_t  event;
    FILE *input;
    char *currentKey = NULL;
    
    yaml_parser_initialize(&parser);
    
    input = fopen(filename, "rb");
    assert(input != NULL);
    
    yaml_parser_set_input_file(&parser, input);
    
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
                    int length = (strlen(aKey) + 1);
                    
                    currentKey = malloc(length * sizeof(char));
                    strncpy(currentKey, aKey, length);
                    currentKey[length - 1] = '\0';
                }
                else {
                    if (strcmp(currentKey, "sprite") == 0) {
                        sprite->resource = resource_load((char *)event.data.scalar.value, RESOURCE_TYPE_IMAGE);
                        if (sprite->resource == NULL) {
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
                animation->reverses = false;
                animation->frames = g_array_new(FALSE, FALSE, sizeof(frame_t));
                animation->start_frame = 0;
                
                break;
            
            case YAML_MAPPING_END_EVENT:
                g_hash_table_insert(sprite->animations, animationName, animation);
                
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
                    int length = (strlen(aKey) + 1);
                    
                    currentKey = malloc(length * sizeof(char));
                    strncpy(currentKey, aKey, length);
                    currentKey[length - 1] = '\0';
                }
                else {
                    if (strcmp(currentKey, "name") == 0) {
                        char *aName = (char *)event.data.scalar.value;
                        int length = (strlen(aName) + 1);
                        
                        animationName = malloc(length * sizeof(char));
                        strncpy(animationName, aName, length);
                        animationName[length - 1] = '\0';
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
                    else if (strcmp(currentKey, "reverses") == 0) {
                        char *value = (char *)event.data.scalar.value;
                        
                        if (strcmp(value, "true") == 0) {
                            animation->reverses = true;
                        }
                        else if (strcmp(value, "false") == 0) {
                            animation->reverses = false;
                        }
                    }
                    else if (strcmp(currentKey, "start_frame") == 0) {
                        animation->start_frame = atoi((char *)event.data.scalar.value);
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
    
    yaml_event_delete(&event);
    
    return true;
}

bool load_frames(yaml_parser_t *parser, animation_t *animation) {
    yaml_event_t event;
    char *currentKey = NULL;
    frame_t currentFrame;
    
    do {
        int handledValue = 0;
        
        if (!yaml_parser_parse(parser, &event)) {
            break;
        }
        
        switch (event.type) {
            case YAML_MAPPING_START_EVENT:
                currentFrame.rect.x = 0;
                currentFrame.rect.y = 0;
                currentFrame.rect.w = 0;
                currentFrame.rect.h = 0;
                currentFrame.step = 200;
                
                break;
            case YAML_MAPPING_END_EVENT:
                g_array_append_val(animation->frames, currentFrame);
                animation->frame_count++;
                
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
                    if (strcmp(currentKey, "x") == 0) {
                        currentFrame.rect.x = atoi((char *)event.data.scalar.value);
                        
                        handledValue = 1;
                    }
                    else if (strcmp(currentKey, "y") == 0) {
                        currentFrame.rect.y = atoi((char *)event.data.scalar.value);
                        
                        handledValue = 1;
                    }
                    else if (strcmp(currentKey, "w") == 0) {
                        currentFrame.rect.w = atoi((char *)event.data.scalar.value);
                        
                        handledValue = 1;
                    }
                    else if (strcmp(currentKey, "h") == 0) {
                        currentFrame.rect.h = atoi((char *)event.data.scalar.value);
                        
                        handledValue = 1;
                    }
                    else if (strcmp(currentKey, "step") == 0) {
                        currentFrame.step = atoi((char *)event.data.scalar.value);
                        
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
