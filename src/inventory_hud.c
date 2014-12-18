//
//  inventory_hud.c
//  zelda
//
//  Created by Grant Butler on 12/18/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "inventory_hud.h"
#include "graphics.h"

void _hud_render(entity_t *hud);
void _hud_dealloc(entity_t *hud);

entity_t *hud_create(inventory_t *inventory) {
    entity_t *hud = entity_create();
    if (!hud) {
        return NULL;
    }
    
    strcpy(hud->class_name, "inventory_hud");
    hud->render = _hud_render;
    hud->dealloc = _hud_dealloc;
    
    inventory_hud_t *inventory_hud = malloc(sizeof(inventory_hud_t));
    if (!inventory_hud) {
        entity_release(hud);
        
        return NULL;
    }
    
    sprite_t *background = sprite_create("res/sprites/empty-hud.png");
    if (!background) {
        entity_release(hud);
        
        return NULL;
    }
    
    inventory_hud->background_sprite = background;
    inventory_hud->inventory = inventory;
    
    hud->entity_data = inventory_hud;
    
    return hud;
}

void _hud_render(entity_t *hud) {
    inventory_hud_t *inventory_hud = (inventory_hud_t *)hud->entity_data;
    
    sprite_render_at_point(inventory_hud->background_sprite, graphics_point_make(0, 0));
}

void _hud_dealloc(entity_t *hud) {
    inventory_hud_t *inventory_hud = (inventory_hud_t *)hud->entity_data;
    
    if (inventory_hud->background_sprite) {
        sprite_free(inventory_hud->background_sprite);
    }
    
    free(inventory_hud);
}
