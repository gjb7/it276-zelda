//
//  entity.c
//  zelda
//
//  Created by Grant Butler on 9/17/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "entity.h"
#include "sdl.h"
#include "graphics.h"
#include "logging.h"
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
    assert(e != NULL);
    
    if (e->parent == NULL) {
        return;
    }
    
    entity_t *parent = e->parent;
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
    
    if (e->render != NULL) {
        SDL_Point rendererSize = graphics_global_renderer_size();
        SDL_Rect viewportSize;
        viewportSize.x = e->position.x;
        viewportSize.y = e->position.y;
        viewportSize.w = rendererSize.x;
        viewportSize.h = rendererSize.y;
        
        if (e->parent) {
            entity_t *parent = e->parent;
            
            do {
                viewportSize.x += parent->position.x;
                viewportSize.y += parent->position.y;
                
                parent = parent->parent;
            } while(parent != NULL);
        }
        
        SDL_RenderSetViewport(graphics_get_global_renderer(), &viewportSize);
        
        e->render(e);
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
