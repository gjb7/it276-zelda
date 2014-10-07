//
//  player.c
//  zelda
//
//  Created by Grant Butler on 10/4/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "player.h"
#include "graphics.h"

void _player_dealloc(entity_t *player);
void _player_render(entity_t *player);

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
    player->render = _player_render;
    
    sprite_t *sprite = sprite_create("res/sprites/link.png", (SDL_Point){ 16, 22 });
    if (!sprite) {
        entity_release(player);
        
        return NULL;
    }
    
    player_data->sprite = sprite;
    
    return player;
}

void _player_render(entity_t *player) {
    SDL_Renderer *renderer = graphics_get_global_renderer();
    
    player_t *player_data = (player_t *)player->entity_data;
    
    SDL_Point frame_size = player_data->sprite->frame_size;
    
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = frame_size.x;
    srcRect.h = frame_size.y;
    
    SDL_Rect destRect;
    destRect.x = player->position.x;
    destRect.y = player->position.y;
    destRect.w = frame_size.x;
    destRect.h = frame_size.y;
    
    if (SDL_RenderCopy(renderer, player_data->sprite->texture, &srcRect, &destRect) != 0) {
        printf("Error copying: %s\n", SDL_GetError());
        
        return;
    }
}

void _player_dealloc(entity_t *player) {
    free(player->entity_data);
    player->entity_data = NULL;
}
