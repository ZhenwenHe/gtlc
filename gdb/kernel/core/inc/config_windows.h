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

#ifndef CONFIG_WINDOWS_H
#define CONFIG_WINDOWS_H


#if(USING_UILIB_TYPE==0)//MFC	
    #ifndef WINVER                     
    #define WINVER 0x0600           
    #endif

    #ifndef _WIN32_WINNT           
    #define _WIN32_WINNT 0x0600     
    #endif

    #ifndef _WIN32_WINDOWS          
    #define _WIN32_WINDOWS 0x0410  
    #endif

    #ifndef _WIN32_IE                       
    #define _WIN32_IE 0x0700        
    #endif

    #define WIN32_LEAN_AND_MEAN 
    #include "windows.h"
    #include "winbase.h"
    #include "process.h"
    #include "tchar.h"
    #pragma warning (disable:4251)

#elif(USING_UILIB_TYPE==1)//Qt

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

    #ifndef MAX_PATH
    #define MAX_PATH 260
    #endif

    #ifndef WM_USER
    #define WM_USER 0x0400
    #endif
#else//wxWidget
    #ifndef _PALETTEENTRY_DEFINED
    #define _PALETTEENTRY_DEFINED
    typedef struct tagPALETTEENTRY {
            unsigned char        peRed;
            unsigned char        peGreen;
            unsigned char        peBlue;
            unsigned char        peFlags;
    } PALETTEENTRY, *PPALETTEENTRY, FAR *LPPALETTEENTRY;
    #endif // !_PALETTEENTRY_DEFINED

    #ifndef _LOGPALETTE_DEFINED
    #define _LOGPALETTE_DEFINED
    /* Logical Palette */
    typedef struct tagLOGPALETTE {
            short        palVersion;
            short        palNumEntries;
            _Field_size_opt_(palNumEntries) PALETTEENTRY        palPalEntry[1];
    } LOGPALETTE, *PLOGPALETTE, NEAR *NPLOGPALETTE, FAR *LPLOGPALETTE;
    #endif // !_LOGPALETTE_DEFINED


    #ifndef MAX_PATH
    #define MAX_PATH 260
    #endif

    #ifndef WM_USER
    #define WM_USER 0x0400
    #endif
#endif


#endif /* CONFIG_WINDOWS_H */

