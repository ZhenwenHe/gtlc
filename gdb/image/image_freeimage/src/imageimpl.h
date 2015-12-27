#pragma once
#include "image.h"
#include "../inc/freeimage.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class ImageImpl :	public cn::edu::cug::gdb::Image
{
protected:
	/// DIB data
	FIBITMAP *_dib;
	/// Original (or last saved) fif format if available, FIF_UNKNOWN otherwise
	FREE_IMAGE_FORMAT _fif;
	/// TRUE whenever the display need to be refreshed
	mutable BOOL _bHasChanged;
protected:
	bool  replace(FIBITMAP *new_dib);
	bool  writeDDSBuffer(Buffer & buf);
public:
	ImageImpl(void);
	ImageImpl(unsigned w, unsigned h, unsigned bpp, void * pdata);
	virtual ~ImageImpl(void);
	/** 
	* @param [in] lpszPathName
	* @param [in] flag
	* @return 执行成功返回true，否则返回false
	*/
	virtual bool load(const char* lpszPathName,int flag=0 );
	/** 
	* @param [in] filename
	* @param [in] flags
	* @return 执行成功返回true，否则返回false
	*/
	virtual bool save( const char *filename,int flag=0 );

	virtual void  clear() ;

	/**
	返回影像像素宽度
	@see FreeImage_GetWidth
	*/
	virtual unsigned int getWidth() const;
	
	/**
	返回影像像素高度
	@see FreeImage_GetHeight
	*/
	virtual unsigned int getHeight() const;
	
	/**
	返回扫描最近对齐方式的字节宽度
	@see FreeImage_GetPitch
	*/
	virtual unsigned int getScanWidth() const;

	/**
	返回位图的字节宽度.<br>
	<b>它与scanline长度不一样</b>.
	@查看 FreeImage_GetLine, getScanWidth
	*/
	virtual unsigned char * getScanLine(unsigned int line) const;

	/**
	返回X方向的分辨率,单位：像素/厘米
	@see FreeImage_GetDotsPerMeterX
	*/
	virtual double getHorizontalResolution() const;
	
	/**
	返回Y方向的分辨率,单位：像素/厘米
	@see FreeImage_GetDotsPerMeterY
	*/
	virtual double getVerticalResolution() const;
	

	/** 若影像分配了内存，执行成功返回true，否则返回false*/
	virtual bool isValid() const;
	 
	/**
	返回影像字节深度. <br>
	当影像类型为 FIT_BITMAP, 有效字节深度可能是 1, 4, 8, 16, 24 或 32.
	@see FreeImage_GetBPP, getImageType
	*/
	virtual unsigned int getBitsPerPixel() const;
	
	/**获取指定行列的像素颜色
	* @param [in] x        指定x轴方向的第x个像素
	* @param [in] y        指定y轴方向的第y个像素
	* @param [in,out] c    指定输入输出的像素颜色
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool getPixel(unsigned int x, unsigned int y, Color4f & c);

	/**获取所有的像素颜色值
	* @param [in,out] data   指定输入输出的Buffer
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool getPixels(Buffer & data);
	/**获取所有的像素颜色值
	* @return 执行成功返回像素值首地址，否则返回0
	*/
	virtual unsigned char * getPixels( ) const ;
	/**
	* @param [in] left   指定复制矩形左边坐标
	* @param [in] top    指定复制矩形上边坐标
	* @param [in] right  指定复制矩形右边坐标
	* @param [in] bottom 指定复制矩形下边坐标
	* @return 执行成功返回ImageSharedPtr，否则返回空的ImageSharedPtr
	*/
	virtual ImageSharedPtr copy( int  left, int top, int right, int bottom)const ;
	/**
	* @param [in] left 指定粘贴的左边的坐标
	* @param [in] top  指定粘贴的上边的坐标
	* @param [in] alpha Alpha 融合
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool paste(const ImageSharedPtr & src, int left, int top, int alpha = 256);
	/**
	* @param [in] left   指定裁剪矩形左边坐标
	* @param [in] top    指定裁剪矩形上边坐标
	* @param [in] right  指定裁剪矩形右边坐标
	* @param [in] bottom 指定裁剪矩形下边坐标
	* @return 执行成功返回ImageSharedPtr，否则返回空的ImageSharedPtr
	*/
	virtual bool crop(int  left, int top, int right, int bottom) ;
	/**
	* @param [in] new_width   指定重采样的宽度
	* @param [in] new_height  指定重采样的高度
	* @param [in] filter      指定重采样的方法
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool rescale(unsigned new_width, unsigned new_height, ImageFilter& filter);
	/**
	* @param [in] width   指定新生成影像的宽度
	* @param [in] height  指定新生成影像高度
	* @param [in] bpp         指定新生成影的bpp
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool reset(unsigned width, unsigned height, unsigned bpp);
	/**
	* @param [in] x       指定要设定影像的x
	* @param [in] y       指定要设定影像的y
	* @param [in] value   指定要设定影像的value
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool setPixelColor(unsigned x, unsigned y, unsigned char *value);

	/**用图像p替换本图像的数据，替换后p中的数据将转移到本图像中，p成为一个空图像
	* @param [in] p        源图像共享指针
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool transfer(const ImageSharedPtr & p);
	/**将图像写入Buffer，以此分别为unsinged int 的width,height,bpp和width*height*bpp/8的字符数组
	* @param [in,out] buf      缓冲区
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool write(Buffer& buf);
	/**将图像置空，从Buffer分别读取unsinged int类型的width,height,bpp和width*height*bpp/8的字符数组，构建新的图像
	* @param [in,out] buf      缓冲区
	* @return 执行成功返回true，否则返回空的false
	*/
	virtual bool read(const Buffer& buf);

	virtual bool write(int imageFormat, Buffer & fout, int flags=0);

	virtual int getFormat() const ;

	virtual int getType() const ;
};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace


#include "factory.h"
class ImageFactoryImpl:public cn::edu::cug::gdb::ImageFactory
{
public:
	ImageFactoryImpl(void);
	virtual ~ImageFactoryImpl(void);
	virtual  cn::edu::cug::gdb::ImageSharedPtr newImage( );
	virtual  cn::edu::cug::gdb::ImageSharedPtr newImage(unsigned w, unsigned h, unsigned bpp, void * pdata );
};

struct ImageFactoryDeletor{
	void operator ()(cn::edu::cug::gdb::Factory * p){
		delete (ImageFactoryImpl*)((cn::edu::cug::gdb::ImageFactory* )p);
	}
};