//
//  entity.c
//  zelda
//
//  Created by Grant Butler on 9/17/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "entity.h"
#include <assert.h>

entity_t *entity_create() {
    entity_t *e = malloc(sizeof(entity_t));
    if (e == NULL) {
        return NULL;
    }
    
    memset(e, 0, sizeof(entity_t));
    e->retain_count = 1;
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
