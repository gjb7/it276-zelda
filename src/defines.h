/*
//  defines.h
//  zelda
//
//  Created by Grant Butler on 10/27/14.
//  Copyright (c) 2014 Grant Butler. All rights reserved.
*/

#ifndef zelda_defines_h
#define zelda_defines_h

#ifndef INLINE
    #if __GNUC__
        #if !__GNUC_STDC_INLINE__
            #define INLINE extern inline
        #else
            #define INLINE inline
        #endif /* if !__GNUC_STDC_INLINE__ */
    #else
        #define INLINE __inline__
    #endif /* if __GNUC__ */
#endif /* ifndef INLINE */

#endif
