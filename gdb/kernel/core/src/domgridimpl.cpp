#include "internal_domgridimpl.h"
#include "terrainbuilder.h"


begin_gtl_namespace
begin_gdb_namespace
//##ModelId=49EF1205035B
// int CDEMGridObject::Interpolate(Vertex3d& vVer) const
// {
// 	// TODO: Add your specialized code here.
// 	// NOTE: Requires a correct return value to compile.
// }
// 
// //##ModelId=49EF141C0232
// CDEMGridObject::Interpolate(std : :vector<Vertex3d>& vVertices) const
// {
// }
// 
// //##ModelId=49EF1297030D
// Vertex3d CDEMGridObject::GetMaxPoint() const
// {
// 	// TODO: Add your specialized code here.
// 	// NOTE: Requires a correct return value to compile.
// }
// 
// //##ModelId=49EF12AC01E4
// Vertex3d CDEMGridObject::GetMinPoint() const
// {
// 	// TODO: Add your specialized code here.
// 	// NOTE: Requires a correct return value to compile.
// }
// 
// //##ModelId=49EF12FC03A9
// void CDEMGridObject::ScaleDEM()
// {
// 	// TODO: Add your specialized code here.
// }
// 
// //##ModelId=49EF136B037A
// Vertex3d CDEMGridObject::GetMaxPointInPolygon(C3DPolygon* pPolygon) const
// {
// 	// TODO: Add your specialized code here.
// 	// NOTE: Requires a correct return value to compile.
// }
// 
// //##ModelId=49EF138B0399
// Vertex3d CDEMGridObject::GetMinPointInPolygon(C3DPolygon* pPolygon) const
// {
// 	// TODO: Add your specialized code here.
// 	// NOTE: Requires a correct return value to compile.
// }
// 
// //##ModelId=49EF13AE00FA
// int CDEMGridObject::Clip(C3DPolygon* pPolygon)
// {
// 	// TODO: Add your specialized code here.
// 	// NOTE: Requires a correct return value to compile.
// }
// 
// //##ModelId=49EF13E7001F
// void CDEMGridObject::Extract(int nRow, int nCol)
// {
// 	// TODO: Add your specialized code here.
// }
// 
// //##ModelId=49EF144F00EA
// void CDEMGridObject::GetDemBound(C3DPolygon* pPolygon)
// {
// 	// TODO: Add your specialized code here.
// }


long DOMGridImpl::CopyDom(DOMGridImpl& src, DOMGridImpl& tar, bool bCopyData)
{
	double resolvx = 0;
	double resolvy = 0;

	double east = 0;
	double west = 0;
	double north = 0;
	double south = 0;

	int ImageW = 0;
	int ImageH = 0;
	int ImageD = 0;

	unsigned long long datalength = 0;

	unsigned char* pData = NULL;

	src.GetDomPara(resolvx, resolvy, east, west, north, south, ImageW, ImageH, ImageD, pData, datalength, bCopyData);
	tar.SetDom(resolvx, resolvy, east, west, north, south, ImageW, ImageH, ImageD, pData, datalength, bCopyData);

	return 1;
}



void DOMGridImpl::setParameters(double resolvx, double resolvy, double east, double west, double north, double south,
	int ImageW, int ImageH, int ImageD, unsigned char * pData,
	unsigned long long datalength, bool bAlloced)
{

	m_resolvx = resolvx;
	m_resolvy = resolvy;

	m_east = east;
	m_west = west;
	m_north = north;
	m_south = south;

	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageD = ImageD;

	if (m_resolvx == 0)
		m_resolvx = fabs(m_east - m_west) / m_ImageW;

	if (m_resolvy == 0)
		m_resolvy = fabs(m_north - m_south) / m_ImageH;

	if (m_pData)
	{
		if (m_bDomDataAlloced)
		{
			delete[]m_pData;
		}
		m_pData = NULL;
	}

	m_bDomDataAlloced = bAlloced;
	m_pDataLength = datalength;
	if (m_bDomDataAlloced){
		m_pData = new unsigned char[m_pDataLength];
		//memcpy(m_pData,pData,m_pDataLength);
		memmove(m_pData, pData, m_pDataLength);
	}
	else
		m_pData = pData;
}

