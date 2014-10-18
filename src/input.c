/*
//  input.c
//  zelda
//
//  Created by Grant Butler on 9/29/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "input.h"
#include "sdl.h"
#include <assert.h>

static const Uint8 *_input_keyboard_state = NULL;
static Uint8 *_input_last_keyboard_state = NULL;
static int _input_keyboard_num_keys;

bool init_input(void) {
    _input_keyboard_state = SDL_GetKeyboardState(&_input_keyboard_num_keys);
    _input_last_keyboard_state = malloc(sizeof(Uint8) * _input_keyboard_num_keys);
    if (_input_last_keyboard_state == NULL) {
        /** TODO: Add an error. */
        
        return false;
    }
    
    memcpy(_input_last_keyboard_state, _input_keyboard_state, sizeof(Uint8) * _input_keyboard_num_keys);
    
    return true;
}

void input_update(void) {
    int i;
    
    assert(_input_keyboard_state != NULL);
    
    for (i = 0; i < _input_keyboard_num_keys; i++) {
        _input_last_keyboard_state[i] = _input_keyboard_state[i];
    }
    
    SDL_PumpEvents();
}

bool input_is_key_down(Uint16 key) {
    /** TODO: Add some safety checks. */
    
    return (_input_keyboard_state[key] == 1);
}

bool input_is_key_up(Uint16 key) {
    /** TODO: Add some safety checks. */
    
    return (_input_keyboard_state[key] == 0);
}
