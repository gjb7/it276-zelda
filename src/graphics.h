//
//  graphics.h
//  zelda
//
//  Created by Grant Butler on 9/21/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__graphics__
#define __zelda__graphics__

#include <stdlib.h>
#include <stdbool.h>

bool init_sdl(void);
bool init_image(void);

void graphics_frame_delay(Uint8 delay);

#endif /* defined(__zelda__graphics__) */
