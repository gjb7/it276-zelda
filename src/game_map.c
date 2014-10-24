/*
//  game_map.c
//  zelda
//
//  Created by Grant Butler on 9/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "game_map.h"
#include "entity.h"
#include "graphics.h"
#include <assert.h>
#include <math.h>

void _game_map_update(entity_t *self);
void _game_map_render(entity_t *self);
void _game_map_dealloc(entity_t *self);
entity_t *_game_map_create_from_map(SDL_RWops *fp);
entity_t *_game_map_create_from_v1_map(SDL_RWops *fp);

entity_t *game_map_create(int layer_count, int width, int height) {
    int i, j;
    entity_t *game_map;
    game_map_t *game_map_data;
    
    assert(layer_count > 0);
    assert(width > 0);
    assert(height > 0);
    
    
    game_map = entity_create();
    if (game_map == NULL) {
        goto exit;
    }
    
    game_map_data = malloc(sizeof(game_map_t));
    if (game_map_data == NULL) {
        goto release_map;
    }
    
    memset(game_map_data, 0, sizeof(game_map_t));
    
    game_map_data->layers = malloc(layer_count * sizeof(Uint8 *));
    if (game_map_data->layers == NULL) {
        goto release_map;
    }
    
    for (i = 0; i < layer_count; i++) {
        game_map_data->layers[i] = malloc(height * width * sizeof(Uint8));
        if (game_map_data->layers[i] == NULL) {
            goto release_layers;
        }
    }
    
    game_map_data->layer_count = layer_count;
    game_map_data->layer_width = width;
    game_map_data->layer_height = height;
    game_map_data->quad = quadtree_create(0, graphics_rect_make(0, 0, width, height));
    
    game_map->entity_data = (void *)game_map_data;
    
    strcpy(game_map->class_name, "game_map");
    game_map->update = _game_map_update;
    game_map->render = _game_map_render;
    game_map->dealloc = _game_map_dealloc;
    
    return game_map;
    
release_layers:
    for (j = 0; j < i; j++) {
        free(game_map_data->layers[j]);
    }
    
    free(game_map_data->layers);
release_map:
    entity_release(game_map);
exit:
    
    return NULL;
}

entity_t *game_map_create_from_file(char *filename) {
    entity_t *game_map = NULL;
    SDL_RWops *fp = SDL_RWFromFile(filename, "rb");
    if (fp == NULL) {
        goto cleanup;
    }
    
    game_map = _game_map_create_from_map(fp);
    
    SDL_RWclose(fp);
    
cleanup:
    return game_map;
}

void _game_map_update(entity_t *self) {
    game_map_t *gameMap = (game_map_t *)self->entity_data;
    
    quadtree_clear(gameMap->quad);
    int childCount = g_slist_length(self->children);
    int i;
    for (i = 0; i < childCount; i++) {
        entity_t *child = g_slist_nth_data(self->children, i);
        quadtree_insert(gameMap->quad, &(child->collision_box));
    }
    
    for (i = 0; i < childCount; i++) {
        GSList *returnObjects = NULL;
        int returnObjectsCount;
        entity_t *child = g_slist_nth_data(self->children, i);
        int j;
        
        quadtree_retrieve(gameMap->quad, returnObjects, &(child->collision_box));
        
        returnObjectsCount = g_slist_length(returnObjects);
        
        for (j = 0; j < returnObjectsCount; j++) {
            // TODO: Collision detection!
        }
        
        g_slist_free(returnObjects);
    }
}

void _game_map_render(entity_t *self) {
    game_map_t *gameMap = (game_map_t *)self->entity_data;
    int i, j;
    int layer_size;
    int layer_width;
    SDL_Point frame_size;
    
    assert(gameMap->tilemap != NULL);
    
    layer_size = gameMap->layer_width * gameMap->layer_height;
    layer_width = gameMap->layer_width;
    frame_size = gameMap->tilemap->frame_size;
    
    for (i = 0; i < gameMap->layer_count; i++) {
        for (j = 0; j < layer_size; j++) {
            Uint8 tile_index = gameMap->layers[i][j];
            
            SDL_Rect destRect;
            destRect.x = (j % layer_width) * frame_size.x;
            destRect.y = floor(j / layer_width) * frame_size.y;
            destRect.w = frame_size.x;
            destRect.h = frame_size.y;
            
            sprite_render(gameMap->tilemap, tile_index, destRect);
        }
    }
}

void _game_map_dealloc(entity_t *self) {
    int i;
    game_map_t *game_map_data = (game_map_t *)self->entity_data;
    
    if (game_map_data->tilemap != NULL) {
        sprite_free(game_map_data->tilemap);
        game_map_data->tilemap = NULL;
    }
    
    if (game_map_data->tilemap_filename != NULL) {
        free(game_map_data->tilemap_filename);
        game_map_data->tilemap_filename = NULL;
    }
    
    for (i = 0; i < game_map_data->layer_count; i++) {
        free(game_map_data->layers[i]);
    }
    
    free(game_map_data->layers);
    free(game_map_data);
}

/**
 * -----------
 * Map Parsing
 * -----------
 */

