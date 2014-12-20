//
//  inventory_hud.c
//  zelda
//
//  Created by Grant Butler on 12/18/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "inventory_hud.h"
#include "graphics.h"
#include <string.h>

void _hud_render(entity_t *hud);
void _hud_dealloc(entity_t *hud);

void render_text_at_point(spritesheet_t *font_spritesheet, char *text, int length, SDL_Point point);

/** Indexes for the font letters */
enum {
    HUD_FONT_INDEX_0 = 9,
    HUD_FONT_INDEX_1 = 13,
    HUD_FONT_INDEX_2 = 5,
    HUD_FONT_INDEX_3 = 3,
    HUD_FONT_INDEX_4 = 2,
    HUD_FONT_INDEX_5 = 1,
    HUD_FONT_INDEX_6 = 12,
    HUD_FONT_INDEX_7 = 8,
    HUD_FONT_INDEX_8 = 4,
    HUD_FONT_INDEX_9 = 0,
    HUD_FONT_INDEX_EMPTY = 7
};

entity_t *hud_create(inventory_t *inventory, entity_t *entity) {
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
    
    spritesheet_t *font = spritesheet_create("res/sprites/hud-font.png", graphics_point_make(7, 7));
    if (!font) {
        entity_release(hud);
        
        return NULL;
    }
    
    spritesheet_t *hearts_spritesheet = spritesheet_create("res/sprites/hud-hearts.png", graphics_point_make(7, 7));
    if (!hearts_spritesheet) {
        entity_release(hud);
        
        return NULL;
    }

    inventory_hud->background_sprite = background;
    inventory_hud->font_spritesheet = font;
    inventory_hud->hearts_spritesheet = hearts_spritesheet;
    inventory_hud->inventory = inventory;
    inventory_hud->entity = entity;
    
    hud->entity_data = inventory_hud;
    
    return hud;
}

void _hud_render(entity_t *hud) {
    inventory_hud_t *inventory_hud = (inventory_hud_t *)hud->entity_data;
    
    sprite_render_at_point(inventory_hud->background_sprite, graphics_point_make(0, 0));
    
    char rupeeString[4];
    sprintf(rupeeString, "%03d", inventory_hud->inventory->rupees);
    
    render_text_at_point(inventory_hud->font_spritesheet, rupeeString, 3, graphics_point_make(44, 11));
    
    char bombString[3];
    sprintf(bombString, "%02d", inventory_hud->inventory->bombs);
    
    render_text_at_point(inventory_hud->font_spritesheet, bombString, 2, graphics_point_make(76, 11));
    
    char arrowString[3];
    sprintf(arrowString, "%02d", inventory_hud->inventory->arrows);
    
    render_text_at_point(inventory_hud->font_spritesheet, arrowString, 2, graphics_point_make(100, 11));
    
    SDL_Point heart_point = graphics_point_make(140, 11);
    entity_t *entity = inventory_hud->entity;
    for (int i = 0; i < entity->max_health; i += 2) {
        int remainder = entity->health - i;
        
        if (remainder >= 2) {
            spritesheet_render_at_point(inventory_hud->hearts_spritesheet, 0, heart_point);
        }
        else if (remainder == 1) {
            spritesheet_render_at_point(inventory_hud->hearts_spritesheet, 1, heart_point);
        }
        else if (remainder <= 0) {
            spritesheet_render_at_point(inventory_hud->hearts_spritesheet, 2, heart_point);
        }
        
        heart_point.x += 8;
        
        if (i == 40) {
            heart_point.y = 19;
        }
    }
}

void render_text_at_point(spritesheet_t *font_spritesheet, char *text, int length, SDL_Point point) {
    for (int i = 0; i < length; i++) {
        char character = text[i];
        int frame_index = HUD_FONT_INDEX_EMPTY;
        
        switch (character) {
            case '0':
                frame_index = HUD_FONT_INDEX_0;
                break;
                
            case '1':
                frame_index = HUD_FONT_INDEX_1;
                break;
                
            case '2':
                frame_index = HUD_FONT_INDEX_2;
                break;
                
            case '3':
                frame_index = HUD_FONT_INDEX_3;
                break;
                
            case '4':
                frame_index = HUD_FONT_INDEX_4;
                break;
                
            case '5':
                frame_index = HUD_FONT_INDEX_5;
                break;
                
            case '6':
                frame_index = HUD_FONT_INDEX_6;
                break;
                
            case '7':
                frame_index = HUD_FONT_INDEX_7;
                break;
                
            case '8':
                frame_index = HUD_FONT_INDEX_8;
                break;
                
            case '9':
                frame_index = HUD_FONT_INDEX_9;
                break;
        }
        
        spritesheet_render_at_point(font_spritesheet, frame_index, point);
        
        point.x += font_spritesheet->frame_size.x + 1;
    }
}

void _hud_dealloc(entity_t *hud) {
    inventory_hud_t *inventory_hud = (inventory_hud_t *)hud->entity_data;
    
    if (inventory_hud->background_sprite) {
        sprite_free(inventory_hud->background_sprite);
    }
    
    free(inventory_hud);
}
