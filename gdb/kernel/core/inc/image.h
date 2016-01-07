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
#pragma once
#include "buffer.h" 



begin_gtl_namespace
begin_gdb_namespace

class  Image;

/** @defgroup Image ImageFilter
*  @{
*/
class CORE_API ImageFilter{
public:
	enum IMAGE_FILTER {
		FILTER_BOX		  = 0,	// Box, pulse, Fourier window, 1st order (constant) b-spline
		FILTER_BICUBIC	  = 1,	// Mitchell & Netravali's two-param cubic filter
		FILTER_BILINEAR   = 2,	// Bilinear filter
		FILTER_BSPLINE	  = 3,	// 4th order (cubic) b-spline
		FILTER_CATMULLROM = 4,	// Catmull-Rom spline, Overhauser spline
		FILTER_LANCZOS3	  = 5	// Lanczos3 filter
	};
	ImageFilter(){ mytype = FILTER_BOX; }
	ImageFilter(int type){
		mytype = type;
	}
	 int getType(){
		return mytype;	
	}
protected:
	int mytype;
	friend class Image;
};
/** @} */

typedef std::shared_ptr<Image> ImageSharedPtr;

/** @defgroup Image Image-图像接口
*  @{
*/
class CORE_API Image{
public:
	enum{
		    IMAGE_FORMAT_UNKNOWN = -1,
			IMAGE_FORMAT_BMP		= 0,
			IMAGE_FORMAT_ICO		= 1,
			IMAGE_FORMAT_JPEG	= 2,
			IMAGE_FORMAT_JNG		= 3,
			IMAGE_FORMAT_KOALA	= 4,
			IMAGE_FORMAT_LBM		= 5,
			IMAGE_FORMAT_IFF = IMAGE_FORMAT_LBM,
			IMAGE_FORMAT_MNG		= 6,
			IMAGE_FORMAT_PBM		= 7,
			IMAGE_FORMAT_PBMRAW	= 8,
			IMAGE_FORMAT_PCD		= 9,
			IMAGE_FORMAT_PCX		= 10,
			IMAGE_FORMAT_PGM		= 11,
			IMAGE_FORMAT_PGMRAW	= 12,
			IMAGE_FORMAT_PNG		= 13,
			IMAGE_FORMAT_PPM		= 14,
			IMAGE_FORMAT_PPMRAW	= 15,
			IMAGE_FORMAT_RAS		= 16,
			IMAGE_FORMAT_TARGA	= 17,
			IMAGE_FORMAT_TIFF	= 18,
			IMAGE_FORMAT_WBMP	= 19,
			IMAGE_FORMAT_PSD		= 20,
			IMAGE_FORMAT_CUT		= 21,
			IMAGE_FORMAT_XBM		= 22,
			IMAGE_FORMAT_XPM		= 23,
			IMAGE_FORMAT_DDS		= 24,
			IMAGE_FORMAT_GIF     = 25,
			IMAGE_FORMAT_HDR		= 26,
			IMAGE_FORMAT_FAXG3	= 27,
			IMAGE_FORMAT_SGI		= 28,
			IMAGE_FORMAT_EXR		= 29,
			IMAGE_FORMAT_J2K		= 30,
			IMAGE_FORMAT_JP2		= 31,
			IMAGE_FORMAT_PFM		= 32,
			IMAGE_FORMAT_PICT	= 33,
			IMAGE_FORMAT_RAW		= 34
	};
	virtual int getFormat() const =0;
	/** Image type used in FreeImage.
	*/
	enum {
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
	};
	virtual int getType() const =0;
	
public:
	/** 
	* @param [in] lpszPathName
	* @param [in] flag
	* @return 执行成功返回true，否则返回false
	*/
	virtual bool load(const char* lpszPathName,int flag=0)=0;
	/** 
	* @param [in] filename
	* @param [in] flags
	* @return 执行成功返回true，否则返回false
	*/
	virtual bool save( const char *filename,int flag=0)=0;

	virtual void  clear() =0;

	/**
	返回影像像素宽度
	@查看 FreeImage_GetWidth
	*/
	virtual unsigned int getWidth() const=0;
	
	/**
	返回影像像素高度
	@查看 FreeImage_GetHeight
	*/
	virtual unsigned int getHeight() const=0;
	
	/**
	返回扫描最近对齐方式的字节宽度
	@查看 FreeImage_GetPitch
	*/
	virtual unsigned int getScanWidth() const=0;
	/**
	 @brief 返回指定扫描行的数据的起始地址
	* @param [in] line 输入指定的行
	* @return 执行成功返回指定扫描行的数据的起始地址
	@查看 FreeImage_GetScanLine, FreeImage documentation
	*/
	virtual unsigned char * getScanLine(unsigned int line) const =0;

