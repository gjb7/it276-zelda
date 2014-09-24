//
//  game_map.c
//  zelda
//
//  Created by Grant Butler on 9/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "game_map.h"
#include "entity.h"

void _game_map_dealloc(entity_t *self);
entity_t *_game_map_create_from_map(SDL_RWops *fp);
int _game_map_create_from_v1_map(SDL_RWops *fp, entity_t **game_map);

entity_t *game_map_create(int layer_count, int width, int height) {
    entity_t *game_map = entity_create();
    if (game_map == NULL) {
        return NULL;
    }
    
    game_map_t *game_map_data = malloc(sizeof(game_map_t));
    if (game_map_data == NULL) {
        entity_release(game_map);
        
        return NULL;
    }
    
    game_map_data->layers = malloc(layer_count * sizeof(char *));
    if (game_map_data->layers == NULL) {
        entity_release(game_map);
        
        return NULL;
    }
    
    for (int i = 0; i < layer_count; i++) {
        game_map_data->layers[i] = malloc(height * width * sizeof(char));
        if (game_map_data->layers[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(game_map_data->layers[j]);
            }
            
            free(game_map_data->layers);
            
            entity_release(game_map);
            
            return NULL;
        }
    }
    
    game_map_data->layer_count = layer_count;
    
    game_map->entity_data = (void *)game_map_data;
    
    strcpy(game_map->class_name, "game_map");
    game_map->dealloc = _game_map_dealloc;
    
    return game_map;
}

entity_t *game_map_create_from_file(char *filename) {
    SDL_RWops *fp = SDL_RWFromFile(filename, "rb");
    if (fp == NULL) {
        return NULL;
    }
    
    return _game_map_create_from_map(fp);
}

void _game_map_dealloc(entity_t *self) {
    game_map_t *game_map_data = (game_map_t *)self->entity_data;
    for (int i = 0; i < game_map_data->layer_count; i++) {
        free(game_map_data->layers[i]);
    }
    
    free(game_map_data->layers);
}

#pragma mark - Map Parsing

entity_t *_game_map_create_from_map(SDL_RWops *fp) {
    char buffer;
    if (SDL_RWread(fp, &buffer, sizeof(buffer), 1) == 0) {
        const char *error = SDL_GetError();
        if (error) {
            fprintf(stderr, "Error parsing map file (V): %s\n", error);
        }
        else {
            fprintf(stderr, "Error parsing map file (V): reached end of file.\n");
        }
        
        SDL_RWclose(fp);
        
        return NULL;
    }
    
    if (buffer != 'V') {
        fprintf(stderr, "Invalid file format. Expected 'V', instead got: %c\n", buffer);
        
        SDL_RWclose(fp);
        
        return NULL;
    }
    
    if (SDL_RWread(fp, &buffer, sizeof(buffer), 1) == 0) {
        const char *error = SDL_GetError();
        if (error) {
            fprintf(stderr, "Error parsing map file (version): %s\n", error);
        }
        else {
            fprintf(stderr, "Error parsing map file (version): reached end of file.\n");
        }
        
        SDL_RWclose(fp);
        
        return NULL;
    }
    
    entity_t *game_map = NULL;
    
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
                
                SDL_RWclose(fp);
                
                return NULL;
            }
            
            if (buffer != ';') {
                fprintf(stderr, "Invalid file format. Expected ';', instead got %c\n", buffer);
                
                SDL_RWclose(fp);
                
                return NULL;
            }
            
            if (_game_map_create_from_v1_map(fp, &game_map) != 0) {
                const char *error = SDL_GetError();
                if (error) {
                    fprintf(stderr, "Error parsing map file (map): %s\n", error);
                }
                else {
                    fprintf(stderr, "Error parsing map file (map): reached end of file.\n");
                }
                
                return NULL;
            }
            
            break;
        default:
            fprintf(stderr, "Invalid version number found. Expected '1', instead got %c\n", buffer);
            
            SDL_RWclose(fp);
            
            return NULL;
    }
    
    return game_map;
}

int _game_map_create_from_v1_map(SDL_RWops *fp, entity_t **game_map) {
    char width = 0;
    char height = 0;
    char layer_count = 0;
    int data_size = SDL_RWsize(fp);
    int current_position = SDL_RWtell(fp);
    entity_t *new_map = NULL;
    game_map_t *new_map_data = NULL;
    int layer_size, current_layer = 0;
    
    while (current_position < data_size) {
        char buffer;
        int result = SDL_RWread(fp, &buffer, sizeof(buffer), 1);
        if (result == 0) {
            SDL_SetError("Error trying to read a char");
            
            return result;
        }
        
        switch (buffer) {
            case 'W':
                if (width != 0) {
                    SDL_SetError("Attempting to set width, when width already set to %i", width);
                    
                    // TODO: Possibly define constants for these error codes?
                    return -1;
                }
                
                result = SDL_RWread(fp, &width, sizeof(width), 1);
                if (result == 0) {
                    SDL_SetError("Error trying to read width");
                    
                    return result;
                }
                break;
                
            case 'H':
                if (height != 0) {
                    SDL_SetError("Attempting to set height, when height already set to %i", height);
                    
                    // TODO: Possibly define constants for these error codes?
                    return -1;
                }
                
                result = SDL_RWread(fp, &height, sizeof(height), 1);
                if (result == 0) {
                    SDL_SetError("Error trying to read height");
                    
                    return result;
                }
                break;
            
            case 'L':
                if (layer_count != 0) {
                    SDL_SetError("Attempting to set layer_count, when layer_count already set to %i", layer_count);
                    
                    // TODO: Possibly define constants for these error codes?
                    return -1;
                }
                
                result = SDL_RWread(fp, &layer_count, sizeof(layer_count), 1);
                if (result == 0) {
                    SDL_SetError("Error trying to read layer_count");
                    
                    return result;
                }
                break;
            
            case 'l':
                if (width == 0 || height == 0 || layer_count == 0) {
                    SDL_SetError("Missing required data.");
                    
                    // TODO: Possibly define constants for these error codes?
                    return -1;
                }
                
                if (new_map == NULL) {
                    new_map = game_map_create(layer_count, width, height);
                    new_map_data = (game_map_t *)new_map->entity_data;
                    layer_size = width * height;
                }
                
                result = SDL_RWread(fp, new_map_data->layers[current_layer], layer_size, 1);
                if (result < layer_size) {
                    SDL_SetError("Invalid file format. Layer at index %i smaller than specified width and height.", current_layer);
                    
                    entity_release(new_map);
                    
                    return -1;
                }
                
                current_layer++;
                
                break;
                
            default:
                SDL_SetError("Invalid file format. Expected 'W', 'H', 'L', or 'l', instead got %c", buffer);
                
                if (new_map != NULL) {
                    entity_release(new_map);
                }
                
                return -1;
                
                break;
        }
        
        result = SDL_RWread(fp, &buffer, sizeof(buffer), 1);
        if (result == 0) {
            SDL_SetError("Error trying to read a ';'");
            
            return result;
        }
        
        if (buffer != ';') {
            SDL_SetError("Invalid file format. Expected ';', instead got %c", buffer);
            
            return -1;
        }
        
        current_position = SDL_RWtell(fp);
    }
    
    if (current_layer < layer_count) {
        SDL_SetError("Invalid file format. Found %i layers when %i specified", current_layer, layer_count);
        
        entity_release(new_map);
        
        return -1;
    }
    
    *game_map = new_map;
    
    return 0;
}
