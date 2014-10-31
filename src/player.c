/*
//  player.c
//  zelda
//
//  Created by Grant Butler on 10/4/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "player.h"
#include "graphics.h"
#include "input.h"
#include "str.h"

void _player_dealloc(entity_t *player);
void _player_render(entity_t *player);
void _player_think(entity_t *player);
void _player_update(entity_t *player);
void _player_touch_world(entity_t *player, entity_direction direction);

entity_t *player_create() {
    player_t *player_data;
    animated_sprite_t *sprite;
    entity_t *player = entity_create();
    
    if (!player) {
        return NULL;
    }
    
    player_data = malloc(sizeof(player_t));
    if (player_data == NULL) {
        entity_release(player);
        
        return NULL;
    }
    
    memset(player_data, 0, sizeof(player_t));
    
    strcpy(player->class_name, "player");
    player->entity_data = player_data;
    player->dealloc = _player_dealloc;
    player->render = _player_render;
    player->update = _player_update;
    
    player->touch_world = _player_touch_world;
    
    player->think = _player_think;
    player->thinkRate = 10;
    
    /** TODO: This should probably be loaded from a config file? */
    player->bounding_box = graphics_rect_make(0, 8, 16, 16);
    
    player->facing = ENTITY_DIRECTION_DOWN;
    
    sprite = animated_sprite_create("res/sprites/link.yaml");
    if (!sprite) {
        entity_release(player);
        
        return NULL;
    }
    animated_sprite_set_current_animation(sprite, "face_down");
    
    player_data->sprite = sprite;
    player_data->input_list = NULL;
    
    return player;
}

void _player_render(entity_t *player) {
    player_t *player_data = (player_t *)player->entity_data;
    
    SDL_Point position = graphics_point_make(0, 0);
    
    animated_sprite_render_frame(player_data->sprite, position);
}

void _player_think(entity_t *player) {
    player_t *player_data = (player_t *)player->entity_data;
    animated_sprite_t *sprite = player_data->sprite;
    
    bool is_up = input_is_key_down(SDL_SCANCODE_W);
    bool is_down = input_is_key_down(SDL_SCANCODE_S);
    bool is_left = input_is_key_down(SDL_SCANCODE_A);
    bool is_right = input_is_key_down(SDL_SCANCODE_D);
    bool is_enter = input_was_key_up(SDL_SCANCODE_RETURN);
    
    if (input_was_key_up(SDL_SCANCODE_W)) {
        player_data->input_list = g_list_append(player_data->input_list, GINT_TO_POINTER(SDL_SCANCODE_W));
    }
    else if (input_was_key_down(SDL_SCANCODE_W)) {
        player_data->input_list = g_list_remove(player_data->input_list, GINT_TO_POINTER(SDL_SCANCODE_W));
    }
    
    if (input_was_key_up(SDL_SCANCODE_S)) {
        player_data->input_list = g_list_append(player_data->input_list, GINT_TO_POINTER(SDL_SCANCODE_S));
    }
    else if (input_was_key_down(SDL_SCANCODE_S)) {
        player_data->input_list = g_list_remove(player_data->input_list, GINT_TO_POINTER(SDL_SCANCODE_S));
    }
    
    if (input_was_key_up(SDL_SCANCODE_A)) {
        player_data->input_list = g_list_append(player_data->input_list, GINT_TO_POINTER(SDL_SCANCODE_A));
    }
    else if (input_was_key_down(SDL_SCANCODE_A)) {
        player_data->input_list = g_list_remove(player_data->input_list, GINT_TO_POINTER(SDL_SCANCODE_A));
    }
    
    if (input_was_key_up(SDL_SCANCODE_D)) {
        player_data->input_list = g_list_append(player_data->input_list, GINT_TO_POINTER(SDL_SCANCODE_D));
    }
    else if (input_was_key_down(SDL_SCANCODE_D)) {
        player_data->input_list = g_list_remove(player_data->input_list, GINT_TO_POINTER(SDL_SCANCODE_D));
    }
    
    if (is_enter) {
        switch (player->facing) {
            case ENTITY_DIRECTION_DOWN:
                animated_sprite_set_current_animation(sprite, "swing_down");
                break;
                
            default:
                break;
        }
        
        player_data->is_swinging = true;
        
        return;
    }
    
    if (str_starts_with(sprite->current_animation_name, "swing")) {
        if (!sprite->current_animation->is_at_end) {
            return;
        }
        else {
            player_data->is_swinging = false;
        }
    }
    
    if (is_up) {
        player->position.y -= 1;
    }
    
    if (is_down) {
        player->position.y += 1;
    }
    
    if (is_left) {
        player->position.x -= 1;
    }
    
    if (is_right) {
        player->position.x += 1;
    }
    
    if (!is_up && !is_down && !is_left && !is_right) {
        switch (player->facing) {
            case ENTITY_DIRECTION_DOWN:
                animated_sprite_set_current_animation(sprite, "face_down");
                
                break;
            case ENTITY_DIRECTION_LEFT:
                animated_sprite_set_current_animation(sprite, "face_left");
                
                break;
            case ENTITY_DIRECTION_UP:
                animated_sprite_set_current_animation(sprite, "face_up");
                
                break;
            case ENTITY_DIRECTION_RIGHT:
                animated_sprite_set_current_animation(sprite, "face_right");
                
                break;
        }
    }
    else {
        GList *last_input_list = g_list_last(player_data->input_list);
        int last_input = GPOINTER_TO_INT(last_input_list->data);
        
        if (last_input == SDL_SCANCODE_W && strcmp(sprite->current_animation_name, "walk_up") != 0) {
            animated_sprite_set_current_animation(sprite, "walk_up");

            player->facing = ENTITY_DIRECTION_UP;
        }
        else if (last_input == SDL_SCANCODE_S && strcmp(sprite->current_animation_name, "walk_down") != 0) {
            animated_sprite_set_current_animation(sprite, "walk_down");
            
            player->facing = ENTITY_DIRECTION_DOWN;
        }
        else if (last_input == SDL_SCANCODE_A && strcmp(sprite->current_animation_name, "walk_left") != 0) {
            animated_sprite_set_current_animation(sprite, "walk_left");
            
            player->facing = ENTITY_DIRECTION_LEFT;
        }
        else if (last_input == SDL_SCANCODE_D && strcmp(sprite->current_animation_name, "walk_right") != 0) {
            animated_sprite_set_current_animation(sprite, "walk_right");
            
            player->facing = ENTITY_DIRECTION_RIGHT;
        }
    }
}

void _player_update(entity_t *player) {
    player_t *player_data = (player_t *)player->entity_data;
    
    animated_sprite_update(player_data->sprite);
}

void _player_touch_world(entity_t *player, entity_direction direction) {
    if ((direction & ENTITY_DIRECTION_DOWN) == ENTITY_DIRECTION_DOWN) {
        player->position.y -= 1;
    }
    
    if ((direction & ENTITY_DIRECTION_LEFT) == ENTITY_DIRECTION_LEFT) {
        player->position.x += 1;
    }
    
    if ((direction & ENTITY_DIRECTION_RIGHT) == ENTITY_DIRECTION_RIGHT) {
        player->position.x -= 1;
    }
    
    if ((direction & ENTITY_DIRECTION_UP) == ENTITY_DIRECTION_UP) {
        player->position.y += 1;
    }
}

void _player_dealloc(entity_t *player) {
    player_t *player_data = (player_t *)player->entity_data;
    
    if (player_data->sprite) {
        animated_sprite_free(player_data->sprite);
    }
    
    free(player->entity_data);
    player->entity_data = NULL;
}
