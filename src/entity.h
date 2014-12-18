/*
//  entity.h
//  zelda
//
//  Created by Grant Butler on 9/17/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef __zelda__entity__
#define __zelda__entity__

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include "sdl.h"

#define ENTITY_CLASS_NAME_LENGTH 128

typedef enum {
    ENTITY_DIRECTION_DOWN = 1 << 0,
    ENTITY_DIRECTION_LEFT = 1 << 1,
    ENTITY_DIRECTION_UP = 1 << 2,
    ENTITY_DIRECTION_RIGHT = 1 << 3
} entity_direction;

/**
 * An entity within the game. An entity is anything that acts or is acted upon by the game.
 */
typedef struct entity_s {
    /** The parent of this entity. If the parent is null, then this is the root entity. */
    struct entity_s *parent;
    
    GSList *children;
    
    /**
     * The retain count of this entity. When it reaches zero, the entity's dealloc method is run, and the entity is freed.
     * The developer should NOT touch this.
     */
    int retain_count;
    
    /** Thinking for the entity. Useful for animations. */
    void (*think)(struct entity_s *self);
    Sint32 thinkRate;
    Uint32 thinkNext;
    
    void (*render)(struct entity_s *self);
    
    /** Updating for the entity. Used for updating position. */
    void (*update)(struct entity_s *self);
    void (*dealloc)(struct entity_s *self);
    
    void (*touch)(struct entity_s *self, struct entity_s *other);
    void (*touch_world)(struct entity_s *self, entity_direction direction);
    
    void (*die)(struct entity_s *self);
    
    char class_name[ENTITY_CLASS_NAME_LENGTH];
    
    SDL_Point position;
    SDL_Rect bounding_box;
    
    entity_direction facing;
    
    /**
     * How often a drop should happen. Once every `drop_frequency` times.
     */
    int drop_frequency;
    
    /**
     * List of all valid drops this entity can spawn.
     */
    GList *drops;
    
    /**
     * Whether the entity was hit recently. Gets set to false after the knockback has finished being applied.
     */
    bool is_hit;
    
    /**
     * How much knockback should be applied in each direction.
     */
    SDL_Point knockback;
    
    /**
     * Tick interval that the knockback should be applied.
     */
    int knockback_step;
    
    /**
     * How long before the knock back effect wears off.
     *
     * Decremented on each update call. When it reaches 0, no more knockback.
     */
    int knockback_cooldown;
    
    int health;
    int max_health;
    
    void *entity_data;
} entity_t;

entity_t *entity_create();
void entity_retain(entity_t *e);
void entity_release(entity_t *e);

void entity_add_child(entity_t *parent, entity_t *child);
void entity_remove_from_parent(entity_t *e);

void entity_think(entity_t *e);
void entity_render(entity_t *e);
void entity_update(entity_t *e);

SDL_Point entity_get_absolute_position(entity_t *e);

/**
 Returns the bounding box adjueted with the entity's position.
 */
SDL_Rect entity_get_bounding_box(entity_t *e);

#endif /* defined(__zelda__entity__) */
