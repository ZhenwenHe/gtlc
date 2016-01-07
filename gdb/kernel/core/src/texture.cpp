#include "texture.h"
#include "factory.h"
#include "internal_textureimpl.h"


begin_gtl_namespace
begin_gdb_namespace

TextureSharedPtr Texture::create(){
#if(USING_3DGEOMETRY_TYPE==0)//use plugin
	return  ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newTexture();
#else
	TextureSharedPtr p(new TextureImpl());
	return p;
#endif
}

TextureSharedPtr Texture::create(const std::string & fileName)
{
#if(USING_3DGEOMETRY_TYPE==0)//use plugin
	TextureSharedPtr pt = ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newTexture();
#else
	TextureSharedPtr pt (new TextureImpl());
#endif	
	Buffer buf;
	if (buf.loadFile(fileName))
	{
		std::string szext = fileName.substr(fileName.rfind('.') + 1);
		szext = (const char*)strupr((char*)szext.c_str());

		if (szext.compare("BMP") == 0)
		{
			pt->setType(Texture::TEXTURE_IMAGE_BMP);
		}
		else if (szext.compare("DDS") == 0)
		{
			pt->setType(Texture::TEXTURE_IMAGE_DDS);
		}
		else if (szext.compare("TGA") == 0)
		{
			pt->setType(Texture::TEXTURE_IMAGE_TGA);
		}
		else if (szext.compare("TIF") == 0 || szext.compare("TIFF") == 0)
		{
			pt->setType(Texture::TEXTURE_IMAGE_TIF);
		}
		else if (szext.compare("JPG") == 0 || szext.compare("JPEG") == 0)
		{
			pt->setType(Texture::TEXTURE_IMAGE_JPG);
		}
		else if (szext.compare("PNG") == 0)
		{
			pt->setType(Texture::TEXTURE_IMAGE_PNG);
		}
		else if (szext.compare("GIF") == 0)
		{
			pt->setType(Texture::TEXTURE_IMAGE_GIF);
		}
		else if (szext.compare("MNG") == 0)
		{
			pt->setType(Texture::TEXTURE_IMAGE_MNG);
		}
		else
			return TextureSharedPtr();

		pt->setID(Identifier::generate());
		std::string fname = fileName.substr(fileName.rfind('\\') + 1);
		pt->setName(fname);
		size_t count_s = buf.size();
		pt->setDataSize(count_s);
		pt->setData((unsigned char *)buf.detach(count_s));
		return pt;
	}
	return TextureSharedPtr();
}


end_gdb_namespace
end_gtl_namespace


