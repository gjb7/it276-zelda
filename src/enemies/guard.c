/*
//  guard.c
//  zelda
//
//  Created by Grant Butler on 10/21/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "guard.h"
#include "graphics.h"

void _guard_think(entity_t *guard);
void _guard_update(entity_t *guard);
void _guard_render(entity_t *guard);
void _guard_dealloc(entity_t *guard);
void _guard_touch_world(entity_t *guard, entity_direction direction);
void _guard_die(entity_t *guard);

entity_t *guard_create() {
    entity_t *guard = entity_create_from_file("res/entities/guard.yaml");
    if (!guard) {
        return NULL;
    }
    
    guard_t *guard_data = malloc(sizeof(guard_t));
    if (!guard_data) {
        entity_release(guard);
        
        return NULL;
    }
    
    guard_data->state = GUARD_STATE_MOVING;
    guard_data->sprite = animated_sprite_create("res/sprites/guard.yaml");
    
    strcpy(guard->class_name, "enemy:gaurd");
    guard->entity_data = guard_data;
    guard->dealloc = _guard_dealloc;
    guard->think = _guard_think;
    guard->thinkRate = (rand() % 1080) + 720;
    guard->update = _guard_update;
    guard->render = _guard_render;
    guard->touch_world = _guard_touch_world;
    guard->die = _guard_die;
    
    guard->bounding_box = graphics_rect_make(0, 12, 16, 16);
    
    guard->facing = rand() % 4;
    
    switch (guard->facing) {
        case ENTITY_DIRECTION_DOWN:
            animated_sprite_set_current_animation(guard_data->sprite, "walk_down");
            
            break;
        case ENTITY_DIRECTION_LEFT:
            animated_sprite_set_current_animation(guard_data->sprite, "walk_left");
            
            break;
        case ENTITY_DIRECTION_UP:
            animated_sprite_set_current_animation(guard_data->sprite, "walk_up");
            
            break;
        case ENTITY_DIRECTION_RIGHT:
            animated_sprite_set_current_animation(guard_data->sprite, "walk_right");
            
            break;
        
        default:
            guard->facing = ENTITY_DIRECTION_DOWN;
            animated_sprite_set_current_animation(guard_data->sprite, "walk_down");
            break;
    }
    
    return guard;
}

void _guard_think(entity_t *guard) {
    guard_t *guard_data = (guard_t *)guard->entity_data;
    
    if (guard_data->state == GUARD_STATE_IDLE) {
        switch (guard->facing) {
            case ENTITY_DIRECTION_DOWN:
                animated_sprite_set_current_animation(guard_data->sprite, "walk_down");
                
                break;
            case ENTITY_DIRECTION_LEFT:
                animated_sprite_set_current_animation(guard_data->sprite, "walk_left");
                
                break;
            case ENTITY_DIRECTION_UP:
                animated_sprite_set_current_animation(guard_data->sprite, "walk_up");
                
                break;
            case ENTITY_DIRECTION_RIGHT:
                animated_sprite_set_current_animation(guard_data->sprite, "walk_right");
                
                break;
        }
        
        guard_data->state = GUARD_STATE_MOVING;
        
        guard->thinkRate = (rand() % 1080) + 720;
    }
    else if (guard_data->state == GUARD_STATE_MOVING) {
        int facing = rand() % 2;
        
        switch (guard->facing) {
            case ENTITY_DIRECTION_DOWN:
                if (facing == 0) {
                    guard->facing = ENTITY_DIRECTION_RIGHT;
                    
                    animated_sprite_set_current_animation(guard_data->sprite, "face_down_look_right");
                }
                else {
                    guard->facing = ENTITY_DIRECTION_LEFT;
                    
                    animated_sprite_set_current_animation(guard_data->sprite, "face_down_look_left");
                }
                
                break;
            case ENTITY_DIRECTION_LEFT:
                if (facing == 0) {
                    guard->facing = ENTITY_DIRECTION_DOWN;
                    
                    animated_sprite_set_current_animation(guard_data->sprite, "face_left_look_down");
                }
                else {
                    guard->facing = ENTITY_DIRECTION_UP;
                    
                    animated_sprite_set_current_animation(guard_data->sprite, "face_left_look_up");
                }
                
                break;
            case ENTITY_DIRECTION_UP:
                if (facing == 0) {
                    guard->facing = ENTITY_DIRECTION_LEFT;
                    
                    animated_sprite_set_current_animation(guard_data->sprite, "face_up_look_left");
                }
                else {
                    guard->facing = ENTITY_DIRECTION_RIGHT;
                    
                    animated_sprite_set_current_animation(guard_data->sprite, "face_up_look_right");
                }
                
                break;
            case ENTITY_DIRECTION_RIGHT:
                if (facing == 0) {
                    guard->facing = ENTITY_DIRECTION_UP;
                    
                    animated_sprite_set_current_animation(guard_data->sprite, "face_right_look_up");
                }
                else {
                    guard->facing = ENTITY_DIRECTION_DOWN;
                    
                    animated_sprite_set_current_animation(guard_data->sprite, "face_right_look_down");
                }
                
                break;
        }
        
        guard_data->state = GUARD_STATE_IDLE;
        
        guard->thinkRate = (rand() % 1080) + 720;
    }
    else if (guard_data->state == GUARD_STATE_DIE) {
        
    }
}

void _guard_update(entity_t *guard) {
    guard_t *guard_data = (guard_t *)guard->entity_data;
    
    animated_sprite_update(guard_data->sprite);
    
    if (guard_data->state == GUARD_STATE_MOVING) {
        if (SDL_GetTicks() % 2 == 0) {
            switch (guard->facing) {
                case ENTITY_DIRECTION_UP:
                    guard->position.y -= 1;
                    break;
                case ENTITY_DIRECTION_DOWN:
                    guard->position.y += 1;
                    break;
                case ENTITY_DIRECTION_LEFT:
                    guard->position.x -= 1;
                    break;
                case ENTITY_DIRECTION_RIGHT:
                    guard->position.x += 1;
                    break;
                    
                default:
                    break;
            }
        }
    }
}

void _guard_render(entity_t *guard) {
    guard_t *guard_data = (guard_t *)guard->entity_data;
    
    SDL_Point position = graphics_point_make(0, 0);
    
    animated_sprite_render_frame(guard_data->sprite, position);
}

void _guard_touch_world(entity_t *guard, entity_direction direction) {
    guard_t *guard_data = (guard_t *)guard->entity_data;
    
    if (guard_data->state == GUARD_STATE_IDLE ||
        guard_data->state == GUARD_STATE_MOVING) {
        _guard_think(guard);
        
        if ((direction & ENTITY_DIRECTION_DOWN) == ENTITY_DIRECTION_DOWN) {
            guard->position.y -= 1;
        }
        
        if ((direction & ENTITY_DIRECTION_LEFT) == ENTITY_DIRECTION_LEFT) {
            guard->position.x += 1;
        }
        
        if ((direction & ENTITY_DIRECTION_RIGHT) == ENTITY_DIRECTION_RIGHT) {
            guard->position.x -= 1;
        }
        
        if ((direction & ENTITY_DIRECTION_UP) == ENTITY_DIRECTION_UP) {
            guard->position.y += 1;
        }
    }
}

void _guard_die(entity_t *gaurd) {
    
}

void _guard_dealloc(entity_t *guard) {
    guard_t *guard_data = (guard_t *)guard->entity_data;
    
    if (guard_data->sprite) {
        animated_sprite_free(guard_data->sprite);
    }
}
