//
//  input.c
//  zelda
//
//  Created by Grant Butler on 9/29/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "input.h"
#include "sdl.h"

const Uint8 *_input_keyboard_state = NULL;

bool init_input(void) {
    _input_keyboard_state = SDL_GetKeyboardState(NULL);
    
    return true;
}

void input_update(void) {
    SDL_PumpEvents();
}

bool input_is_key_down(Uint16 key) {
    // TODO: Add some safety checks.
    
    return (_input_keyboard_state[key] == 1);
}

bool input_is_key_up(Uint16 key) {
    // TODO: Add some safety checks.
    
    return (_input_keyboard_state[key] == 0);
}
