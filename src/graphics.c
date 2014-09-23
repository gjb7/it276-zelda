//
//  graphics.c
//  zelda
//
//  Created by Grant Butler on 9/21/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "graphics.h"

int init_sdl(void) {
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    if (result != 0) {
        return result;
    }
    atexit(SDL_Quit);
    return result;
}