entity_t *_game_map_create_from_map(SDL_RWops *fp) {
    entity_t *game_map = NULL;
    char buffer;
    
    if (SDL_RWread(fp, &buffer, sizeof(buffer), 1) == 0) {
        const char *error = SDL_GetError();
        if (error) {
            fprintf(stderr, "Error parsing map file (V): %s\n", error);
        }
        else {
            fprintf(stderr, "Error parsing map file (V): reached end of file.\n");
        }
        
        goto cleanup;
    }
    
    if (buffer != 'V') {
        fprintf(stderr, "Invalid file format. Expected 'V', instead got: %c\n", buffer);
        
        goto cleanup;
    }
    
    if (SDL_RWread(fp, &buffer, sizeof(buffer), 1) == 0) {
        const char *error = SDL_GetError();
        if (error) {
            fprintf(stderr, "Error parsing map file (version): %s\n", error);
        }
        else {
            fprintf(stderr, "Error parsing map file (version): reached end of file.\n");
        }
        
        goto cleanup;
    }
    
    switch (buffer) {
        case 1:
            if (SDL_RWread(fp, &buffer, sizeof(buffer), 1) == 0) {
                const char *error = SDL_GetError();
                if (error) {
                    fprintf(stderr, "Error parsing map file (;): %s\n", error);
                }
                else {
                    fprintf(stderr, "Error parsing map file (;): reached end of file.\n");
                }
                
                goto cleanup;
            }
            
            if (buffer != ';') {
                fprintf(stderr, "Invalid file format. Expected ';', instead got %c\n", buffer);
                
                goto cleanup;
            }
            
            game_map = _game_map_create_from_v1_map(fp);
            if (game_map == NULL) {
                const char *error = SDL_GetError();
                if (error) {
                    fprintf(stderr, "Error parsing map file (map): %s\n", error);
                }
                else {
                    fprintf(stderr, "Error parsing map file (map): reached end of file.\n");
                }
                
                goto cleanup;
            }
            
            break;
        default:
            fprintf(stderr, "Invalid version number found. Expected '1', instead got %i\n", (int)buffer);
            
            goto cleanup;
            
            break;
    }
    
cleanup:
    return game_map;
}

