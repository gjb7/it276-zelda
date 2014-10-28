/*
//  tilesheet.c
//  zelda
//
//  Created by Grant Butler on 10/25/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "tilesheet.h"
#include "graphics.h"
#include <yaml.h>

const int tilesheet_frame_width = 16;
const int tilesheet_frame_height = 16;

bool load_tiles(yaml_parser_t *parser, tilesheet_t *tilesheet);
bool load_tilesheet_from_yaml_file(char *filename, tilesheet_t *tilesheet);

void tilesheet_free(tilesheet_t *tilesheet);

void hash_table_tilesheet_free_tile(void *gpointer) {
    free(gpointer);
}

void hash_table_tilesheet_key_free(void *gpointer) {
    free(gpointer);
}

tilesheet_t *tilesheet_create(char *filename) {
    tilesheet_t *tilesheet = malloc(sizeof(tilesheet_t));
    if (tilesheet == NULL) {
        return NULL;
    }
    
    memset(tilesheet, 0, sizeof(tilesheet_t));
    
    tilesheet->tiles = g_hash_table_new_full(g_int_hash, g_int_equal, hash_table_tilesheet_key_free, hash_table_tilesheet_free_tile);
    
    if (!load_tilesheet_from_yaml_file(filename, tilesheet)) {
        tilesheet_free(tilesheet);
        
        return NULL;
    }
    
    return tilesheet;
}

void tilesheet_free(tilesheet_t *tilesheet) {
    g_hash_table_destroy(tilesheet->tiles);
    
    if (tilesheet->sprite) {
        sprite_free(tilesheet->sprite);
    }
    
    free(tilesheet);
}

bool load_tilesheet_from_yaml_file(char *filename, tilesheet_t *tilesheet) {
    yaml_parser_t parser;
    yaml_event_t  event;
    FILE *input;
    char *currentKey = NULL;
    
    yaml_parser_initialize(&parser);
    
    input = fopen(filename, "rb");
    if (input == NULL) {
        goto error;
    }
    
    yaml_parser_set_input_file(&parser, input);
    
    do {
        int handledValue = 0;
        
        if (!yaml_parser_parse(&parser, &event)) {
            break;
        }
        
        switch (event.type) {
            case YAML_SEQUENCE_START_EVENT:
                if (strcmp(currentKey, "tiles") == 0) {
                    load_tiles(&parser, tilesheet);
                    
                    handledValue = 1;
                }
                
                break;
            case YAML_SCALAR_EVENT:
                if (!currentKey) {
                    char *aKey = (char *)event.data.scalar.value;
                    int length = (strlen(aKey) + 1);
                    
                    currentKey = malloc(length * sizeof(char));
                    strncpy(currentKey, aKey, length);
                    currentKey[length - 1] = '\0';
                }
                else {
                    if (strcmp(currentKey, "sprite") == 0) {
                        tilesheet->sprite = sprite_create((char *)event.data.scalar.value, graphics_point_make(tilesheet_frame_width, tilesheet_frame_height));
                        if (tilesheet->sprite == NULL) {
                            goto error;
                        }
                        
                        handledValue = 1;
                    }
                }
                
                break;
            default:
                break;
        }
        
        if (handledValue) {
            free(currentKey);
            currentKey = NULL;
            
            handledValue = 0;
        }
        
        if (event.type != YAML_STREAM_END_EVENT) {
            yaml_event_delete(&event);
        }
    } while (event.type != YAML_STREAM_END_EVENT);
    
    yaml_event_delete(&event);
    
    yaml_parser_delete(&parser);
    
    fclose(input);
    
    return true;
    
error:
    yaml_event_delete(&event);
    yaml_parser_delete(&parser);
    fclose(input);
    
    return false;
}

bool load_tiles(yaml_parser_t *parser, tilesheet_t *tilesheet) {
    yaml_event_t event;
    char *currentKey = NULL;
    tile_t *tile = NULL;
    int tile_index = -1;
    
    do {
        int handledValue = 0;
        
        if (!yaml_parser_parse(parser, &event)) {
            break;
        }
        
        switch (event.type) {
            case YAML_MAPPING_START_EVENT:
                tile = malloc(sizeof(tile_t));
                memset(tile, 0, sizeof(tile_t));
                
                tile->collision_type = TILE_COLLISION_TYPE_NONE;
                
                break;
                
            case YAML_MAPPING_END_EVENT:
                if (tile_index == -1) {
                    goto error;
                }
                
                int *key = malloc(sizeof(int));
                *key = tile_index;
                
                g_hash_table_insert(tilesheet->tiles, key, tile);
                
                break;
                
            case YAML_SCALAR_EVENT:
                if (!currentKey) {
                    char *aKey = (char *)event.data.scalar.value;
                    int length = (strlen(aKey) + 1);
                    
                    currentKey = malloc(length * sizeof(char));
                    strncpy(currentKey, aKey, length);
                    currentKey[length - 1] = '\0';
                }
                else {
                    if (strcmp(currentKey, "index") == 0) {
                        tile_index = atoi((char *)event.data.scalar.value);
                    }
                    else if (strcmp(currentKey, "collision_type") == 0) {
                        tile->collision_type = atoi((char *)event.data.scalar.value);
                    }
                    
                    handledValue = 1;
                }
                
                break;
            default:
                break;
        }
        
        if (handledValue) {
            free(currentKey);
            currentKey = NULL;
            
            handledValue = 0;
        }
        
        if (event.type != YAML_SEQUENCE_END_EVENT) {
            yaml_event_delete(&event);
        }
    } while (event.type != YAML_SEQUENCE_END_EVENT);
    
    yaml_event_delete(&event);
    
    return true;
    
error:
    
    if (currentKey != NULL) {
        free(currentKey);
    }
    
    free(tile);
    yaml_event_delete(&event);
    
    return false;
}
