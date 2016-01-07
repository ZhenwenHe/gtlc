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
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "identifier.h"



begin_gtl_namespace
begin_gdb_namespace

class Texture;
typedef std::shared_ptr<Texture>  TextureSharedPtr; 
typedef Texture*  TexturePtr;
/** @defgroup Texture Texture 
*  @{
*/
class CORE_API Texture  
{
public:
	enum
	{
		TEXTURE_IMAGE_BMP=0,
		TEXTURE_IMAGE_DDS=2,//为兼用一些老数据，1也为DDS，并且默认的为DDS
		TEXTURE_IMAGE_TGA=3,
		TEXTURE_IMAGE_TIF=4,
		TEXTURE_IMAGE_JPG=5,
		TEXTURE_IMAGE_PNG=6,
		TEXTURE_IMAGE_GIF=7,
		TEXTURE_IMAGE_MNG=8
	};
public:	
	virtual Identifier::raw_type getID() const=0;
	virtual void setID(Identifier::raw_type)=0;
	virtual std::string getName() const =0;
	virtual void setName(std::string strTexName) =0;
	virtual int getType() const =0;
	virtual void setType(int nTexType) =0;
	virtual size_t getDataSize() const =0;
	virtual void setDataSize(size_t length)=0 ;
	virtual unsigned char* getData() const=0;
	virtual void setData(unsigned char* texdata)=0;

	virtual int getWrapMode() const =0;
	virtual void setWrapMode(int nWrapMode) =0;

	static TextureSharedPtr create();

	static TextureSharedPtr create(const std::string & fileName);

};
/** @} */ 

end_gdb_namespace
end_gtl_namespace

