/*
//  graphics.c
//  zelda
//
//  Created by Grant Butler on 9/21/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "sdl.h"
#include "graphics.h"
#include <assert.h>

static SDL_Renderer *_graphics_global_renderer;
static SDL_Point _graphics_global_renderer_size;

bool init_sdl(void) {
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    if (result != 0) {
        return false;
    }
    atexit(SDL_Quit);
    return true;
}

bool init_image(void) {
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        return false;
    }
    atexit(IMG_Quit);
    
    return true;
}

void graphics_frame_delay(Uint8 delay) {
    static Uint8 pass = 100;
    Uint8 diff;
    diff = SDL_GetTicks() - pass;
    if (diff < delay) {
        SDL_Delay(delay - diff);
    }
    pass = SDL_GetTicks();
}

void graphics_set_global_renderer(SDL_Renderer *renderer) {
    int w, h;
    
    assert(renderer != NULL);
    
    _graphics_global_renderer = renderer;
    
    
    if (SDL_GetRendererOutputSize(renderer, &w, &h) >= 0) {
        _graphics_global_renderer_size = graphics_point_make(w, h);
    }
}

SDL_Renderer *graphics_get_global_renderer(void) {
    SDL_Renderer *renderer = _graphics_global_renderer;
    
    assert(renderer != NULL);
    
    return renderer;
}

SDL_Point graphics_global_renderer_size(void) {
    return _graphics_global_renderer_size;
}
