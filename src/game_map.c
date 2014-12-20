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
#include <glib.h>
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
    game_map_data->quad = quadtree_create(0, graphics_rect_make(0, 0, width * tilesheet_frame_width, height * tilesheet_frame_height));
    
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

SDL_Point game_map_get_size(entity_t *entity) {
    game_map_t *map = (game_map_t *)entity->entity_data;
    
    return graphics_point_make(map->layer_width * tilesheet_frame_width, map->layer_height * tilesheet_frame_height);
}

void _game_map_check_collision(game_map_t *map, entity_t *child, entity_direction edge, entity_direction *collision_direction) {
    SDL_Rect boundingBox = entity_get_bounding_box(child);
    SDL_Point firstPoint;
    SDL_Point secondPoint;
    int first_index, second_index;
    int j;
    
    switch (edge) {
        case ENTITY_DIRECTION_UP:
            firstPoint = graphics_point_make(floor(boundingBox.x / tilesheet_frame_width), floor(boundingBox.y / tilesheet_frame_height));
            secondPoint = graphics_point_make(floor((boundingBox.x + boundingBox.w) / tilesheet_frame_width), floor(boundingBox.y / tilesheet_frame_height));
            break;
        case ENTITY_DIRECTION_RIGHT:
            firstPoint = graphics_point_make(floor((boundingBox.x + boundingBox.w) / tilesheet_frame_width), floor(boundingBox.y / tilesheet_frame_height));
            secondPoint = graphics_point_make(floor((boundingBox.x + boundingBox.w) / tilesheet_frame_width), floor((boundingBox.y + boundingBox.h) / tilesheet_frame_height));
            break;
        case ENTITY_DIRECTION_DOWN:
            firstPoint = graphics_point_make(floor((boundingBox.x + boundingBox.w) / tilesheet_frame_width), floor((boundingBox.y + boundingBox.h) / tilesheet_frame_height));
            secondPoint = graphics_point_make(floor(boundingBox.x / tilesheet_frame_width), floor((boundingBox.y + boundingBox.h) / tilesheet_frame_height));
            break;
        case ENTITY_DIRECTION_LEFT:
            firstPoint = graphics_point_make(floor(boundingBox.x / tilesheet_frame_width), floor((boundingBox.y + boundingBox.h) / tilesheet_frame_height));
            secondPoint = graphics_point_make(floor(boundingBox.x / tilesheet_frame_width), floor(boundingBox.y / tilesheet_frame_height));
            break;
    }
    
    first_index = (firstPoint.y * map->layer_width) + firstPoint.x;
    second_index = (secondPoint.y * map->layer_width) + secondPoint.x;
    
    /* TODO: We should just be checking one layer. Figure out which one. */
    for (j = 0; j < map->layer_count; j++) {
        int first_tile_index = (int)map->layers[j][first_index];
        tile_t *first_tile = g_hash_table_lookup(map->tilesheet->tiles, &first_tile_index);
        if (first_tile == NULL) {
            continue;
        }
        
        int second_tile_index = (int)map->layers[j][second_index];
        tile_t *second_tile = g_hash_table_lookup(map->tilesheet->tiles, &second_tile_index);
        if (second_tile == NULL) {
            continue;
        }
        
        /* TODO: We only support full block for now. */
        if (first_tile->collision_type == TILE_COLLISION_TYPE_FULL_TILE && second_tile->collision_type == TILE_COLLISION_TYPE_FULL_TILE) {
            *collision_direction |= edge;
        }
    }
}

