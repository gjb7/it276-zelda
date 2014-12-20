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

sprite_t *sprite_create(char *filename) {
    SDL_Texture *texture;
    int texture_width, texture_height;
    sprite_t *sprite;
    
    assert(filename != NULL);
    
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
    
    sprite->texture_width = texture_width;
    sprite->texture_height = texture_height;
    
    return sprite;
}

void sprite_render(sprite_t *sprite, SDL_Rect srcRect, SDL_Rect destRect) {
    SDL_Renderer *renderer = graphics_get_global_renderer();
    SDL_Texture *texture = sprite->resource->data.image.texture;

    if (SDL_RenderCopy(renderer, texture, &srcRect, &destRect) != 0) {
        printf("Error copying: %s\n", SDL_GetError());
    }
}

void sprite_render_at_point(sprite_t *sprite, SDL_Point point) {
    SDL_Rect srcRect = graphics_rect_make(0, 0, sprite->texture_width, sprite->texture_height);
    SDL_Rect destRect = graphics_rect_make(point.x, point.y, sprite->texture_width, sprite->texture_height);
    
    sprite_render(sprite, srcRect, destRect);
}

void sprite_free(sprite_t *sprite) {
    if (sprite->resource) {
        resource_release(sprite->resource);
    }
    
    free(sprite);
}
