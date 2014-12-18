//
//  inventory.c
//  zelda
//
//  Created by Grant Butler on 12/18/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "inventory.h"

inventory_t *inventory_create() {
    inventory_t *inventory = malloc(sizeof(inventory_t));
    if (!inventory) {
        return NULL;
    }
    
    memset(inventory, 0, sizeof(inventory_t));
    
    inventory->max_bombs = 10;
    inventory->max_arrows = 30;
    
    return inventory;
}

inventory_free(inventory_t *inventory) {
    free(inventory);
}