entity_t *_game_map_create_from_v1_map(SDL_RWops *fp) {
    char width = 0;
    char height = 0;
    char layer_count = 0;
    SDL_Point frame_size;
    int data_size = SDL_RWsize(fp);
    int current_position = SDL_RWtell(fp);
    entity_t *new_map = NULL;
    game_map_t *new_map_data = NULL;
    int layer_size, current_layer = 0;
    char *tilemap_filename = NULL;
    int tilemap_filename_length = 0;
    char buffer;
    
    while (current_position < data_size) {
        if (SDL_RWread(fp, &buffer, sizeof(buffer), 1) == 0) {
            SDL_SetError("Error trying to read a char");
            
            goto cleanup;
        }
        
        switch (buffer) {
            case 'W':
                if (width != 0) {
                    SDL_SetError("Attempting to set width, when width already set to %i", width);
                    
                    goto cleanup;
                }
                
                if (SDL_RWread(fp, &width, sizeof(width), 1) == 0) {
                    SDL_SetError("Error trying to read width");
                    
                    goto cleanup;
                }
                break;
                
            case 'H':
                if (height != 0) {
                    SDL_SetError("Attempting to set height, when height already set to %i", height);
                    
                    goto cleanup;
                }
                
                if (SDL_RWread(fp, &height, sizeof(height), 1) == 0) {
                    SDL_SetError("Error trying to read height");
                    
                    goto cleanup;
                }
                break;
            
            case 'L':
                if (layer_count != 0) {
                    SDL_SetError("Attempting to set layer_count, when layer_count already set to %i", layer_count);
                    
                    goto cleanup;
                }
                
                if (SDL_RWread(fp, &layer_count, sizeof(layer_count), 1) == 0) {
                    SDL_SetError("Error trying to read layer_count");
                    
                    goto cleanup;
                }
                
                if (layer_count <= 0) {
                    SDL_SetError("Invalid file format. No layers provided.");
                    
                    goto cleanup;
                }
                
                break;
            
            case 'l':
                if (width == 0 || height == 0 || layer_count == 0) {
                    SDL_SetError("Missing required data.");
                    
                    goto cleanup;
                }
                
                if (new_map == NULL) {
                    new_map = game_map_create(layer_count, width, height);
                    new_map_data = (game_map_t *)new_map->entity_data;
                    layer_size = width * height;
                }
                
                if (SDL_RWread(fp, new_map_data->layers[current_layer], layer_size, 1) == 0) {
                    SDL_SetError("Invalid file format. Layer at index %i smaller than specified width and height.", current_layer);
                    
                    goto cleanup;
                }
                
                current_layer++;
                
                break;
            
            case 'T': {
                int tilemap_filename_start_location = SDL_RWtell(fp);
                if (tilemap_filename_start_location == -1) {
                    SDL_SetError("Error trying to determine current location.");
                    
                    goto cleanup;
                }
                
                while (1) {
                    if (SDL_RWread(fp, &buffer, sizeof(buffer), 1) == 0) {
                        SDL_SetError("Error trying to read the tilemap filename");
                        
                        goto cleanup;
                    }
                    
                    if (buffer == ';') {
                        break;
                    }
                    
                    tilemap_filename_length++;
                }
                
                if (tilemap_filename_length <= 0) {
                    SDL_SetError("Invalid file format. No tilemap filename provided.");
                    
                    goto cleanup;
                }
                
                if (SDL_RWseek(fp, tilemap_filename_start_location, RW_SEEK_SET) == -1) {
                    SDL_SetError("Invalid file format. Could not seek back to beginning of tilemap filename.");
                    
                    goto cleanup;
                }
                
                tilemap_filename = malloc(sizeof(char) * (tilemap_filename_length + 1));
                if (SDL_RWread(fp, tilemap_filename, tilemap_filename_length, 1) == 0) {
                    SDL_SetError("Unable to read tilemap filename.");
                    
                    goto cleanup;
                }
                
                tilemap_filename[tilemap_filename_length] = '\0';
                
                break;
            }
                
            default:
                SDL_SetError("Invalid file format. Expected 'W', 'H', 'L', 'l', or 'T', instead got %c", buffer);
                
                goto cleanup;
                
                break;
        }
        
        if (SDL_RWread(fp, &buffer, sizeof(buffer), 1) == 0) {
            SDL_SetError("Error trying to read a ';'");
            
            goto cleanup;
        }
        
        if (buffer != ';') {
            SDL_SetError("Invalid file format. Expected ';', instead got %c", buffer);
            
            goto cleanup;
        }
        
        current_position = SDL_RWtell(fp);
    }
    
    if (new_map == NULL) {
        SDL_SetError("Map never created.");
        
        goto cleanup;
    }
    
    if (tilemap_filename == NULL) {
        SDL_SetError("Invalid file format. No tilemap filename provided.");
        
        goto cleanup;
    }
    
    new_map_data->tilemap_filename = tilemap_filename;
    
    frame_size.x = 16;
    frame_size.y = 16;
    
    new_map_data->tilemap = sprite_create(new_map_data->tilemap_filename, frame_size);
    
    if (current_layer < layer_count) {
        SDL_SetError("Invalid file format. Found %i layers when %i specified", current_layer, layer_count);
        
        goto cleanup;
    }
    
    return new_map;
    
cleanup:
    if (new_map != NULL) {
        entity_release(new_map);
    }
    
    return NULL;
}