void DOMGridImpl::setParameters(double east, double west, double north, double south, const ImageSharedPtr & p){
	m_resolvx = p->getHorizontalResolution();
	m_resolvy = p->getVerticalResolution();

	m_east = east;
	m_west = west;
	m_north = north;
	m_south = south;

	m_ImageW = p->getWidth();
	m_ImageH = p->getHeight();
	m_ImageD = 1;

	if (m_resolvx == 0)
		m_resolvx = fabs(m_east - m_west) / m_ImageW;

	if (m_resolvy == 0)
		m_resolvy = fabs(m_north - m_south) / m_ImageH;

	if (m_pData)
	{
		if (m_bDomDataAlloced)
		{
			delete[]m_pData;
		}
		m_pData = NULL;
	}

	m_bDomDataAlloced = true;
	int bpp_div_8 = p->getBitsPerPixel() / 8;
	m_pDataLength = bpp_div_8*m_ImageW*m_ImageH;

	try{
		m_pData = new unsigned char[m_pDataLength];
	}
	catch (...)
	{
		std::cout << "Allocate Memory  Error!" << std::endl;
	}

	for (unsigned int i = 0; i<m_ImageH; i++){
		memcpy(m_pData + i*m_ImageW*bpp_div_8, p->getScanLine(i), m_ImageW*bpp_div_8);
	}

}
void DOMGridImpl::getParameters(double& resolvx, double& resolvy, double& east, double& west, double& north, double& south,
	int& ImageW, int& ImageH, int& ImageD, unsigned char *& pData,
	unsigned long long & datalength, bool bAlloced){
	GetDomPara(resolvx, resolvy, east, west, north, south, ImageW, ImageH, ImageD, pData, datalength, bAlloced);
}

void DOMGridImpl::getResolution(double& resolvx, double& resolvy){
	GetDomResolv(resolvx, resolvy);
}

void DOMGridImpl::getDirection(double& east, double& west, double& north, double& south)
{
	GetDomDir(east, west, north, south);
}

void DOMGridImpl::getImageSize(int& ImageW, int& ImageH, int& ImageD){
	GetDomImageSize(ImageW, ImageH, ImageD);
}

unsigned char * DOMGridImpl::getImageData()
{
	return GetDomData();
}

unsigned long long  DOMGridImpl::getImageLength()
{
	return GetImageLength();
}

unsigned int DOMGridImpl::getBitsPerPixel() const
{
	return m_pDataLength / (m_ImageW*m_ImageH*m_ImageD) * 8;
}
DOMBlockSharedPtr DOMGridImpl::clip(const Envelope2d & rect){
	DOMBlockSharedPtr p;
	Envelope2d e_grid(m_west, m_east, m_south, m_north);
	Envelope2d e_block = rect;
	int flag = Envelope2d::test(e_block, e_grid);
	//* 0-表示两个包围盒相离
	//* 1-表示两个包围盒a和b相交
	//* 2-表示包围盒a在b中
	//* 3-表示包围盒b在a中
	if (flag == 0){
		return p;
	}

	p = DOMBlockSharedPtr(new DOMBlock());
	p->blockID = 0;
	p->layerID = 0;
	int x1, x2, y1, y2;
	WP2IP(rect.minx, rect.miny, &x1, &y1);
	WP2IP(rect.maxx, rect.maxy, &x2, &y2);

	//int xx1 = x1-1;
	//int xx2 = x2-1;
	//int yy1 = y1-1;
	//int yy2 = y2-1;

	p->xPixelsNumber = x2 - x1/*+1*/;
	p->yPixelsNumber = y2 - y1/*+1*/;
	p->row = 0;
	p->column = 0;
	p->scope = rect;
	p->imageLength = p->xPixelsNumber*p->yPixelsNumber*m_ImageD*getBitsPerPixel() / 8;
	p->imageData = new unsigned char[p->imageLength];
	memset(p->imageData, 0, p->imageLength);
	p->valid = 1;
	if (flag == 1){//部分填充
		p->full = 0;
	}
	else if (flag == 2){//全部填充
		p->full = 1;
	}
	else{//部分填充
		p->full = 0;
	}
	unsigned char * pd = p->imageData;
	unsigned char * pid = 0;


	for (int y = max(y1, 0); y<min(y2, m_ImageH); y++){
		for (int x = max(x1, 0); x<min(x2, m_ImageW); x++){
			pd = p->imageData + ((y - y1)*p->xPixelsNumber + (x - x1))*getBitsPerPixel() / 8;
			pid = getPixel(x, y);
			if (pid)
				memcpy(pd, pid, getBitsPerPixel() / 8);
		}
	}
	return p;
}
unsigned char * DOMGridImpl::getPixel(int ix, int iy){
	if (ix<0 || ix >= m_ImageW || iy<0 || iy >= m_ImageH)
	{//如果超出范围返回无效像素值
		return 0;
	}
	else
	{
		return m_pData + getBitsPerPixel() / 8 * (iy*m_ImageW + ix);
	}
}

