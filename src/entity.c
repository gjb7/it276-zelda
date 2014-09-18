//
//  entity.c
//  zelda
//
//  Created by Grant Butler on 9/17/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "entity.h"

void _entity_free(entity *e) {
    free(e);
    e = NULL;
}

entity *entity_create() {
    entity *e = malloc(sizeof(entity));
    memset(e, 0, sizeof(entity));
    e->retain_count = 1;
    return e;
}

void entity_retain(entity *e) {
    e->retain_count++;
}

void entity_release(entity *e) {
    if (--e->retain_count == 0) {
        if (e->dealloc != NULL) {
            e->dealloc(e);
        }
        
        _entity_free(e);
    }
}
