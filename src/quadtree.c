//
//  quadtree.c
//  zelda
//
//  Created by Grant Butler on 10/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#define INLINE

#include "quadtree.h"
#include "graphics.h"

#include <stdbool.h>

static int MAX_OBJECTS = 10;
static int MAX_LEVELS = 5;

quadtree_t *quadtree_create(int level, SDL_Rect rect) {
    int i;
    
    quadtree_t *quadtree = malloc(sizeof(quadtree_t));
    if (!quadtree) {
        return NULL;
    }
    
    quadtree->level = level;
    quadtree->bounds = rect;
    quadtree->objects = NULL;
    
    for (i = 0; i < QUADTREE_NODES_LENGTH; i++) {
        quadtree->nodes[i] = NULL;
    }
    
    return quadtree;
}

void quadtree_free(quadtree_t *quadtree) {
    quadtree_clear(quadtree);
    
    free(quadtree);
}

void quadtree_clear(quadtree_t *quadtree) {
    int i;
    
    if (quadtree->objects != NULL) {
        g_slist_free(quadtree->objects);
        quadtree->objects = NULL;
    }
    
    for (i = 0; i < QUADTREE_NODES_LENGTH; i++) {
        if (quadtree->nodes[i] != NULL) {
            quadtree_free(quadtree->nodes[i]);
            quadtree->nodes[i] = NULL;
        }
    }
}

void quadtree_split(quadtree_t *quadtree) {
    int subWidth = (int)(quadtree->bounds.w / 2);
    int subHeight = (int)(quadtree->bounds.h / 2);
    int x = quadtree->bounds.x;
    int y = quadtree->bounds.y;
    int subLevel = quadtree->level + 1;
    
    quadtree->nodes[0] = quadtree_create(subLevel, graphics_rect_make(x + subWidth, y, subWidth, subHeight));
    quadtree->nodes[1] = quadtree_create(subLevel, graphics_rect_make(x, y, subWidth, subHeight));
    quadtree->nodes[2] = quadtree_create(subLevel, graphics_rect_make(x, y + subWidth, subWidth, subHeight));
    quadtree->nodes[3] = quadtree_create(subLevel, graphics_rect_make(x + subWidth, y + subWidth, subWidth, subHeight));
}

int quadtree_get_index(quadtree_t *quadtree, entity_t *entity) {
    int index = -1;
    double verticalMidpoint = quadtree->bounds.y + (quadtree->bounds.h / 2.0);
    double horizontalMidpoint = quadtree->bounds.x + (quadtree->bounds.w / 2.0);
    SDL_Rect rect = entity_get_collision_box(entity);
    
    bool isInTopQuadrant = (rect.y < horizontalMidpoint && rect.y + rect.h < horizontalMidpoint);
    bool isInBottomQuadrant = (rect.y > horizontalMidpoint);
    bool isInLeftQuadrant = (rect.x < verticalMidpoint && rect.x + rect.w < horizontalMidpoint);
    bool isInRightQuadrant = (rect.x > verticalMidpoint);
    
    if (isInLeftQuadrant) {
        if (isInTopQuadrant) {
            index = 1;
        }
        else if (isInBottomQuadrant) {
            index = 2;
        }
    }
    else if (isInRightQuadrant) {
        if (isInTopQuadrant) {
            index = 0;
        }
        else if (isInBottomQuadrant) {
            index = 3;
        }
    }
    
    return index;
}

void quadtree_insert(quadtree_t *quadtree, entity_t *entity) {
    if (quadtree->nodes[0] != NULL) {
        int index = quadtree_get_index(quadtree, entity);
        
        if (index != -1) {
            quadtree_insert(quadtree->nodes[index], entity);
            
            return;
        }
    }
    
    quadtree->objects = g_slist_append(quadtree->objects, entity);
    
    if (g_slist_length(quadtree->objects) > MAX_OBJECTS && quadtree->level < MAX_LEVELS) {
        int i = 0;
        
        if (quadtree->nodes[0] == NULL) {
            quadtree_split(quadtree);
        }
        
        while (i < g_slist_length(quadtree->objects)) {
            entity_t *anEntity = g_slist_nth_data(quadtree->objects, i);
            int index = quadtree_get_index(quadtree, anEntity);
            if (index != -1) {
                entity_t *removedEntity = (entity_t *)g_slist_nth_data(quadtree->objects, i);
                quadtree->objects = g_slist_remove(quadtree->objects, removedEntity);
                
                quadtree_insert(quadtree->nodes[index], removedEntity);
            }
            else {
                i++;
            }
        }
    }
}

GSList *quadtree_retrieve(quadtree_t *quadtree, GSList *returnObjects, entity_t *entity) {
    int index = quadtree_get_index(quadtree, entity);
    if (index != -1 && quadtree->nodes[0] != NULL) {
        returnObjects = quadtree_retrieve(quadtree->nodes[index], returnObjects, entity);
    }
    
    returnObjects = g_slist_concat(returnObjects, quadtree->objects);
    
    return returnObjects;
}