	/**
	返回X方向的分辨率,单位：像素/厘米
	@查看 FreeImage_GetDotsPerMeterX
	*/
	virtual double getHorizontalResolution() const =0;
	
	/**
	返回Y方向的分辨率,单位：像素/厘米
	@查看 FreeImage_GetDotsPerMeterY
	*/
	virtual double getVerticalResolution() const =0;
	

	/** 若影像分配了内存，执行成功返回true，否则返回false*/
	virtual bool isValid() const=0;
	 
	/**
	返回影像字节深度. <br>
	当影像类型为 FIT_BITMAP, 有效字节深度可能是 1, 4, 8, 16, 24 或 32.
	@查看 FreeImage_GetBPP, getImageType
	*/
	virtual unsigned int getBitsPerPixel() const=0;

	/**获取指定行列的像素颜色
	* @param [in] x        指定x轴方向的第x个像素
	* @param [in] y        指定y轴方向的第y个像素
	* @param [in,out] c    指定输入输出的像素颜色
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool getPixel(unsigned int x, unsigned int y, Color4f & c)=0;

	/**获取所有的像素颜色值
	* @param [in,out] data   指定输入输出的Buffer
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool getPixels(Buffer & data)=0;
	/**获取所有的像素颜色值
	* @return 执行成功返回像素值首地址，否则返回0
	*/
	virtual unsigned char * getPixels( ) const =0;
	/**
	* @param [in] left   指定复制矩形左边坐标
	* @param [in] top    指定复制矩形上边坐标
	* @param [in] right  指定复制矩形右边坐标
	* @param [in] bottom 指定复制矩形下边坐标
	* @return 执行成功返回ImageSharedPtr，否则返回空的ImageSharedPtr
	*/
	virtual ImageSharedPtr copy( int  left, int top, int right, int bottom)const=0 ;
	/**
	* @param [in] left 指定粘贴的左边的坐标
	* @param [in] top  指定粘贴的上边的坐标
	* @param [in] alpha Alpha 融合
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool paste(const ImageSharedPtr & src, int left, int top, int alpha = 256)=0;
	/**
	* @param [in] left   指定裁剪矩形左边坐标
	* @param [in] top    指定裁剪矩形上边坐标
	* @param [in] right  指定裁剪矩形右边坐标
	* @param [in] bottom 指定裁剪矩形下边坐标
	* @return 执行成功返回ImageSharedPtr，否则返回空的ImageSharedPtr
	*/
	virtual bool crop(int  left, int top, int right, int bottom)=0 ;
	
	/**
	* @param [in] new_width   指定重采样的宽度
	* @param [in] new_height  指定重采样的高度
	* @param [in] filter      指定重采样的方法
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool rescale(unsigned new_width, unsigned new_height, ImageFilter& filter)=0;
	/**
	* @param [in] x       指定要设定影像的x
	* @param [in] y       指定要设定影像的y
	* @param [in] value   指定要设定影像的value
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool setPixelColor(unsigned x, unsigned y, unsigned char *value)=0;
	/**重置图像大小和BPP
	* @param [in] width        要设定影像的宽度
	* @param [in] height       要设定影像的高度
	* @param [in] bpp          影像的BPP值
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool reset(unsigned width, unsigned height, unsigned bpp)=0;

	/**用图像p替换本图像的数据，替换后p中的数据将转移到本图像中，p成为一个空图像
	* @param [in] p        源图像共享指针
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool transfer(const ImageSharedPtr & p)=0;

	/**将图像写入Buffer，以此分别为unsinged int 的width,height,bpp和width*height*bpp/8的字符数组
	* @param [in,out] buf      缓冲区
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool write(Buffer& buf)=0;
	/**将图像置空，从Buffer分别读取unsinged int类型的width,height,bpp和width*height*bpp/8的字符数组，构建新的图像
	* @param [in,out] buf      缓冲区
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool read(const Buffer& buf)=0;

	virtual bool write(int imageFormat, Buffer & fout, int flags=0)=0;
public:
	/** 
	* @return 静态函数返回一个影像共享指针实例
	*/
	static ImageSharedPtr create();
	/**创建一个新图像
	* @param [in] width        要设定影像的宽度
	* @param [in] height       要设定影像的高度
	* @param [in] bpp          影像的BPP值
	* @param [in] pdata        像素数据，按行排列，大小为width*height*bpp/8个字节
	* @return 图像共享指针
	*/
	static ImageSharedPtr create(unsigned width, unsigned height, unsigned bpp, void * pdata);
	/**创建一个新图像,，并拷贝p中的数据
	* @param [in] p        源图像
	* @return 图像共享指针
	*/
	static ImageSharedPtr create(const ImageSharedPtr & p);

};
/** @} */ 


end_gdb_namespace
end_gtl_namespace



