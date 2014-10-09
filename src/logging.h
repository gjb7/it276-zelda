/*
//  logging.h
//  zelda
//
//  Created by Grant Butler on 10/4/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef __zelda__logging__
#define __zelda__logging__

#define PRINT_RECT(rect) \
    do { \
        printf("%s: .x=%i, .y=%i, .w=%i, .h=%i\n", #rect, rect.x, rect.y, rect.w, rect.h); \
    } while(0);

#define PRINT_POINT(rect) \
    do { \
        printf("%s: .x=%i, .y=%i\n", #rect, rect.x, rect.y); \
    } while(0);

#endif /* defined(__zelda__logging__) */
