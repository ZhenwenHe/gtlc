#include "config.h"
#include "terrain.h" 


begin_gtl_namespace
begin_gdb_namespace

DOMInfo::DOMInfo()
{
	x0 = 0.0;
	y0 = 0.0;
	xResolution = 0.0;
	yResolution = 0.0;
	xPixelsNumber = 0;
	yPixelsNumber = 0;
	bitsCount = 24;
}


DOMInfo::DOMInfo(double x0, double y0, double resX, double resY, int nXPixelsNum, int nYPixelsNum, int nBitsNum)
{
	this->x0 = x0;
	this->y0 = y0;
	xResolution = resX;
	yResolution = resY;
	xPixelsNumber = nXPixelsNum;
	yPixelsNumber = nYPixelsNum;
	bitsCount = nBitsNum;
}


bool DOMInfo::writeBuffer(Buffer& buf){
	buf.write((void*)(&x0), sizeof(double));
	buf.write((void*)(&y0), sizeof(double));
	buf.write((void*)(&xResolution), sizeof(double));
	buf.write((void*)(&yResolution), sizeof(double));
	buf.write((void*)(&xPixelsNumber), sizeof(int));
	buf.write((void*)(&yPixelsNumber), sizeof(int));
	buf.write((void*)(&bitsCount), sizeof(int));

	return true;
}

bool DOMInfo::readBuffer(Buffer& buf){
	buf.read((void*)(&x0), sizeof(double));
	buf.read((void*)(&y0), sizeof(double));
	buf.read((void*)(&xResolution), sizeof(double));
	buf.read((void*)(&yResolution), sizeof(double));
	buf.read((void*)(&xPixelsNumber), sizeof(int));
	buf.read((void*)(&yPixelsNumber), sizeof(int));
	buf.read((void*)(&bitsCount), sizeof(int));

	return true;
}

end_gdb_namespace
end_gtl_namespace

