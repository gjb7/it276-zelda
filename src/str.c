/*
//  str.c
//  zelda
//
//  Created by Grant Butler on 10/29/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "str.h"
#include <string.h>

bool str_starts_with(char *string, const char *prefix) {
    int prefix_length = strlen(prefix),
        string_length = strlen(string);
    
    if (prefix_length > string_length) {
        return false;
    }
    else {
        return strncmp(prefix, string, prefix_length) == 0;
    }
}
