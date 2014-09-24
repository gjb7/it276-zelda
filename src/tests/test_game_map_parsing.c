//
//  test_game_map_parsing.c
//  zelda
//
//  Created by Grant Butler on 9/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include <check.h>
#include "test_support.h"

#include "game_map.h"

extern entity_t *_game_map_create_from_map(SDL_RWops *fp);

START_TEST(test_valid_game_map_parsing)
{
    char valid_map[] = {
        0x56, 0x01, 0x3B, 0x57, 0x0A, 0x3B, 0x48, 0x0A, 0x3B, 0x4C, 0x01, 0x3B, 0x54, 0x72, 0x65, 0x73, 0x2F, 0x74, 0x69, 0x6C, 0x65, 0x6D, 0x61, 0x70, 0x73, 0x2F, 0x6F, 0x76, 0x65, 0x72, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x2E, 0x70, 0x6E, 0x67, 0x3B, 0x6C, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x3B
    };
    SDL_RWops *fp = SDL_RWFromConstMem(valid_map, sizeof(valid_map));
    
    entity_t *game_map = _game_map_create_from_map(fp);
    
    ck_assert_ptr_ne(game_map, NULL);
    
    entity_release(game_map);
}
END_TEST

START_TEST(test_invalid_game_map_version)
{
    char invalid_map[] = {
        0x56, 0x10, 0x3B, 0x57, 0x0A, 0x3B, 0x48, 0x0A, 0x3B, 0x4C, 0x01, 0x3B, 0x6C, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x3B
    };
    SDL_RWops *fp = SDL_RWFromConstMem(invalid_map, sizeof(invalid_map));
    
    entity_t *game_map = _game_map_create_from_map(fp);
    
    ck_assert_ptr_eq(game_map, NULL);
}
END_TEST

START_TEST(test_invalid_game_map_size_too_large)
{
    char invalid_map[] = {
        0x56, 0x01, 0x3B, 0x57, 0x10, 0x3B, 0x48, 0x10, 0x3B, 0x4C, 0x01, 0x3B, 0x6C, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x3B
    };
    SDL_RWops *fp = SDL_RWFromConstMem(invalid_map, sizeof(invalid_map));
    
    entity_t *game_map = _game_map_create_from_map(fp);
    
    ck_assert_ptr_eq(game_map, NULL);
}
END_TEST

START_TEST(test_invalid_game_map_size_too_small)
{
    char invalid_map[] = {
        0x56, 0x01, 0x3B, 0x57, 0x01, 0x3B, 0x48, 0x01, 0x3B, 0x4C, 0x01, 0x3B, 0x6C, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x3B
    };
    SDL_RWops *fp = SDL_RWFromConstMem(invalid_map, sizeof(invalid_map));
    
    entity_t *game_map = _game_map_create_from_map(fp);
    
    ck_assert_ptr_eq(game_map, NULL);
}
END_TEST

Suite *game_map_parsing_suite() {
    Suite *s;
    TCase *tc_valid_map, *tc_invalid_map;
    s = suite_create("Game Map Parsing");
    
    tc_valid_map = tcase_create("Valid Map");
    tcase_add_test(tc_valid_map, test_valid_game_map_parsing);
    suite_add_tcase(s, tc_valid_map);
    
    tc_invalid_map = tcase_create("Invalid Map");
    tcase_add_test(tc_invalid_map, test_invalid_game_map_version);
    tcase_add_test(tc_invalid_map, test_invalid_game_map_size_too_large);
    tcase_add_test(tc_invalid_map, test_invalid_game_map_size_too_small);
    suite_add_tcase(s, tc_invalid_map);
    
    return s;
}
