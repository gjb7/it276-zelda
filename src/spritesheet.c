//
//  spritesheet.c
//  zelda
//
//  Created by Grant Butler on 12/18/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "spritesheet.h"
#include "graphics.h"
#include <assert.h>

spritesheet_t *spritesheet_create(char *filename, SDL_Point frame_size) {
    spritesheet_t *spritesheet = malloc(sizeof(spritesheet_t));
    if (!spritesheet) {
        return NULL;
    }
    
    memset(spritesheet, 0, sizeof(spritesheet_t));
    
    assert(frame_size.x > 0);
    assert(frame_size.y > 0);
    
    sprite_t *sprite = sprite_create(filename);
    if (!sprite) {
        spritesheet_free(spritesheet);
        
        return NULL;
    }
    
    spritesheet->sprite = sprite;
    spritesheet->frame_size = frame_size;
    spritesheet->column_count = sprite->texture_width / frame_size.x;
    spritesheet->row_count = sprite->texture_height / frame_size.y;
    
    return spritesheet;
}

void spritesheet_render(spritesheet_t *spritesheet, int frame, SDL_Rect destRect) {
    int column_count = spritesheet->column_count;
    SDL_Point frame_size = spritesheet->frame_size;
    SDL_Rect srcRect = graphics_rect_make((frame % column_count) * frame_size.x, floor(frame / column_count) * frame_size.y, frame_size.x, frame_size.y);
    
    sprite_render(spritesheet->sprite, srcRect, destRect);
}

void spritesheet_render_at_point(spritesheet_t *spritesheet, int frame, SDL_Point point) {
    spritesheet_render(spritesheet, frame, graphics_rect_make(point.x, point.y, spritesheet->frame_size.x, spritesheet->frame_size.y));
}

void spritesheet_free(spritesheet_t *spritesheet) {
    if (spritesheet->sprite) {
        sprite_free(spritesheet->sprite);
    }
    
    free(spritesheet);
}
