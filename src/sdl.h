//
//  sdl.h
//  zelda
//
//  Created by Grant Butler on 9/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef zelda_sdl_h
#define zelda_sdl_h

#if ((defined __MACH__) && (defined __APPLE__))

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#elif defined(unix) || defined(__unix__)

#include <SDL.h>
#include <SDL_image.h>

#endif

#endif
