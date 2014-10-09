/*
//  input.h
//  zelda
//
//  Created by Grant Butler on 9/29/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef __zelda__input__
#define __zelda__input__

#include <stdlib.h>
#include <stdbool.h>
#include "sdl.h"

bool init_input(void);
void input_update(void);
bool input_is_key_down(Uint16 key);
bool input_is_key_up(Uint16 key);

#endif /* defined(__zelda__input__) */
