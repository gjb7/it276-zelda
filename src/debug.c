//
//  debug.c
//  zelda
//
//  Created by Grant Butler on 10/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "debug.h"

static bool _debug_render_collision_boxes = false;

void debug_set_render_collision_boxes(bool renderCollisionBoxes) {
    _debug_render_collision_boxes = renderCollisionBoxes;
}

bool debug_get_render_collision_boxes() {
    return _debug_render_collision_boxes;
}
