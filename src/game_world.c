//
//  game_world.c
//  zelda
//
//  Created by Grant Butler on 9/20/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "game_world.h"

void _game_world_dealloc(entity_t *self);

entity_t *game_world_create() {
    entity_t *game_world = entity_create();
    game_world->class_name = "game_world";
    game_world->dealloc = _game_world_dealloc;
    
    game_world_t *game_world_data = malloc(sizeof(game_world_t));
    
    game_world->entity_data = (void *)game_world_data;
    
    return game_world;
}

void _game_world_dealloc(entity_t *self) {
    game_world_t *game_world_data = (game_world_t *)self->entity_data;
    free(game_world_data);
}
