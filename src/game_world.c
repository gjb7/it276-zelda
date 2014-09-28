//
//  game_world.c
//  zelda
//
//  Created by Grant Butler on 9/20/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "game_world.h"
#include <assert.h>

void _game_world_dealloc(entity_t *self);

entity_t *game_world_create() {
    entity_t *game_world = entity_create();
    if (game_world == NULL) {
        return NULL;
    }
    
    game_world_t *game_world_data = malloc(sizeof(game_world_t));
    if (game_world_data == NULL) {
        entity_release(game_world);
        
        return NULL;
    }
    
    memset(game_world_data, 0, sizeof(game_world_t));
    
    game_world->entity_data = (void *)game_world_data;
    
    strcpy(game_world->class_name, "game_world");
    game_world->dealloc = _game_world_dealloc;
    
    return game_world;
}

void game_world_set_current_map(entity_t *e, game_map_t *game_map) {
    assert(game_map != NULL);
    
    game_world_t *game_world = (game_world_t *)e->entity_data;
    
    entity_retain(game_map);
    game_map_t *old_game_map = game_world->current_map;
    game_world->current_map = game_map;
    entity_release(old_game_map);
}

void _game_world_dealloc(entity_t *self) {
    game_world_t *game_world_data = (game_world_t *)self->entity_data;
    
    if (game_world_data->current_map != NULL) {
        entity_release(game_world_data->current_map);
        game_world_data->current_map = NULL;
    }
    
    free(game_world_data);
}
