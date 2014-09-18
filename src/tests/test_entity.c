//
//  test_entity.c
//  zelda
//
//  Created by Grant Butler on 9/17/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include <check.h>

#include "../entity.h"

START_TEST(test_entity_create)
{
    entity *e = entity_create();
    
    ck_assert_int_eq(e->retain_count, 1);
    
    entity_release(e);
}
END_TEST

Suite *entity_suite() {
    Suite *s;
    TCase *tc_core;
    s = suite_create("Entity");
    tc_core = tcase_create("Core");
    
    tcase_add_test(tc_core, test_entity_create);
    suite_add_tcase(s, tc_core);
    
    return s;
}
