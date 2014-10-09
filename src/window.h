/*
//  window.h
//  zelda
//
//  Created by Grant Butler on 9/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef __zelda__window__
#define __zelda__window__

#include <stdlib.h>
#include "sdl.h"

typedef struct window_w {
    SDL_Window *window;
    SDL_Renderer *renderer;
} window_t;

window_t *window_create(char *title, int width, int height);
void window_free(window_t *window);

#endif /* defined(__zelda__window__) */
