/*
//  game_world.c
//  zelda
//
//  Created by Grant Butler on 9/20/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "game_world.h"
#include "player.h"
#include "graphics.h"
#include "game_map.h"
#include <assert.h>

void _game_world_dealloc(entity_t *self);
void _game_world_update(entity_t *self);

entity_t *game_world_create() {
    game_world_t *game_world_data;
    entity_t *player;
    entity_t *game_world = entity_create();
    
    if (game_world == NULL) {
        return NULL;
    }
    
    game_world_data = malloc(sizeof(game_world_t));
    if (game_world_data == NULL) {
        entity_release(game_world);
        
        return NULL;
    }
    
    memset(game_world_data, 0, sizeof(game_world_t));
    
    game_world->entity_data = (void *)game_world_data;
    
    strcpy(game_world->class_name, "game_world");
    game_world->update = _game_world_update;
    game_world->dealloc = _game_world_dealloc;
    
    player = player_create();
    if (!player) {
        entity_release(game_world);
        
        return NULL;
    }
    
    game_world_data->player = player;
    
    entity_t *inventory_hud = hud_create(player_get_inventory(player), player);
    if (!inventory_hud) {
        entity_release(game_world);
        
        return NULL;
    }
    
    inventory_hud->position = graphics_point_make(18, 12);
    
    game_world_data->inventory_hud = inventory_hud;
    entity_add_child(game_world, inventory_hud);
    
    
    return game_world;
}

void game_world_set_current_map(entity_t *e, entity_t *game_map) {
    entity_t *old_game_map;
    game_world_t *game_world = (game_world_t *)e->entity_data;
    
    assert(game_map != NULL);
    
    entity_remove_from_parent(game_world->player);
    entity_insert_child_below_child(e, game_map, game_world->inventory_hud);
    
    entity_retain(game_map);
    old_game_map = game_world->current_map;
    game_world->current_map = game_map;
    
    game_world->player->position = graphics_point_make(64, 64);
    
    entity_add_child(game_map, game_world->player);
    
    if (old_game_map != NULL) {
        entity_remove_from_parent(old_game_map);
        entity_release(old_game_map);
    }
}

void _game_world_update(entity_t *self) {
    game_world_t *game_world = (game_world_t *)self->entity_data;
    
    entity_t *player_entity = game_world->player;
    SDL_Point player_location = player_entity->position;
    SDL_Rect player_bounding_box = player_entity->bounding_box;
    SDL_Point player_center = graphics_point_make(player_location.x + player_bounding_box.x + (player_bounding_box.w / 2.0), player_location.y + player_bounding_box.y + (player_bounding_box.h / 2.0));
    
    entity_t *map_entity = game_world->current_map;
    SDL_Point map_size = game_map_get_size(map_entity);
    
    SDL_Point renderer_size = graphics_global_renderer_size();

    float scaleX, scaleY;
    graphics_global_renderer_scale(&scaleX, &scaleY);
    SDL_Point scaled_renderer_size = graphics_point_make(renderer_size.x / scaleX, renderer_size.y / scaleY);
    
    SDL_Point map_position = map_entity->position;
    
    if (player_center.x > scaled_renderer_size.x / 2.0 &&
        player_center.x < map_size.x - scaled_renderer_size.x / 2.0) {
        map_position.x = scaled_renderer_size.x / 2.0 - player_center.x;
    }
    
    if (player_center.y > scaled_renderer_size.y / 2.0 &&
        player_center.y < map_size.y - scaled_renderer_size.y / 2.0) {
        map_position.y = scaled_renderer_size.y / 2.0 - player_center.y;
    }
    
    map_entity->position = map_position;
}

void _game_world_dealloc(entity_t *self) {
    game_world_t *game_world_data = (game_world_t *)self->entity_data;
    
    if (game_world_data->current_map != NULL) {
        entity_release(game_world_data->current_map);
        game_world_data->current_map = NULL;
    }
    
    entity_release(game_world_data->player);
    
    free(game_world_data);
    self->entity_data = NULL;
}