void _game_map_update(entity_t *self) {
    game_map_t *gameMap = (game_map_t *)self->entity_data;
    int i;
    int childCount;
    
    quadtree_clear(gameMap->quad);
    
    childCount = g_slist_length(self->children);
    
    for (i = 0; i < childCount; i++) {
        entity_t *child = g_slist_nth_data(self->children, i);
        
        SDL_Rect boundingBox = entity_get_bounding_box(child);
        if (boundingBox.w > 0 && boundingBox.h > 0) {
            quadtree_insert(gameMap->quad, child);
        }
    }
    
    for (i = 0; i < childCount; i++) {
        GSList *returnObjects = NULL;
        int returnObjectsCount;
        entity_t *child = g_slist_nth_data(self->children, i);
        int j;
        
        if (!child) {
            continue;
        }
        
        SDL_Rect boundingBox = entity_get_bounding_box(child);
        
        if (boundingBox.w <= 0 || boundingBox.h <= 0) {
            continue;
        }
        
        returnObjects = quadtree_retrieve(gameMap->quad, returnObjects, child);
        returnObjectsCount = g_slist_length(returnObjects);
        
        for (j = 0; j < returnObjectsCount; j++) {
            entity_t *collidedObject = g_slist_nth_data(returnObjects, j);
            SDL_Rect collidedObjectBoundingBox;
            
            if (collidedObject == child) {
                continue;
            }
            
            collidedObjectBoundingBox = entity_get_bounding_box(collidedObject);
            
            if (SDL_HasIntersection(&boundingBox, &collidedObjectBoundingBox)) {
                if (child->touch != NULL) {
                    child->touch(child, collidedObject);
                }
            }
        }
    }
    
    for (i = 0; i < childCount; i++) {
        entity_direction collision_direction = 0;
        entity_t *child = g_slist_nth_data(self->children, i);
        
        if (!child) {
            continue;
        }
        
        if (child->touch_world == NULL) {
            continue;
        }
        
        _game_map_check_collision(gameMap, child, ENTITY_DIRECTION_UP, &collision_direction);
        _game_map_check_collision(gameMap, child, ENTITY_DIRECTION_RIGHT, &collision_direction);
        _game_map_check_collision(gameMap, child, ENTITY_DIRECTION_DOWN, &collision_direction);
        _game_map_check_collision(gameMap, child, ENTITY_DIRECTION_LEFT, &collision_direction);
        
        if (collision_direction != 0) {
            child->touch_world(child, collision_direction);
        }
    }
}

void _game_map_render(entity_t *self) {
    game_map_t *gameMap = (game_map_t *)self->entity_data;
    int i, j;
    int layer_size;
    int layer_width;
    SDL_Point frame_size;
    spritesheet_t *spritesheet;
    
    assert(gameMap->tilesheet != NULL);
    
    layer_size = gameMap->layer_width * gameMap->layer_height;
    layer_width = gameMap->layer_width;
    spritesheet = gameMap->tilesheet->spritesheet;
    frame_size = spritesheet->frame_size;
    
    for (i = 0; i < gameMap->layer_count; i++) {
        for (j = 0; j < layer_size; j++) {
            Uint8 tile_index = gameMap->layers[i][j];
            SDL_Rect destRect = graphics_rect_make((j % layer_width) * frame_size.x, floor(j / layer_width) * frame_size.y, frame_size.x, frame_size.y);
            
            spritesheet_render(spritesheet, tile_index, destRect);
        }
    }
}

void _game_map_dealloc(entity_t *self) {
    int i;
    game_map_t *game_map_data = (game_map_t *)self->entity_data;
    
    quadtree_free(game_map_data->quad);
    
    if (game_map_data->tilesheet != NULL) {
        tilesheet_free(game_map_data->tilesheet);
        game_map_data->tilesheet = NULL;
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
    int data_size = SDL_RWsize(fp);
    int current_position = SDL_RWtell(fp);
    entity_t *new_map = NULL;
    game_map_t *new_map_data = NULL;
    int layer_size = 0;
    int current_layer = 0;
    char *tilesheet_filename = NULL;
    int tilesheet_filename_length = 0;
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
                int tilesheet_filename_start_location = SDL_RWtell(fp);
                if (tilesheet_filename_start_location == -1) {
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
                    
                    tilesheet_filename_length++;
                }
                
                if (tilesheet_filename_length <= 0) {
                    SDL_SetError("Invalid file format. No tilemap filename provided.");
                    
                    goto cleanup;
                }
                
                if (SDL_RWseek(fp, tilesheet_filename_start_location, RW_SEEK_SET) == -1) {
                    SDL_SetError("Invalid file format. Could not seek back to beginning of tilemap filename.");
                    
                    goto cleanup;
                }
                
                tilesheet_filename = malloc(sizeof(char) * (tilesheet_filename_length + 1));
                if (SDL_RWread(fp, tilesheet_filename, tilesheet_filename_length, 1) == 0) {
                    SDL_SetError("Unable to read tilemap filename.");
                    
                    goto cleanup;
                }
                
                tilesheet_filename[tilesheet_filename_length] = '\0';
                
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
    
    if (tilesheet_filename == NULL) {
        SDL_SetError("Invalid file format. No tilesheet filename provided.");
        
        goto cleanup;
    }
    
    new_map_data->tilesheet = tilesheet_create(tilesheet_filename);
    if (new_map_data->tilesheet == NULL) {
        SDL_SetError("Could not load tilesheet at %s", tilesheet_filename);
        
        goto cleanup;
    }
    
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
