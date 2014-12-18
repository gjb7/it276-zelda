//
//  inventory_hud.h
//  zelda
//
//  Created by Grant Butler on 12/18/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__inventory_hud__
#define __zelda__inventory_hud__

#include "entity.h"
#include "sprite.h"
#include "inventory.h"

typedef struct inventory_hud_s {
    sprite_t *background_sprite;
    
    inventory_t *inventory;
} inventory_hud_t;

entity_t *hud_create(inventory_t *inventory);

#endif /* defined(__zelda__inventory_hud__) */
