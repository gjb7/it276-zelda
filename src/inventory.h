//
//  inventory.h
//  zelda
//
//  Created by Grant Butler on 12/18/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__inventory__
#define __zelda__inventory__

#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    INVENTORY_ITEM_BOW = 1 << 1,
    INVENTORY_ITEM_BOOMERANG = 1 << 2,
    INVENTORY_ITEM_BOMBS = 1 << 3,
    INVENTORY_ITEM_LANTERN = 1 << 4,
} inventory_item_t;

typedef struct inventory_s {
    int magic;
    int max_magic;
    
    int rupees;
    int max_rupees;
    
    int arrows;
    int max_arrows;
    
    int bombs;
    int max_bombs;
    
    int selected_item;
    
    int items;
} inventory_t;

inventory_t *inventory_create();

void inventory_add_rupees(inventory_t *inventory, int rupees);

bool inventory_has_item(inventory_t *inventory, inventory_item_t item);
void inventory_give_item(inventory_t *inventory, inventory_item_t item);

void inventory_free(inventory_t *inventory);

#endif /* defined(__zelda__inventory__) */
