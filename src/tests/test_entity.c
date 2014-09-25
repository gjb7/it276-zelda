//
//  test_entity.c
//  zelda
//
//  Created by Grant Butler on 9/17/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include <check.h>
#include "test_support.h"

#include "entity.h"

START_TEST(test_entity_create)
{
    entity_t *e = entity_create();
    
    ck_assert_int_eq(e->retain_count, 1);
    
    entity_release(e);
}
END_TEST

START_TEST(test_entity_retain)
{
    entity_t *e = entity_create();
    entity_retain(e);
    
    ck_assert_int_eq(e->retain_count, 2);
    
    entity_release(e);
    entity_release(e);
}
END_TEST

START_TEST(test_entity_hierarchy)
{
    entity_t *parent = entity_create();
    entity_t *child = entity_create();
    
    ck_assert_ptr_eq(parent->parent, NULL);
    ck_assert_ptr_eq(child->parent, NULL);
    
    ck_assert_int_eq(g_slist_length(parent->children), 0);
    ck_assert_int_eq(g_slist_length(child->children), 0);
    
    entity_add_child(parent, child);
    
    ck_assert_ptr_eq(parent->parent, NULL);
    ck_assert_ptr_eq(child->parent, parent);
    
    ck_assert_int_eq(g_slist_length(parent->children), 1);
    ck_assert_int_eq(g_slist_length(child->children), 0);
    
    entity_remove_from_parent(child);
    
    ck_assert_ptr_eq(parent->parent, NULL);
    ck_assert_ptr_eq(child->parent, NULL);
    
    ck_assert_int_eq(g_slist_length(parent->children), 0);
    ck_assert_int_eq(g_slist_length(child->children), 0);
    
    entity_release(parent);
    entity_release(child);
}
END_TEST

START_TEST(test_free_entity_hierarchy)
{
    entity_t *parent = entity_create();
    entity_t *child = entity_create();
    
    entity_add_child(parent, child);
    
    entity_release(parent);
    
    ck_assert_ptr_eq(child->parent, NULL);
    ck_assert_int_eq(child->retain_count, 1);
    
    entity_release(child);
}
END_TEST

Suite *entity_suite() {
    Suite *s;
    TCase *tc_memory, *tc_hierarchy;
    s = suite_create("Entity");
    
    tc_memory = tcase_create("Memory Management");
    tcase_add_test(tc_memory, test_entity_create);
    tcase_add_test(tc_memory, test_entity_retain);
    suite_add_tcase(s, tc_memory);
    
    tc_hierarchy = tcase_create("Entity Hierarchy");
    tcase_add_test(tc_hierarchy, test_entity_hierarchy);
    tcase_add_test(tc_hierarchy, test_free_entity_hierarchy);
    suite_add_tcase(s, tc_hierarchy);
    
    return s;
}
