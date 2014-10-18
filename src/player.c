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

void _player_dealloc(entity_t *player);
void _player_render(entity_t *player);
void _player_think(entity_t *player);
void _player_update(entity_t *player);

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
    
    player->think = _player_think;
    player->thinkRate = 10;
    
    sprite = animated_sprite_create("res/sprites/link.yaml");
    if (!sprite) {
        entity_release(player);
        
        return NULL;
    }
    animated_sprite_set_current_animation(sprite, "walk_down");
    
    player_data->sprite = sprite;
    
    return player;
}

void _player_render(entity_t *player) {
    player_t *player_data = (player_t *)player->entity_data;
    
    SDL_Point position;
    position.x = 0;
    position.y = 0;
    
    animated_sprite_render_frame(player_data->sprite, position);
}

void _player_think(entity_t *player) {
    if (input_is_key_down(SDL_SCANCODE_W)) {
        player->position.y -= 1;
    }
    
    if (input_is_key_down(SDL_SCANCODE_S)) {
        player->position.y += 1;
    }
    
    if (input_is_key_down(SDL_SCANCODE_A)) {
        player->position.x -= 1;
    }
    
    if (input_is_key_down(SDL_SCANCODE_D)) {
        player->position.x += 1;
    }
}

void _player_update(entity_t *player) {
    player_t *player_data = (player_t *)player->entity_data;
    
    animated_sprite_update(player_data->sprite);
}

void _player_dealloc(entity_t *player) {
    free(player->entity_data);
    player->entity_data = NULL;
}
