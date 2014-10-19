/*
//  test_animated_sprites.c
//  zelda
//
//  Created by Grant Butler on 10/8/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include <check.h>
#include "test_support.h"

#include "graphics.h"
#include "animated_sprite.h"

START_TEST(test_animated_sprite_loading)
{
    animated_sprite_t *sprite = animated_sprite_create("res/sprites/link.yaml");
    
    ck_assert_ptr_ne(sprite, NULL);
    
    if (sprite) {
        animated_sprite_free(sprite);
    }
}
END_TEST

void loading_setup(void) {
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);
    SDL_FreeSurface(surface);
    graphics_set_global_renderer(renderer);
}

void loading_teardown(void) {
    SDL_DestroyRenderer(graphics_get_global_renderer());
}

Suite *animated_sprite_suite() {
    Suite *s;
    TCase *tc_loading;
    s = suite_create("Animated Sprite");
    
    tc_loading = tcase_create("Loading");
    tcase_add_checked_fixture(tc_loading, loading_setup, loading_teardown);
    tcase_add_test(tc_loading, test_animated_sprite_loading);
    suite_add_tcase(s, tc_loading);
    
    return s;
}
