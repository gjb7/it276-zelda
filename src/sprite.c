//
//  sprite.c
//  zelda
//
//  Created by Grant Butler on 9/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "sprite.h"
#include "graphics.h"
#include <assert.h>

sprite_t *sprite_create(char *filename, SDL_Rect frame_size) {
    assert(filename != NULL);
    assert(frame_size.w > 0);
    assert(frame_size.h > 0);
    
    SDL_Renderer *renderer = graphics_get_global_renderer();
    
    sprite_t *sprite = malloc(sizeof(sprite_t));
    if (sprite == NULL) {
        return NULL;
    }
    
    memset(sprite, 0, sizeof(sprite_t));
    
    sprite->texture = IMG_LoadTexture(renderer, filename);
    if (sprite->texture == NULL) {
        sprite_free(sprite);
        
        return NULL;
    }
    
    int texture_width, texture_height;
    if (SDL_QueryTexture(sprite->texture, NULL, NULL, &texture_width, &texture_height) != 0) {
        fprintf(stderr, "Error getting texture information: %s", SDL_GetError());
        
        sprite_free(sprite);
        
        return NULL;
    }
    
    sprite->frame_size = frame_size;
    sprite->column_count = texture_width / frame_size.w;
    sprite->row_count = texture_height / frame_size.h;
    
    return sprite;
}

void sprite_free(sprite_t *sprite) {
    if (sprite->texture) {
        SDL_DestroyTexture(sprite->texture);
    }
    
    free(sprite);
}
