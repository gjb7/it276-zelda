/*
//  resource.c
//  zelda
//
//  Created by Grant Butler on 10/19/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "resource.h"
#include "graphics.h"

#include <glib.h>
#include <assert.h>

static GHashTable *_resource_table = NULL;

void _resource_free(resource_t *resource);

bool resource_init(void) {
    _resource_table = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);
    
    return true;
}

resource_t *resource_load(char *file, resource_type type) {
    resource_t *resource = g_hash_table_lookup(_resource_table, file);
    
    if (!resource) {
        resource = malloc(sizeof(resource_t));
        if (!resource) {
            return NULL;
        }
        
        memset(resource, 0, sizeof(resource_t));
        
        resource->type = type;
        
        switch (type) {
            case RESOURCE_TYPE_IMAGE: {
                SDL_Renderer *renderer = graphics_get_global_renderer();
                
                resource->image.texture = IMG_LoadTexture(renderer, file);
                
                break;
            }
            default:
                // TODO: Do an error.
                
                free(resource);
                
                return NULL;
                
                break;
        }
    }
    
    resource_retain(resource);
    
    return resource;
}

void resource_retain(resource_t *resource) {
    assert(resource != NULL);
    
    resource->retain_count++;
}

void resource_release(resource_t *resource) {
    assert(resource != NULL);
    
    if (--resource->retain_count == 0) {
        switch (resource->type) {
            case RESOURCE_TYPE_IMAGE:
                SDL_DestroyTexture(resource->image.texture);
                
                break;
                
            default:
                // TODO: Log a message that says that we can't handle an unknown type.
                
                break;
        }
        
        free(resource);
    }
}
