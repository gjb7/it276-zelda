/*
//  resource.h
//  zelda
//
//  Created by Grant Butler on 10/19/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef __zelda__resource__
#define __zelda__resource__

#include <stdbool.h>
#include "sdl.h"

typedef enum resource_type {
    RESOURCE_TYPE_UNKNOWN = 0,
    RESOURCE_TYPE_IMAGE = 1,
    RESOURCE_TYPE_AUDIO = 2
} resource_type;

typedef struct audio_resource_s {
    /* Add an unused field so the compiler won't complain. */
    int unused;
} audio_resource_t;

typedef struct image_resource_s {
    SDL_Texture *texture;
} image_resource_t;

typedef union resource_s {
    int retain_count;
    
    resource_type type;
    
    image_resource image;
    audio_resource audio;
} resource_t;

bool resource_init(void);

resource_t *resource_load(char *file, resource_type type);

void resource_retain(resource_t *resource);
void resource_release(resource_t *resource);

#endif /* defined(__zelda__resource__) */
