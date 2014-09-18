//
//  entity.h
//  zelda
//
//  Created by Grant Butler on 9/17/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__entity__
#define __zelda__entity__

#include <libc.h>
#include <glib.h>

/**
 * An entity within the game. An entity is anything that acts or is acted upon by the game.
 */
typedef struct entity_t {
    /// The parent of this entity. If the parent is null, then this is the root entity.
    struct entity_t *parent;
    
    GSList *children;
    
    /// The retain count of this entity. When it reaches zero, the entity's dealloc method is run, and the entity is freed.
    /// The developer should NOT touch this.
    int retain_count;
    
    void (*think)(struct entity_t *self);
    void (*render)(struct entity_t *self); // TODO: a rendering context type object should be passed in.
    void (*update)(struct entity_t *self);
    void (*dealloc)(struct entity_t *self);
} entity;

entity *entity_create();
void entity_retain(entity *e);
void entity_release(entity *e);

void entity_add_child(entity *parent, entity *child);
void entity_remove_from_parent(entity *e);

#endif /* defined(__zelda__entity__) */
