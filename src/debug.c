/*
//  debug.c
//  zelda
//
//  Created by Grant Butler on 10/23/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#include "debug.h"

static bool _debug_render_bounding_boxes = false;

void debug_set_render_bounding_boxes(bool renderBoundingBoxes) {
    _debug_render_bounding_boxes = renderBoundingBoxes;
}

bool debug_get_render_bounding_boxes() {
    return _debug_render_bounding_boxes;
}
