//
//  player.c
//  zelda
//
//  Created by Grant Butler on 10/4/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "player.h"

void _player_dealloc(entity_t *player);

entity_t *player_create() {
    entity_t *player = entity_create();
    if (!player) {
        return NULL;
    }
    
    player_t *player_data = malloc(sizeof(player_t));
    if (player_data == NULL) {
        entity_release(player);
        
        return NULL;
    }
    
    memset(player_data, 0, sizeof(player_t));
    
    strcpy(player->class_name, "player");
    player->entity_data = player_data;
    player->dealloc = _player_dealloc;
    
    sprite_t *sprite = sprite_create("res/sprites/link.png", (SDL_Point){ 16, 22 });
    if (!sprite) {
        entity_release(player);
        
        return NULL;
    }
    
    player_data->sprite = sprite;
    
    return player;
}

void _player_dealloc(entity_t *player) {
    free(player->entity_data);
    player->entity_data = NULL;
}
