/*
//  test_support.h
//  zelda
//
//  Created by Grant Butler on 9/20/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef zelda_test_support_h
#define zelda_test_support_h

#include <check.h>

#ifndef _ck_assert_ptr
#define _ck_assert_ptr(X, OP, Y) do { \
const void* _ck_x = (X); \
const void* _ck_y = (Y); \
ck_assert_msg(_ck_x OP _ck_y, "Assertion '%s' failed: %s==%#x, %s==%#x", #X#OP#Y, #X, _ck_x, #Y, _ck_y); \
} while (0)
#endif

#ifndef ck_assert_ptr_eq
#define ck_assert_ptr_eq(X, Y) _ck_assert_ptr(X, ==, Y)
#endif

#ifndef ck_assert_ptr_ne
#define ck_assert_ptr_ne(X, Y) _ck_assert_ptr(X, !=, Y)
#endif

#endif
