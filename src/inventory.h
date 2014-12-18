//
//  inventory.h
//  zelda
//
//  Created by Grant Butler on 12/18/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
//

#ifndef __zelda__inventory__
#define __zelda__inventory__

#include <stdio.h>

typedef struct inventory_s {
    int magic;
    int max_magic;
    
    int rupees;
    int max_rupees;
    
    int arrows;
    int max_arrows;
    
    int bombs;
    int max_bombs;
    
    
} inventory_t;

inventory_t *inventory_create();

inventory_free(inventory_t *inventory);

#endif /* defined(__zelda__inventory__) */
