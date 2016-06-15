/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose.
* It is provided "as is" without express or implied warranty.
*/

#ifndef CONFIG_LINUX_H
#define CONFIG_LINUX_H


    #ifndef _PALETTEENTRY_DEFINED
    #define _PALETTEENTRY_DEFINED
    typedef struct tagPALETTEENTRY {
            unsigned char*        peRed;
            unsigned char*        peGreen;
            unsigned char*        peBlue;
            unsigned char*        peFlags;
    } PALETTEENTRY, *PPALETTEENTRY;
    #endif // !_PALETTEENTRY_DEFINED

    #ifndef _LOGPALETTE_DEFINED
    #define _LOGPALETTE_DEFINED
    /* Logical Palette */
    typedef struct tagLOGPALETTE {
            short        palVersion;
            short        palNumEntries;
            PALETTEENTRY        palPalEntry[1];
    } LOGPALETTE, *PLOGPALETTE;
    #endif // !_LOGPALETTE_DEFINED

    #include <sys/time.h>

#endif /* CONFIG_LINUX_H */

