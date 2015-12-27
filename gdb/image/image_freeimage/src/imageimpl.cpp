#include "imageimpl.h"
 

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

ImageImpl::ImageImpl( )
{
	_dib = NULL;
	_bHasChanged = FALSE;
	 _fif =FIF_UNKNOWN;
}

ImageImpl::ImageImpl(unsigned w, unsigned h, unsigned bpp, void * pdata)
{
	_dib = NULL;
	_bHasChanged = FALSE;
	int bpp_div_8 = bpp/8;
	_fif =FIF_UNKNOWN;
	if(reset(w,h,bpp)){
		if(pdata)
		{
			unsigned char* pid = 0;
			for(unsigned i = 0;i<h;i++)
			{
				/*for(unsigned j =0;j<w;j++)
				{
					pid =((unsigned char*)pdata)+(w*i+j)*bpp/8;
					if(pid)
						setPixelColor(j,i,pid);
				}*/
				memcpy(getScanLine(i),((unsigned char*)pdata)+(w*i*bpp_div_8),w*bpp_div_8);
			}

		}
	}
}

ImageImpl::~ImageImpl( )
{
	if(_dib) {
		FreeImage_Unload(_dib);
		_dib = NULL;
	}
}
void  ImageImpl::clear() {
	if(_dib) {
		FreeImage_Unload(_dib);
		_dib = NULL;
	}
	_bHasChanged = TRUE;
}
/** 
* @param [in] lpszPathName
* @param [in] flag
* @return 执行成功返回true，否则返回false
*/
bool ImageImpl::load(const char* lpszPathName, int flag  ){
	
	// check the file signature and get its format
	// (the second argument is currently not used by FreeImage)
	_fif = FreeImage_GetFileType(lpszPathName, 0);
	if(_fif == FIF_UNKNOWN) {
		// no signature ?
		// try to guess the file format from the file extension
		_fif = FreeImage_GetFIFFromFilename(lpszPathName);
	}
	// check that the plugin has reading capabilities ...
	if((_fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(_fif)) {
		// Free the previous dib
		if(_dib) {
			FreeImage_Unload(_dib);			
		}
		// Load the file
		_dib = FreeImage_Load(_fif, lpszPathName, flag);
		_bHasChanged = TRUE;
		if(_dib == NULL)
			return false;

		 
		return true;
	}
	return false;
}
/** 
* @param [in] filename
* @param [in] flags
* @return 执行成功返回true，否则返回false
*/
bool ImageImpl::save( const char *lpszPathName, int flags  ){
	
	BOOL bSuccess = FALSE;

	// Try to guess the file format from the file extension
	_fif = FreeImage_GetFIFFromFilename(lpszPathName);
	if(_fif != FIF_UNKNOWN ) {
		// Check that the dib can be saved in this format
		BOOL bCanSave;

		FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(_dib);
		if(image_type == FIT_BITMAP) {
			// standard bitmap type
			WORD bpp = FreeImage_GetBPP(_dib);
			bCanSave = (FreeImage_FIFSupportsWriting(_fif) && FreeImage_FIFSupportsExportBPP(_fif, bpp));
		} else {
			// special bitmap type
			bCanSave = FreeImage_FIFSupportsExportType(_fif, image_type);
		}

		if(bCanSave) {
			bSuccess = FreeImage_Save(_fif, _dib, lpszPathName, flags);
			return bSuccess?true:false;
		}
	}
	return bSuccess?true:false;
}

/**
Returns the image width in pixels
@see FreeImage_GetWidth
*/
unsigned int ImageImpl::getWidth() const{
	return FreeImage_GetWidth(_dib); 
}
	
/**
Returns the image height in pixels
@see FreeImage_GetHeight
*/
unsigned int ImageImpl::getHeight() const{
	return FreeImage_GetHeight(_dib); 
}
	
/**
Returns the width of the bitmap in bytes rounded to the nearest DWORD.
@see FreeImage_GetPitch
*/
unsigned int ImageImpl::getScanWidth() const{
	return FreeImage_GetPitch(_dib);
}
 unsigned char *  ImageImpl::getScanLine(unsigned int line) const{
	 return FreeImage_GetScanLine(_dib,line);
 }

/**
Returns the bitmap resolution along the X axis, in pixels / cm
@see FreeImage_GetDotsPerMeterX
*/
double ImageImpl::getHorizontalResolution() const{
	return (FreeImage_GetDotsPerMeterX(_dib) / (double)100);
}
	
/**
Returns the bitmap resolution along the Y axis, in pixels / cm
@see FreeImage_GetDotsPerMeterY
*/
double ImageImpl::getVerticalResolution() const{
	return (FreeImage_GetDotsPerMeterY(_dib) / (double)100);
}
/// Returns TRUE if the image is allocated, FALSE otherwise
bool ImageImpl::isValid() const
{
	return (_dib != NULL) ? true:false; 
}
	 
/**
Returns the bitdepth of the bitmap. <br>
When the image type is FIT_BITMAP, valid bitdepth can be 1, 4, 8, 16, 24 or 32.
@see FreeImage_GetBPP, getImageType
*/
unsigned int ImageImpl::getBitsPerPixel() const
{
	return FreeImage_GetBPP(_dib);
}


//获取所有的像素颜色值
unsigned char * ImageImpl::getPixels() const {
	return FreeImage_GetBits(_dib); 
}
//获取所有的像素颜色值
bool ImageImpl::getPixels(Buffer & data)
{
	data.set(getPixels(),getScanWidth()*getHeight()*getBitsPerPixel()/8,false);
	return true;
}

//获取指定行列的像素颜色
bool ImageImpl::getPixel(unsigned int x, unsigned int y, Color4f & c)
{
	RGBQUAD val;
	if(FreeImage_GetPixelColor(_dib,x,y,&val)){
		c.r = val.rgbRed;
		c.g=val.rgbGreen;
		c.b=val.rgbBlue;
		c.a=val.rgbReserved;
		return true;
	}
	return false;
}

ImageSharedPtr ImageImpl::copy( int  left, int top, int right, int bottom)const{
	FIBITMAP *dst = FreeImage_Copy(_dib, left, top, right, bottom);
	ImageImpl* image = new ImageImpl();
	image->_dib = dst; 	
	return ImageSharedPtr((Image*)image); 
}

bool ImageImpl::paste(const ImageSharedPtr & src, int left, int top, int alpha){
	if(_dib) {
		BOOL bResult = FreeImage_Paste(_dib, ((ImageImpl*)(src.get()))->_dib, left, top, alpha);
		_bHasChanged = TRUE;
		return bResult?true:false;
	}
	return false;
}

bool ImageImpl::crop(int  left, int top, int right, int bottom){
	if(_dib) {
		FIBITMAP *dst = FreeImage_Copy(_dib, left, top, right, bottom);
		return replace(dst);
	}
	return false;
}
bool  ImageImpl::replace(FIBITMAP *new_dib) {
	if(new_dib == NULL) 
		return false;
	if(_dib)
		FreeImage_Unload(_dib);
	_dib = new_dib;
	_bHasChanged = TRUE;
	return true;
}
bool  ImageImpl::transfer(const ImageSharedPtr & pImage) {
	ImageImpl * p = (ImageImpl*)(pImage.get());
	if(p->_dib == NULL) 
		return false;
	if(_dib)
		FreeImage_Unload(_dib);
	_dib = p->_dib;
	_bHasChanged = TRUE;

	p->_dib=0;
	p->_bHasChanged=TRUE;
	return true;
}

bool ImageImpl::rescale(unsigned new_width, unsigned new_height,ImageFilter& filter){
	if(_dib) {
		//没有定数据类型((FREEIMAGEHEADER *)dib->data)->type.
		switch(FreeImage_GetImageType(_dib)) {
		case FIT_BITMAP:
		case FIT_UINT16:
		case FIT_RGB16:
		case FIT_RGBA16:
		case FIT_FLOAT:
		case FIT_RGBF:
		case FIT_RGBAF:
			break;
		default:
			return false;
			break;
		}
		// Perform upsampling / downsampling
		FIBITMAP *dst = FreeImage_Rescale(_dib, new_width, new_height,/*FILTER_BOX*/(FREE_IMAGE_FILTER)filter.getType());
		return replace(dst);
	}
	else
	{
		return false;
	}
}
bool ImageImpl::reset(unsigned width, unsigned height, unsigned bpp){

	if(_dib) {
		FreeImage_Unload(_dib);
	}
	if((_dib = FreeImage_AllocateT(FIT_BITMAP, width, height, bpp, 0, 0, 0)) == NULL)
		return false;
	_fif = FIF_BMP;
//	if( 1== FIT_BITMAP) {
		// Create palette if needed
		switch(bpp)	{
			case 1:
			case 4:
			case 8:
				RGBQUAD *pal = FreeImage_GetPalette(_dib);
				for(unsigned i = 0; i < FreeImage_GetColorsUsed(_dib); i++) {
					pal[i].rgbRed = i;
					pal[i].rgbGreen = i;
					pal[i].rgbBlue = i;
				}
				break;
		}
	//}

	_bHasChanged = true;

	return true;
}
bool ImageImpl::setPixelColor(unsigned x, unsigned y, unsigned char *value){
	RGBQUAD v;
	v.rgbBlue  = value[2];
	v.rgbGreen = value[1];
	v.rgbRed   = value[0];
	v.rgbReserved = 0;
	return FreeImage_SetPixelColor(_dib,x,y,&v);
}
/**将图像写入Buffer，以此分别为unsinged int 的width,height,bpp和width*height*bpp/8的字符数组
* @param [in,out] buf      缓冲区
* @return 执行成功返回true，否则返回空的false
*/
bool ImageImpl::write(Buffer& buf)
{
	unsigned w = getWidth();
	unsigned h = getHeight();
	unsigned bpp = getBitsPerPixel();
	buf.write(&w,sizeof(unsigned));
	buf.write(&h,sizeof(unsigned));
	buf.write(&bpp,sizeof(unsigned));
	buf.write(getPixels(),bpp/8*w*h);
	return true;
}
/**将图像置空，从Buffer分别读取unsinged int类型的width,height,bpp和width*height*bpp/8的字符数组，构建新的图像
* @param [in,out] buf      缓冲区
* @return 执行成功返回true，否则返回空的false
*/
bool ImageImpl::read(const Buffer& buffer)
{
	Buffer & buf = (Buffer&) buffer;
	unsigned w = getWidth();
	unsigned h = getHeight();
	unsigned bpp = getBitsPerPixel();
	buf.read(&w,sizeof(unsigned));
	buf.read(&h,sizeof(unsigned));
	buf.read(&bpp,sizeof(unsigned));
	reset(w,h,bpp);
	buf.read(getPixels(),bpp/8*w*h);
	return true;
}


int ImageImpl::getFormat() const
{
	return _fif;
}
int ImageImpl::getType() const
{
	return FreeImage_GetImageType(_dib);
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace



ImageFactoryImpl::ImageFactoryImpl(void)
{
}


ImageFactoryImpl::~ImageFactoryImpl(void)
{
}

cn::edu::cug::gdb::ImageSharedPtr ImageFactoryImpl::newImage( ){	 
	return cn::edu::cug::gdb::ImageSharedPtr(new cn::edu::cug::gdb::ImageImpl());
	 
}

cn::edu::cug::gdb::ImageSharedPtr ImageFactoryImpl::newImage(unsigned w, unsigned h, unsigned bpp, void * pdata ){	 
	return cn::edu::cug::gdb::ImageSharedPtr(new cn::edu::cug::gdb::ImageImpl(w, h, bpp, pdata));
	 
}
