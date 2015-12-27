#include "imageimpl.h"
 

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
typedef unsigned int UI32;
typedef int I32;

struct  DDCOLORKEY
{
	DDCOLORKEY():
dwColorSpaceLowValue(0),
	dwColorSpaceHighValue(0) {}

UI32    dwColorSpaceLowValue;
UI32    dwColorSpaceHighValue;
};

struct DDPIXELFORMAT
{

	DDPIXELFORMAT():
dwSize(0),
	dwFlags(0),
	dwFourCC(0),
	dwRGBBitCount(0),
	dwRBitMask(0),
	dwGBitMask(0),
	dwBBitMask(0),
	dwRGBAlphaBitMask(0) {}


UI32    dwSize;
UI32    dwFlags;
UI32    dwFourCC;
union
{
	UI32    dwRGBBitCount;
	UI32    dwYUVBitCount;
	UI32    dwZBufferBitDepth;
	UI32    dwAlphaBitDepth;
	UI32    dwLuminanceBitDepth;
};
union
{
	UI32    dwRBitMask;
	UI32    dwYBitMask;
};
union
{
	UI32    dwGBitMask;
	UI32    dwUBitMask;
};
union
{
	UI32    dwBBitMask;
	UI32    dwVBitMask;
};
union
{
	UI32    dwRGBAlphaBitMask;
	UI32    dwYUVAlphaBitMask;
	UI32    dwRGBZBitMask;
	UI32    dwYUVZBitMask;
};
};

struct  DDSCAPS2
{
	DDSCAPS2():
dwCaps(0),
	dwCaps2(0),
	dwCaps3(0),
	dwCaps4(0) {}

UI32       dwCaps;
UI32       dwCaps2;
UI32       dwCaps3;
union
{
	UI32       dwCaps4;
	UI32       dwVolumeDepth;
};
};

struct DDSURFACEDESC2
{
	DDSURFACEDESC2():
dwSize(0),
	dwFlags(0),
	dwHeight(0),
	dwWidth(0), 
	lPitch(0),
	dwBackBufferCount(0),
	dwMipMapCount(0),
	dwAlphaBitDepth(0),
	dwReserved(0),     
	lpSurface(0),      
	dwTextureStage(0) {}      


UI32         dwSize;
UI32         dwFlags;
UI32         dwHeight;
UI32         dwWidth; 
union                          
{
	I32              lPitch;
	UI32     dwLinearSize;
};
union
{
	UI32      dwBackBufferCount;
	UI32      dwDepth;      
};
union
{
	UI32     dwMipMapCount;
	UI32     dwRefreshRate;
};
UI32         dwAlphaBitDepth;
UI32         dwReserved;     
UI32        lpSurface;         //Fred Marmond: removed from pointer type to UI32 for 64bits compatibility. it is unused data 
DDCOLORKEY    ddckCKDestOverlay;      
DDCOLORKEY    ddckCKDestBlt;           
DDCOLORKEY    ddckCKSrcOverlay;        
DDCOLORKEY    ddckCKSrcBlt;            
DDPIXELFORMAT ddpfPixelFormat;         
DDSCAPS2      ddsCaps;                 
UI32 dwTextureStage;          
};

//
// Structure of a DXT-1 compressed texture block
// see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directx9_c/Opaque_and_1_Bit_Alpha_Textures.asp
//
struct DXT1TexelsBlock
{
	unsigned short color_0;     // colors at their
	unsigned short color_1;     // extreme
	unsigned int   texels4x4;   // interpolated colors (2 bits per texel)
};

//
// DDSURFACEDESC2 flags that mark the validity of the struct data
//
#define DDSD_CAPS               0x00000001l     // default
#define DDSD_HEIGHT             0x00000002l        // default
#define DDSD_WIDTH              0x00000004l        // default
#define DDSD_PIXELFORMAT        0x00001000l        // default
#define DDSD_PITCH              0x00000008l     // For uncompressed formats
#define DDSD_MIPMAPCOUNT        0x00020000l
#define DDSD_LINEARSIZE         0x00080000l     // For compressed formats
#define DDSD_DEPTH              0x00800000l        // Volume Textures

