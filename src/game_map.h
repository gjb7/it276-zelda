//
//  game_map.h
//  zelda
//
//  Created by Grant Butler on 9/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__game_map__
#define __zelda__game_map__

#include <stdlib.h>
#include "entity.h"
#include "sdl.h"
#include "sprite.h"

typedef struct game_map_s {
    char *tilemap_filename;
    sprite_t *tilemap;
    char **layers;
    int layer_count;
} game_map_t;

entity_t *game_map_create(int layer_count, int width, int height);
entity_t *game_map_create_from_file(char *filename);

#endif /* defined(__zelda__game_map__) */
