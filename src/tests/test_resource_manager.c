/*
//  test_resource_manager.c
//  zelda
//
//  Created by Grant Butler on 10/19/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include <check.h>
#include "test_support.h"

#include "graphics.h"
#include "resource.h"

START_TEST(test_resource_loading)
{
    resource_t *resource = resource_load("res/sprites/link.png", RESOURCE_TYPE_IMAGE);
    
    ck_assert_ptr_ne(resource, NULL);
    ck_assert_int_eq(resource->type, RESOURCE_TYPE_IMAGE);
    ck_assert_int_eq(resource->retain_count, 1);
    ck_assert_ptr_ne(resource->data.image.texture, NULL);
    
    resource_release(resource);
}
END_TEST

START_TEST(test_resource_double_loading)
{
    resource_t *resourceA = resource_load("res/sprites/link.png", RESOURCE_TYPE_IMAGE);
    resource_t *resourceB = resource_load("res/sprites/link.png", RESOURCE_TYPE_IMAGE);
    
    ck_assert_ptr_eq(resourceA, resourceB);
    ck_assert_int_eq(resourceA->retain_count, 2);
    ck_assert_int_eq(resourceB->retain_count, 2);
    
    resource_release(resourceA);
    resource_release(resourceB);
}
END_TEST

START_TEST(test_resource_retain)
{
    resource_t *resource = resource_load("res/sprites/link.png", RESOURCE_TYPE_IMAGE);
    resource_retain(resource);
    
    ck_assert_int_eq(resource->retain_count, 2);
    
    resource_release(resource);
    resource_release(resource);
}
END_TEST

void resource_setup(void) {
    if (!init_resource()) {
        ck_abort_msg("Could not setup resource manager.");
    }
    
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);
    SDL_FreeSurface(surface);
    graphics_set_global_renderer(renderer);
}

void resource_teardown(void) {
    SDL_DestroyRenderer(graphics_get_global_renderer());
}

Suite *resource_manager_suite() {
    Suite *s;
    TCase *tc_loading;
    s = suite_create("Resource Manager");
    
    tc_loading = tcase_create("Core");
    tcase_add_checked_fixture(tc_loading, resource_setup, resource_teardown);
    tcase_add_test(tc_loading, test_resource_loading);
    tcase_add_test(tc_loading, test_resource_double_loading);
    tcase_add_test(tc_loading, test_resource_retain);
    suite_add_tcase(s, tc_loading);
    
    return s;
}