//
// DDPIXELFORMAT flags
//
#define DDPF_ALPHAPIXELS        0x00000001l
#define DDPF_FOURCC             0x00000004l        // Compressed formats 
#define DDPF_RGB                0x00000040l        // Uncompressed formats
#define DDPF_ALPHA              0x00000002l
#define DDPF_COMPRESSED         0x00000080l
#define DDPF_LUMINANCE          0x00020000l
#define DDPF_BUMPLUMINANCE      0x00040000l        // L,U,V
#define DDPF_BUMPDUDV           0x00080000l        // U,V

//
// DDSCAPS flags
//
#define DDSCAPS_TEXTURE         0x00001000l     // default
#define DDSCAPS_COMPLEX         0x00000008l
#define DDSCAPS_MIPMAP          0x00400000l
#define DDSCAPS2_VOLUME         0x00200000l


#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((UI32)(char)(ch0) | ((UI32)(char)(ch1) << 8) |   \
	((UI32)(char)(ch2) << 16) | ((UI32)(char)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)

/*
* FOURCC codes for DX compressed-texture pixel formats
*/
#define FOURCC_DXT1  (MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT2  (MAKEFOURCC('D','X','T','2'))
#define FOURCC_DXT3  (MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT4  (MAKEFOURCC('D','X','T','4'))
#define FOURCC_DXT5  (MAKEFOURCC('D','X','T','5'))

