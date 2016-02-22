#include "config.h"
#include "terrain.h" 


begin_gtl_namespace
begin_gdb_namespace

DOMBlock::DOMBlock()
{
	imageData = NULL;
	valid = 0;
	full = 1;
	layerID = 0;
	row = 0;
	column = 0;
	xPixelsNumber = 0;
	yPixelsNumber = 0;
	imageLength = 0;
}

void DOMBlock::clear()
{
	valid = 0;
	SAFE_DELETE(imageData);
}

DOMBlock DOMBlock::operator=(const DOMBlock &other){
	blockID = other.blockID;
	column = other.column;
	full = other.full;
	imageLength = other.imageLength;
	layerID = other.layerID;
	row = other.row;
	scope = other.scope;
	valid = other.valid;
	xPixelsNumber = other.xPixelsNumber;
	yPixelsNumber = other.yPixelsNumber;
	if (other.imageLength > 0)
	{
		SAFE_DELETE(imageData);
		imageData = new  unsigned char[other.imageLength];
		memcpy(imageData, other.imageData, other.imageLength);
	}
	else{
		imageData = NULL;
	}
	return *this;
}

DOMBlock::~DOMBlock()
{
	clear();
}


bool DOMBlock::writeBuffer(Buffer& buf){

	buf.write((void*)(&blockID), sizeof(Identifier::raw_type));
	buf.write((void*)(&layerID), sizeof(Identifier::raw_type));
	buf.write((void*)(&row), sizeof(int));
	buf.write((void*)(&column), sizeof(int));
	buf.write((void*)(&full), sizeof(int));
	buf.write((void*)(&xPixelsNumber), sizeof(int));
	buf.write((void*)(&yPixelsNumber), sizeof(int));

	double v[4];
	scope.get(v);
	buf.write((void*)v, sizeof(double) * 4);
	//这里要检查一下imageData的数据长度是不是等于xPixelsNumber * yPixelsNumber

	//int  s = imageLength;
	//buf.write((void*)(&s),sizeof(int));
	buf.write((void*)(&imageLength), sizeof(int));
	if (imageLength>0)
	{
		buf.write((void*)imageData, imageLength*sizeof(unsigned char));
	}
	buf.write((void*)(&valid), sizeof(int));

	return true;
}

bool DOMBlock::readBuffer(Buffer& buf){
	buf.read((void*)(&blockID), sizeof(Identifier::raw_type));
	buf.read((void*)(&layerID), sizeof(Identifier::raw_type));
	buf.read((void*)(&row), sizeof(int));
	buf.read((void*)(&column), sizeof(int));
	buf.read((void*)(&full), sizeof(int));
	buf.read((void*)(&xPixelsNumber), sizeof(int));
	buf.read((void*)(&yPixelsNumber), sizeof(int));

	double v[4];
	buf.read((void*)v, sizeof(double) * 4);
	scope.set(v);
	//这里要检查一下imageData的数据长度是不是等于xPixelsNumber * yPixelsNumber
	/*int  s = 0;
	buf.read((void*)(&s),sizeof(int));*/
	buf.read((void*)(&imageLength), sizeof(int));
	if (imageLength>0)
	{
		if (imageData == 0)
			imageData = new  unsigned char[imageLength];
		buf.read((void*)imageData, imageLength*sizeof(unsigned char));
	}

	buf.read((void*)(&valid), sizeof(int));

	return true;
}

 

ImageSharedPtr DOMBlock::DOMBlock2Image(DOMBlockSharedPtr & p){
	return Image::create(p->xPixelsNumber, p->yPixelsNumber, p->computeBitsPrepixel(), (void*)(p->imageData));
}
//只能生成一个没有blockID号layid等信息的block
DOMBlockSharedPtr DOMBlock::Image2DOMBlock(ImageSharedPtr &image){
	DOMBlockSharedPtr _dom = DOMBlockSharedPtr(new DOMBlock());
	int bpp_div_8 = image->getBitsPerPixel() / 8;
	_dom->xPixelsNumber = image->getWidth();
	_dom->yPixelsNumber = image->getHeight();
	_dom->imageLength = image->getHeight()*image->getWidth() * bpp_div_8;

	_dom->imageData = new unsigned char[_dom->imageLength];

	unsigned char* pd = _dom->imageData;
	for (int i = 0; i<_dom->yPixelsNumber; i++)
	{
		for (int j = 0; j<_dom->xPixelsNumber; j++)
		{
			pd = _dom->imageData + (_dom->xPixelsNumber*i + j)*image->getBitsPerPixel() / 8;
			Color4f cc;
			image->getPixel(j, i, cc);
			pd[0] = cc.r;
			pd[1] = cc.g;
			pd[2] = cc.b;
		}
		//memcpy(_dom->getRowPixels(i),image->getScanLine(i),_dom->xPixelsNumber*bpp_div_8);
		//memmove(_dom->getRowPixels(i),image->getScanLine(i),_dom->xPixelsNumber*bpp_div_8);
	}

	return _dom;
}

