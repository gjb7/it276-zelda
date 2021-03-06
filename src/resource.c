/*
//  resource.c
//  zelda
//
//  Created by Grant Butler on 10/19/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "resource.h"
#include "graphics.h"

#include <stdio.h>
#include <glib.h>
#include <assert.h>

static GHashTable *_resource_table = NULL;

void _resource_quit(void);
void _resource_free(resource_t *resource);

bool init_resource(void) {
    _resource_table = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);
    
    atexit(_resource_quit);
    
    return true;
}

void _resource_quit(void) {
    g_hash_table_destroy(_resource_table);
}

resource_t *resource_load(char *file, resource_type type) {
    assert(_resource_table != NULL);
    
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
                
                resource->data.image.texture = IMG_LoadTexture(renderer, file);
                if (!resource->data.image.texture) {
                    goto cleanup;
                }
                
                break;
            }
            default:
                fprintf(stderr, "Unable to initialize resource type: %i", type);
                
                goto cleanup;
                
                break;
        }
        
        g_hash_table_insert(_resource_table, file, resource);
    }
    
    resource_retain(resource);
    
    return resource;

cleanup:
    free(resource);
    
    return NULL;
}

void resource_retain(resource_t *resource) {
    assert(resource != NULL);
    
    resource->retain_count++;
}

gboolean _resource_remove(gpointer key, gpointer value, gpointer user_data) {
    return (gboolean)(value == user_data);
}

void resource_release(resource_t *resource) {
    assert(resource != NULL);
    
    if (--resource->retain_count == 0) {
        switch (resource->type) {
            case RESOURCE_TYPE_IMAGE:
                SDL_DestroyTexture(resource->data.image.texture);
                
                break;
                
            default:
                fprintf(stderr, "Don't know how to dealloc resource of type: %i", resource->type);
                
                break;
        }
        
        g_hash_table_foreach_remove(_resource_table, _resource_remove, resource);
        
        free(resource);
    }
}