unsigned char * DOMGridImpl::getPixel(double x, double y){
	int ix, iy;
	WP2IP(x, y, &ix, &iy);
	return getPixel(ix, iy);
}

void DOMGridImpl::IP2WP(int x, int y, double * wx, double * wy){
	*wx = x * m_resolvx + m_west;
	*wy = y * m_resolvy + m_south;
}
void DOMGridImpl::WP2IP(double   wx, double   wy, int *ix, int *iy){
	*ix = floor((wx - m_west) / m_resolvx);
	*iy = floor((wy - m_south) / m_resolvy);
}
//rect.lengthX() 计算出xPixelNumber和yPixelMumber
unsigned long long DOMGridImpl::calculateXPixelNmuber(double length){
	return floor(length*m_ImageW / (m_east - m_west));
}
unsigned long long  DOMGridImpl::calculateYPixelNmuber(double length){
	return floor(length*m_ImageH / (m_north - m_south));
}

DOMBlockSharedPtr DOMGridImpl::clip(int begX, int xNumbers, int begY, int yNumbers){
	DOMBlockSharedPtr p;
	if (begX>m_ImageW || begY>m_ImageH || ((begX + xNumbers)<0) || (begY + yNumbers)<0)
	{
		return p;
	}
	p = DOMBlockSharedPtr(new DOMBlock());

	p->xPixelsNumber = xNumbers;
	p->yPixelsNumber = yNumbers;
	p->row = 0;
	p->column = 0;
	p->imageLength = p->xPixelsNumber*p->yPixelsNumber*m_ImageD*getBitsPerPixel() / 8;
	p->imageData = new unsigned char[p->imageLength];
	memset(p->imageData, 0, p->imageLength);
	p->valid = 1;
	//范围和是否全部填充到上一层与ID行列号一起赋值

	unsigned char * pd = p->imageData;
	unsigned char * pid = 0;
	//控制点不在提交的grid上时的范围
	int pbx = 0;
	int pby = 0;
	if (begX<0)
	{
		xNumbers += begX;
		pbx = abs(begX);
		begX = 0;
	}
	if (begY<0)
	{
		yNumbers += begY;
		pby = abs(begY);
		begY = 0;
	}
	if (begX + xNumbers>m_ImageW)
		xNumbers = m_ImageW - begX;
	if (begY + yNumbers>m_ImageH)
		yNumbers = m_ImageH - begY;

	for (int j = begY; j<yNumbers; j++)
	{
		for (int i = begX; i<xNumbers; i++)
		{
			pd = p->imageData + ((j + pby)*p->xPixelsNumber + (pbx + i))*getBitsPerPixel() / 8;
			pid = getPixel(i, j);
			if (pid)
				memcpy(pd, pid, getBitsPerPixel() / 8);
		}
	}
	return p;
}

