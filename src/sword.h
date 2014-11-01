/*
//  sword.h
//  zelda
//
//  Created by Grant Butler on 10/29/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef __zelda__sword__
#define __zelda__sword__

#include <stdio.h>
#include "entity.h"

entity_t *sword_create();

int sword_bounding_box_height_for_direction(entity_t *sword, entity_direction facing);

#endif /* defined(__zelda__sword__) */
