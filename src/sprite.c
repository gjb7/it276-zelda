//
//  sprite.c
//  zelda
//
//  Created by Grant Butler on 9/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "sprite.h"

sprite_t *sprite_create(char *filename, SDL_Renderer *renderer, int column_count, int row_count) {
    sprite_t *sprite = malloc(sizeof(sprite_t));
    if (sprite == NULL) {
        return NULL;
    }
    
    sprite->texture = IMG_LoadTexture(renderer, filename);
    if (sprite->texture == NULL) {
        free(sprite);
        
        return NULL;
    }
    
    int texture_width, texture_height;
    if (SDL_QueryTexture(sprite->texture, NULL, NULL, &texture_width, &texture_height) != 0) {
        fprintf(stderr, "Error getting texture information: %s", SDL_GetError());
        
        sprite_free(sprite);
        
        return NULL;
    }
    
    sprite->frame_size.w = texture_width / column_count;
    sprite->frame_size.h = texture_height / row_count;
    
    return sprite;
}

void sprite_free(sprite_t *sprite) {
    SDL_DestroyTexture(sprite->texture);
    free(sprite);
}