void DOMGridImpl::clip(DOMLayerInfoSharedPtr lay, std::vector<DOMBlockSharedPtr> & blocks){
	Envelope2d e2d;
	getDirection(e2d.maxx, e2d.minx, e2d.maxy, e2d.miny);

	Envelope2d block_e2d;
	DOMBlockSharedPtr dsp;
	int xPixNumbers = lay->xPixelsNumber / lay->xBlocksNumber;
	int yPixNumbers = lay->yPixelsNumber / lay->yBlocksNumber;


	unsigned long long beggridrow, beggridcol, endgridrow, endgridcol;
	lay->WP2IP(m_west, m_south, &beggridcol, &beggridrow);
	lay->WP2IP(m_east, m_north, &endgridcol, &endgridrow);

	////方案1按照grid像素
	//for(int i =0;i<m_ImageW;i++)
	//{
	//	for(int j =0;j<m_ImageH;j++)
	//   {
	//	   //lay->calculate(

	//   }
	//}
	//方案2按照每个block的起点像素,_isbegin是用于判断是否得到第一个提交的block在lay中对应的起始row,col;其余的通过计算得出
	bool _isbegin = false;
	unsigned long long beglaycol, beglayrow;
	unsigned long long _begtemp;

	for (long j = 0; j<lay->yBlocksNumber; j++){
		for (long i = 0; i<lay->xBlocksNumber; i++){
			block_e2d = lay->calcualteBlockEnvelope(j, i);
			int flag = Envelope2d::test(block_e2d, e2d);
			if (flag != 0){
				if (!_isbegin)
				{
					lay->WP2IP(block_e2d.minx, block_e2d.miny, &beglaycol, &beglayrow);
					_begtemp = beglaycol;
					_isbegin = true;
				}
				dsp = DOMBlockSharedPtr(new DOMBlock());
				dsp->xPixelsNumber = xPixNumbers;
				dsp->yPixelsNumber = yPixNumbers;
				dsp->row = j;
				dsp->column = i;
				dsp->imageLength = xPixNumbers*yPixNumbers*m_ImageD*getBitsPerPixel() / 8;
				dsp->imageData = new unsigned char[dsp->imageLength];
				memset(dsp->imageData, 0, dsp->imageLength);
				dsp->valid = 1;

				if (flag == 2){//部分填充
					dsp->full = 1;
				}
				else {//部分填充
					dsp->full = 0;
				}
				dsp->scope = block_e2d;
				dsp->blockID = TerrainBuilder::calculate(j, i);
				unsigned char* pid = 0;
				unsigned char* pd = dsp->imageData;
				//for(unsigned long long j =beglayrow;j<beglayrow+yPixNumbers;j++ ){
				//	for(unsigned long long i=beglaycol;i<beglayrow+xPixNumbers;i++)
				//	{
				//		pid = getlayPixel(lay,j,i);
				//		pd  = dsp->imageData + (j*xPixNumbers+i)*getBitsPerPixel()/8;
				//		if(pid)
				//		{
				//			memcpy(pd,pid,getBitsPerPixel()/8);
				//		}
				//	}
				//}
				for (unsigned long long t = max(beglayrow, beggridrow); t<min(beglayrow + yPixNumbers, endgridrow); t++)
				{
					for (unsigned long long s = max(beglaycol, beggridcol); s<min(beglaycol + xPixNumbers, endgridcol); s++)
					{
						pd = dsp->imageData + ((t - beglayrow)*xPixNumbers + s - beglaycol)*getBitsPerPixel() / 8;
						pid = getLayerPixel(lay, t, s);
						if (pid)
						{
							memcpy(pd, pid, getBitsPerPixel() / 8);
						}
					}
				}
				blocks.push_back(dsp);
				beglaycol += xPixNumbers;
			}
		}
		if (_isbegin)
		{
			beglayrow += yPixNumbers;
			beglaycol = _begtemp;
		}
	}

}

unsigned char* DOMGridImpl::getLayerPixel(DOMLayerInfoSharedPtr lay, int row, int col){
	unsigned long long beggridrow, beggridcol, endgridrow, endgridcol;
	lay->WP2IP(m_west, m_south, &beggridcol, &beggridrow);
	lay->WP2IP(m_east, m_north, &endgridcol, &endgridrow);
	if ((col>endgridcol) || (col<beggridcol) || (row>endgridrow) || (row<beggridrow))
	{
		return 0;
	}
	else
	{
		int bpp_div_8 = getBitsPerPixel() / 8;
		unsigned char* pd = m_pData + bpp_div_8*((row - beggridrow)*m_ImageW + (col - beggridcol));
		//return m_pData + bpp_div_8*((row-beggridrow)*m_ImageW + (col-beggridcol)); 
		return pd;
	}
}

end_gdb_namespace
end_gtl_namespace



