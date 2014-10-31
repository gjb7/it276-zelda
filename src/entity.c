/*
//  entity.c
//  zelda
//
//  Created by Grant Butler on 9/17/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "entity.h"
#include "sdl.h"
#include "graphics.h"
#include "logging.h"
#include "debug.h"
#include <assert.h>

entity_t *entity_create() {
    entity_t *e = malloc(sizeof(entity_t));
    if (e == NULL) {
        return NULL;
    }
    
    memset(e, 0, sizeof(entity_t));
    e->retain_count = 1;
    
    e->position.x = 0;
    e->position.y = 0;
    
    return e;
}

void entity_retain(entity_t *e) {
    assert(e != NULL);
    
    e->retain_count++;
}

void _entity_free_children(entity_t *e) {
    e->parent = NULL;
    entity_release(e);
}

void entity_dealloc(entity_t *e) {
    assert(e != NULL);
    
    g_slist_free_full(e->children, (GDestroyNotify)_entity_free_children);
    
    if (e->dealloc != NULL) {
        e->dealloc(e);
    }
}

void entity_release(entity_t *e) {
    assert(e != NULL);
    
    if (--e->retain_count == 0) {
        entity_dealloc(e);
        
        free(e);
    }
}

void entity_add_child(entity_t *parent, entity_t *child) {
    assert(parent != NULL);
    assert(child != NULL);
    
    entity_retain(child);
    
    if (child->parent != NULL) {
        entity_remove_from_parent(child);
    }
    
    parent->children = g_slist_append(parent->children, child);
    child->parent = parent;
}

void entity_remove_from_parent(entity_t *e) {
    entity_t *parent;
    
    assert(e != NULL);
    
    if (e->parent == NULL) {
        return;
    }
    
    parent = e->parent;
    parent->children = g_slist_remove(parent->children, e);
    e->parent = NULL;
    entity_release(e);
}

void _entity_think_iterator(gpointer data, gpointer user_data) {
    entity_t *e = (entity_t *)data;
    
    entity_think(e);
}

void entity_think(entity_t *e) {
    assert(e != NULL);
    
    if (e->think != NULL) {
        if (SDL_GetTicks() > e->thinkNext) {
            e->think(e);
            
            if (e->thinkRate > 0) {
                e->thinkNext += e->thinkRate;
            }
        }
    }
    
    g_slist_foreach(e->children, _entity_think_iterator, NULL);
}

void _entity_render_iterator(gpointer data, gpointer user_data) {
    entity_t *e = (entity_t *)data;
    
    entity_render(e);
}

void entity_render(entity_t *e) {
    assert(e != NULL);
    
    SDL_Point absolutePosition = entity_get_absolute_position(e);
    SDL_Point rendererSize = graphics_global_renderer_size();
    SDL_Rect viewportSize = graphics_rect_make(absolutePosition.x, absolutePosition.y, rendererSize.x, rendererSize.y);
    
    SDL_RenderSetViewport(graphics_get_global_renderer(), &viewportSize);
    
    if (e->render != NULL) {
        e->render(e);
    }
    
    if (debug_get_render_bounding_boxes()) {
        if (e->bounding_box.w > 0 && e->bounding_box.h > 0) {
            SDL_Renderer *renderer = graphics_get_global_renderer();
            
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 96);
            SDL_RenderFillRect(renderer, &(e->bounding_box));
        }
    }
    
    g_slist_foreach(e->children, _entity_render_iterator, NULL);
}

void _entity_update_iterator(gpointer data, gpointer user_data) {
    entity_t *e = (entity_t *)data;
    
    entity_update(e);
}

void entity_update(entity_t *e) {
    assert(e != NULL);
    
    if (e->update != NULL) {
        e->update(e);
    }
    
    g_slist_foreach(e->children, _entity_update_iterator, NULL);
}

SDL_Point entity_get_absolute_position(entity_t *e) {
    SDL_Point position = e->position;
    
    if (e->parent) {
        entity_t *parent = e->parent;
        
        do {
            position.x += parent->position.x;
            position.y += parent->position.y;
            
            parent = parent->parent;
        } while(parent != NULL);
    }
    
    return position;
}

SDL_Rect entity_get_bounding_box(entity_t *e) {
    SDL_Point absolutePosition = entity_get_absolute_position(e);
    SDL_Rect boundingBox = graphics_rect_make(absolutePosition.x + e->bounding_box.x, absolutePosition.y + e->bounding_box.y, e->bounding_box.w, e->bounding_box.h);
    return boundingBox;
}