DOMBlockSharedPtr DOMBlock::merge(const DOMBlockSharedPtr & p1, const DOMBlockSharedPtr & p2)
{
	if (p1.get() == 0 || p2.get() == 0)
		return DOMBlockSharedPtr();
	if (p1->xPixelsNumber == p2->xPixelsNumber && p1->yPixelsNumber == p2->yPixelsNumber)
		return DOMBlockSharedPtr();
	if ((!p1->valid) && (!p2->valid)){
		DOMBlockSharedPtr p(new DOMBlock());
		p->valid = false;
		return p;
	}
	int bpp_div_8 = p1->imageLength / (p1->xPixelsNumber*p1->yPixelsNumber);

	/*
	Identifier::raw_type		    blockID;				//DOM数据块ID
	Identifier::raw_type			layerID;				//金字塔层ID
	int			row;					                    //数据块的行号
	int			column;					                    //数据块的列号
	int			full;					                    //是否包含填充的无效值,1表示没有，0表示有无效值
	int			xPixelsNumber;				                //数据块X方向像素数
	int			yPixelsNumber;				                //数据块Y方向像素数

	Envelope2d	  scope;					               //当前块在X、Y方向的范围
	unsigned char*imageData;				               //影像数据块
	int			  valid;					               //影像数据块的有效性
	int			  imageLength;
	*/
	DOMBlockSharedPtr p(new DOMBlock());
	p->full = p1->full&&p2->full;
	p->scope = p1->scope;
	p->scope.extend(p2->scope);
	p->imageLength = p1->imageLength + p2->imageLength;
	p->imageData = new unsigned char[p->imageLength];
	p->valid = true;

	if (fabs(p1->scope.minx - p2->scope.minx) <= TOLERANCE)//p1,p2纵向排列
	{
		p->xPixelsNumber = p1->xPixelsNumber;
		p->yPixelsNumber = p1->yPixelsNumber * 2;

		if (p1->scope.miny>p2->scope.miny){//p1在上面，p2在下面
			memcpy(p->imageData, p2->imageData, p2->imageLength);
			memcpy(p->imageData + p2->imageLength, p1->imageData, p1->imageLength);
		}
		else{//p2在上面，p1在下面				
			memcpy(p->imageData, p1->imageData, p1->imageLength);
			memcpy(p->imageData + p1->imageLength, p2->imageData, p2->imageLength);
		}
	}
	else{//p1,p2横向排列
		p->xPixelsNumber = p1->xPixelsNumber * 2;
		p->yPixelsNumber = p1->yPixelsNumber;
		unsigned char * pr = 0;
		unsigned char * p1r = 0;
		unsigned char * p2r = 0;
		if (p1->scope.minx<p2->scope.minx){//p1 left, p2 right
			for (int i = 0; i<p->yPixelsNumber; i++){
				pr = p->getRowPixels(i);
				p1r = p1->getRowPixels(i);
				p2r = p2->getRowPixels(i);
				memcpy(pr, p1r, p1->xPixelsNumber*bpp_div_8);
				memcpy(pr + (p1->xPixelsNumber*bpp_div_8), p2r, p2->xPixelsNumber*bpp_div_8);
			}
		}
		else{//p2 left, p1 right
			for (int i = 0; i<p->yPixelsNumber; i++){
				pr = p->getRowPixels(i);
				p1r = p1->getRowPixels(i);
				p2r = p2->getRowPixels(i);
				memcpy(pr, p2r, p2->xPixelsNumber*bpp_div_8);
				memcpy(pr + (p2->xPixelsNumber*bpp_div_8), p1r, p1->xPixelsNumber*bpp_div_8);
			}
		}
	}
	return p;
}

DOMBlockSharedPtr DOMBlock::resample(const DOMBlockSharedPtr & p, int newx, int newy)
{
	if (newx == 0 || newy == 0) return DOMBlockSharedPtr();

	if (p->xPixelsNumber == newx &&p->yPixelsNumber == newy){
		return DOMBlockSharedPtr(new DOMBlock(*p.get()));
	}

	float xScale, yScale, fX, fY;

	xScale = (float)p->xPixelsNumber / (float)newx;
	yScale = (float)p->yPixelsNumber / (float)newy;

	int bpp_div_8 = p->computeBitsPrepixel() / 8;

	DOMBlockSharedPtr des(new DOMBlock());
	des->full = p->full;
	des->scope = p->scope;
	des->scope = p->scope;
	des->xPixelsNumber = newx;
	des->yPixelsNumber = newy;
	des->valid = true;

	des->imageLength = newx*newy*bpp_div_8;
	des->imageData = new unsigned char[des->imageLength];

	// nearest pixel	
	for (int y = 0; y<newy; y++){
		fY = y * yScale;
		for (int x = 0; x<newx; x++){
			fX = x * xScale;
			memcpy(des->getPixel(x, y), p->getPixel((int)fX, (int)fY), bpp_div_8);
		}
	}
	return des;
}
end_gdb_namespace
end_gtl_namespace

