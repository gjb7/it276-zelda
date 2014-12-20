//
//  inventory.c
//  zelda
//
//  Created by Grant Butler on 12/18/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#include "inventory.h"
#include <string.h>

inventory_t *inventory_create() {
    inventory_t *inventory = malloc(sizeof(inventory_t));
    if (!inventory) {
        return NULL;
    }
    
    memset(inventory, 0, sizeof(inventory_t));
    
    inventory->max_magic = 100;
    inventory->max_rupees = 99;
    inventory->max_bombs = 10;
    inventory->max_arrows = 30;
    
    return inventory;
}

bool inventory_has_item(inventory_t *inventory, inventory_item_t item) {
    return ((inventory->items & item) == item);
}

void inventory_give_item(inventory_t *inventory, inventory_item_t item) {
    inventory->items |= item;
}

void inventory_free(inventory_t *inventory) {
    free(inventory);
}
