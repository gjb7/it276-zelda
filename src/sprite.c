/*
//  sprite.c
//  zelda
//
//  Created by Grant Butler on 9/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "sprite.h"
#include "graphics.h"
#include <assert.h>

sprite_t *sprite_create(char *filename, SDL_Point frame_size) {
    SDL_Texture *texture;
    int texture_width, texture_height;
    sprite_t *sprite;
    
    assert(filename != NULL);
    assert(frame_size.x > 0);
    assert(frame_size.y > 0);
    
    sprite = malloc(sizeof(sprite_t));
    if (sprite == NULL) {
        return NULL;
    }
    
    memset(sprite, 0, sizeof(sprite_t));
    
    sprite->resource = resource_load(filename, RESOURCE_TYPE_IMAGE);
    if (sprite->resource == NULL) {
        sprite_free(sprite);
        
        return NULL;
    }
    
    texture = sprite->resource->data.image.texture;
    
    if (SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height) != 0) {
        fprintf(stderr, "Error getting texture information: %s", SDL_GetError());
        
        sprite_free(sprite);
        
        return NULL;
    }
    
    sprite->frame_size = frame_size;
    sprite->column_count = texture_width / frame_size.x;
    sprite->row_count = texture_height / frame_size.y;
    
    return sprite;
}

void sprite_render(sprite_t *sprite, int frame, SDL_Rect destRect) {
    int column_count = sprite->column_count;
    SDL_Point frame_size = sprite->frame_size;
    SDL_Renderer *renderer = graphics_get_global_renderer();
    SDL_Texture *texture = sprite->resource->data.image.texture;
    
    SDL_Rect srcRect;
    srcRect.x = (frame % column_count) * frame_size.x;
    srcRect.y = floor(frame / column_count) * frame_size.y;
    srcRect.w = frame_size.x;
    srcRect.h = frame_size.y;
    
    if (SDL_RenderCopy(renderer, texture, &srcRect, &destRect) != 0) {
        printf("Error copying: %s\n", SDL_GetError());
    }
}

void sprite_free(sprite_t *sprite) {
    if (sprite->resource) {
        resource_release(sprite->resource);
    }
    
    free(sprite);
}
