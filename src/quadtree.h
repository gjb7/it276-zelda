//
//  quadtree.h
//  zelda
//
//  Created by Grant Butler on 10/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__quadtree__
#define __zelda__quadtree__

#include <stdio.h>
#include <glib.h>
#include "sdl.h"
#include "entity.h"

#define QUADTREE_NODES_LENGTH 4

typedef struct quadtree_s {
    int level;
    GSList *objects;
    SDL_Rect bounds;
    struct quadtree_s *nodes[QUADTREE_NODES_LENGTH];
} quadtree_t;

quadtree_t *quadtree_create(int level, SDL_Rect rect);
void quadtree_clear(quadtree_t *quadtree);
void quadtree_insert(quadtree_t *quadtree, entity_t *entity);
GSList *quadtree_retrieve(quadtree_t *quadtree, GSList *returnObjects, entity_t *entity);

#endif /* defined(__zelda__quadtree__) */