bool ImageImpl::writeDDSBuffer(Buffer& fout)
{
	// Initialize ddsd structure and its members 
    DDSURFACEDESC2 ddsd;
    DDPIXELFORMAT  ddpf;
    DDSCAPS2       ddsCaps;

    ddsd.dwSize = sizeof(ddsd);  
    ddpf.dwSize = sizeof(ddpf);

    // Default values and initialization of structures' flags
    unsigned int SD_flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    unsigned int CAPS_flags  = DDSCAPS_TEXTURE; 
    unsigned int PF_flags = 0;
    unsigned int CAPS2_flags = 0;

    // Get image properties
    unsigned int pixelSize      = getBitsPerPixel();
    unsigned long long imageSize = getScanWidth();
	imageSize *= getHeight(); 
    bool is3dImage = false;

    ddsd.dwWidth  = getWidth();
    ddsd.dwHeight = getHeight();
	//²»´¦Àí3D Image
    //int r = img->r();
    //if(r > 1)  /* check for 3d image */
    //{
    //    is3dImage = true;
    //    ddsd.dwDepth = r;
    //    SD_flags    |= DDSD_DEPTH;
    //    CAPS_flags  |= DDSCAPS_COMPLEX;
    //    CAPS2_flags |= DDSCAPS2_VOLUME;
    //}



    // Determine format - set flags and ddsd, ddpf properties
	/*
	IMAGE_TYPE_UNKNOWN = 0,	// unknown type
	IMAGE_TYPE_BITMAP  = 1,	// standard image			: 1-, 4-, 8-, 16-, 24-, 32-bit
	IMAGE_TYPE_UINT16	= 2,	// array of unsigned short	: unsigned 16-bit
	IMAGE_TYPE_INT16	= 3,	// array of short			: signed 16-bit
	IMAGE_TYPE_UINT32	= 4,	// array of unsigned long	: unsigned 32-bit
	IMAGE_TYPE_INT32	= 5,	// array of long			: signed 32-bit
	IMAGE_TYPE_FLOAT	= 6,	// array of float			: 32-bit IEEE floating point
	IMAGE_TYPE_DOUBLE	= 7,	// array of double			: 64-bit IEEE floating point
	IMAGE_TYPE_COMPLEX	= 8,	// array of FICOMPLEX		: 2 x 64-bit IEEE floating point
	IMAGE_TYPE_RGB16	= 9,	// 48-bit RGB image			: 3 x 16-bit
	IMAGE_TYPE_RGBA16	= 10,	// 64-bit RGBA image		: 4 x 16-bit
	IMAGE_TYPE_RGBF	= 11,	// 96-bit RGB float image	: 3 x 32-bit IEEE floating point
	IMAGE_TYPE_RGBAF	= 12	// 128-bit RGBA float image	: 4 x 32-bit IEEE floating point
	*/
    switch (getType())
    {
	case IMAGE_TYPE_UNKNOWN:
		{
			return false;
		}
	case IMAGE_TYPE_BITMAP:
		{
			switch(getBitsPerPixel())
			{
			case 1:
			case 4:
				{
					return false;
				}
			case 8:
				{
					ddpf.dwRGBAlphaBitMask  = 0x000000ff;
					PF_flags |= DDPF_ALPHA;
					ddpf.dwRGBBitCount = pixelSize;
					ddsd.lPitch = getScanWidth();
					SD_flags |= DDSD_PITCH;
					break;
				}
			case 16:
				{
					return false;
				}
			case 24:
				{
					ddpf.dwRBitMask        = 0x000000ff;
					ddpf.dwGBitMask        = 0x0000ff00;
					ddpf.dwBBitMask        = 0x00ff0000;  
					PF_flags |= DDPF_RGB;
					ddpf.dwRGBBitCount = pixelSize;
					ddsd.lPitch = getScanWidth();
					SD_flags |= DDSD_PITCH;
					break;
				}
			case 32:
				{
					ddpf.dwRBitMask        = 0x000000ff;
					ddpf.dwGBitMask        = 0x0000ff00;
					ddpf.dwBBitMask        = 0x00ff0000;  
					ddpf.dwRGBAlphaBitMask = 0xff000000;
					PF_flags |= (DDPF_ALPHAPIXELS | DDPF_RGB);
					ddpf.dwRGBBitCount = pixelSize; 
					ddsd.lPitch = getScanWidth();
					SD_flags |= DDSD_PITCH;
					break;
				}
			}
			break;
		}
	case IMAGE_TYPE_UINT16:
		{
			return false;
		}
	case IMAGE_TYPE_INT16:
		{
			return false;
		}
	case IMAGE_TYPE_UINT32:
		{
			return false;
		}
	case IMAGE_TYPE_INT32:
		{
			return false;
		}
	case IMAGE_TYPE_FLOAT:
		{
			return false;
		}
	case IMAGE_TYPE_DOUBLE:
		{
			return false;
		}
	case IMAGE_TYPE_COMPLEX:
		{
			return false;
		}
	case IMAGE_TYPE_RGB16:
		{
			return false;
		}
	case IMAGE_TYPE_RGBA16:
		{
			return false;
		}
	case IMAGE_TYPE_RGBF:
		{
			return false;
		}	
	case IMAGE_TYPE_RGBAF:
		{
			return false;
		}			
	default:
		return false;
	}

   
    //// set even more flags
    //if(img->isMipmap() && !is3dImage)
    //{
    //    SD_flags   |= DDSD_MIPMAPCOUNT;
    //    CAPS_flags |= DDSCAPS_COMPLEX | DDSCAPS_MIPMAP;
    //    ddsd.dwMipMapCount = img->getNumMipmapLevels();
    //    
    //    //osg::notify(osg::INFO)<<"writing out with mipmaps ddsd.dwMipMapCount"<<ddsd.dwMipMapCount<<std::endl;
    //}
    //else
    //{
    //    //osg::notify(osg::INFO)<<"no mipmaps to write out."<<std::endl;
    //}


    // Assign flags and structure members of ddsd
    ddsd.dwFlags    = SD_flags;
    ddpf.dwFlags    = PF_flags;
    ddsCaps.dwCaps  = CAPS_flags;
    ddsCaps.dwCaps2 = CAPS2_flags;

    ddsd.ddpfPixelFormat = ddpf;
    ddsd.ddsCaps = ddsCaps;


    // Write DDS file
    fout.write("DDS ", 4); /* write FOURCC */
    fout.write(reinterpret_cast<char*>(&ddsd), sizeof(ddsd)); /* write file header */

    //    int isize = img->getTotalSizeInBytesIncludingMipmaps();
    /*if(!is3dImage)
    {*/
        fout.write((void*)reinterpret_cast<const char*>(getPixels()), imageSize);
    //}
    //else  /* 3d image */
    //{
    //    for(int i = 0; i < r; ++i)
    //    {
    //        fout.write((void*)reinterpret_cast<const char*>(img->data(0, 0, i)), imageSize);
    //    }
    //}

    // Check for correct saving
   /* if (fout.fail())
    {
        return false;
    }*/

    // If we get that far the file was saved properly //
    return true; 
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

