//
//  graphics.c
//  zelda
//
//  Created by Grant Butler on 9/21/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "sdl.h"
#include "graphics.h"

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
