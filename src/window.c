//
//  window.c
//  zelda
//
//  Created by Grant Butler on 9/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "window.h"

window_t *window_create(char *title, int width, int height) {
    window_t *window = malloc(sizeof(window_t));
    if (window == NULL) {
        return NULL;
    }
    
    memset(window, 0, sizeof(window_t));
    
    window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window->window == NULL) {
        free(window);
        return NULL;
    }
    
    window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
    if (window->renderer == NULL) {
        SDL_DestroyWindow(window->window);
        free(window);
    }
    
    return window;
}

void window_free(window_t *window) {
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    free(window);
}
