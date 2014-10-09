/*
//  graphics.h
//  zelda
//
//  Created by Grant Butler on 9/21/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef __zelda__graphics__
#define __zelda__graphics__

#include <stdlib.h>
#include <stdbool.h>
#include "sdl.h"

bool init_sdl(void);
bool init_image(void);

void graphics_frame_delay(Uint8 delay);

void graphics_set_global_renderer(SDL_Renderer *renderer);
SDL_Renderer *graphics_get_global_renderer(void);

SDL_Point graphics_global_renderer_size(void);

#endif /* defined(__zelda__graphics__) */
