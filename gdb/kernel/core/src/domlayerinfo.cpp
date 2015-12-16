#include "config.h"
#include "terrain.h" 
#include "TerrainBuilder.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

DOMLayerInfo::DOMLayerInfo()
{
	layerID = 0;
	demLayerID = 0;
	domdbID = 0;
	/*strcpy_s(layerName, MAX_NAME, "");*/
	xBlocksNumber = 0;
	yBlocksNumber = 0;
	//xPixelsNumber = 256;
	//yPixelsNumber = 256;
	xPixelsNumber = 0;
	yPixelsNumber = 0;
	layerResolution = 0.0;
	scale = 2;
}


DOMLayerInfo::DOMLayerInfo(Identifier::raw_type lyrID, Identifier::raw_type demLyrID, Identifier::raw_type dbID, char* name, Envelope2d lyrScope, Envelope2d dataScope, int xBlocks, int yBlocks, int xPixels, int yPixels, double res,
	int nScale, int boriDataLayer, int btopLayer)
{
	layerID = lyrID;
	domdbID = dbID;
	demLayerID = demLyrID;
	//strcpy_s(layerName, MAX_NAME, name);
	layerName = (const char *)name;

	layerScope = lyrScope;
	dataScope = dataScope;
	xBlocksNumber = xBlocks;
	yBlocksNumber = yBlocks;
	xPixelsNumber = xPixels;
	yPixelsNumber = yPixels;
	layerResolution = res;
	scale = nScale;
}

DOMLayerInfo DOMLayerInfo::operator=(const DOMLayerInfo &other)
{
	layerID = other.layerID;
	demLayerID = other.demLayerID;
	domdbID = other.domdbID;
	//strcpy_s(layerName, MAX_NAME, other.layerName);
	layerName = other.layerName;
	layerScope = other.layerScope;
	dataScope = other.dataScope;
	xBlocksNumber = other.xBlocksNumber;
	yBlocksNumber = other.yBlocksNumber;
	xPixelsNumber = other.xPixelsNumber;
	yPixelsNumber = other.yPixelsNumber;
	layerResolution = other.layerResolution;
	scale = other.scale;
	return *this;
}

bool DOMLayerInfo::operator == (DOMLayerInfo &other)
{
	if (xBlocksNumber == other.xBlocksNumber && yBlocksNumber == other.yBlocksNumber
		&& xPixelsNumber == other.xPixelsNumber && yPixelsNumber == other.yPixelsNumber
		&& fabs(layerResolution - other.layerResolution) < 0.001 && scale == other.scale)
		return true;
	else
		return false;
}


bool DOMLayerInfo::operator != (DOMLayerInfo &other)
{
	if (xBlocksNumber != other.xBlocksNumber || yBlocksNumber != other.yBlocksNumber
		|| xPixelsNumber != other.xPixelsNumber || yPixelsNumber != other.yPixelsNumber
		|| fabs(layerResolution - other.layerResolution) > 0.001 && scale == other.scale)
		return true;
	else
		return false;
}

void DOMLayerInfo::clear()
{
	layerID = 0;
	demLayerID = 0;
	domdbID = 0;
	//strcpy_s(layerName, MAX_NAME, "");
	layerName.clear();
	xBlocksNumber = 0;
	yBlocksNumber = 0;
	xPixelsNumber = 256;
	yPixelsNumber = 256;
	layerResolution = 0.0;
	scale = 2;
	layerScope = Envelope2d();
	dataScope = Envelope2d();
}
Envelope2d DOMLayerInfo::calcualteBlockEnvelope(unsigned int block_row_in_layer, unsigned int block_col_in_layer){
	//这里的blocksize需要经过计算出来
	double _blockSize = min((layerScope.maxx - layerScope.minx) / xBlocksNumber, (layerScope.maxy - layerScope.miny) / yBlocksNumber);
	return Envelope2d(layerScope.minx + block_col_in_layer*_blockSize,
		layerScope.minx + (block_col_in_layer + 1)*_blockSize,
		layerScope.miny + block_row_in_layer*_blockSize,
		layerScope.miny + (block_row_in_layer + 1)*_blockSize);
}

void DOMLayerInfo::calculate(unsigned long long  row, unsigned long long col, Identifier::raw_type &blkID, int &blockRow, int &blockCol){
	int i = col / xPixelsNumber;
	int j = row / yPixelsNumber;
	blkID = TerrainBuilder::calculate(j, i);
	blockRow = col%xPixelsNumber;
	blockCol = row%yPixelsNumber;
}

void DOMLayerInfo::calculate(Identifier::raw_type blkID, int blockRow, int blockCol, unsigned long long  &row, unsigned long long &col){
	int i, j;
	TerrainBuilder::calculate(blkID, j, i);
	col = i*xPixelsNumber + blockCol;
	row = j*yPixelsNumber + blockRow;
}

void DOMLayerInfo::IP2WP(unsigned long long  col, unsigned long long row, double * wx, double * wy){
	*wx = col*layerScope.lengthX() / (xPixelsNumber/**xBlocksNumber*/)+layerScope.minx;
	*wy = row*layerScope.lengthY() / (yPixelsNumber/**yBlocksNumber*/)+layerScope.miny;
}
void DOMLayerInfo::WP2IP(double   wx, double   wy, unsigned long long *col, unsigned long long *row){
	*col = (wx - layerScope.minx)*xPixelsNumber/**xBlocksNumber*/ / layerScope.lengthX();
	*row = (wy - layerScope.miny)*yPixelsNumber/**yBlocksNumber*/ / layerScope.lengthY();
}



bool DOMLayerInfo::writeBuffer(Buffer& buf){
	buf.write((void*)(&layerID), sizeof(Identifier::raw_type));
	buf.write((void*)(&demLayerID), sizeof(Identifier::raw_type));
	buf.write((void*)(&domdbID), sizeof(Identifier::raw_type));

	double v[4];
	layerScope.get(v);
	buf.write((void*)v, sizeof(double) * 4);

	double t[4];
	dataScope.get(t);
	buf.write((void*)t, sizeof(double) * 4);

	/*size_t s = layerName.size();
	buf.write((void*)(&s),sizeof(size_t));
	if(s>0)
	buf.write((void*)(&layerName),s);*/
	buf.write(layerName);
	buf.write((void*)(&layerResolution), sizeof(double));

	buf.write((void*)(&xPixelsNumber), sizeof(int));
	buf.write((void*)(&yPixelsNumber), sizeof(int));
	buf.write((void*)(&xBlocksNumber), sizeof(int));
	buf.write((void*)(&yBlocksNumber), sizeof(int));
	buf.write((void*)(&scale), sizeof(int));

	return true;
}

bool DOMLayerInfo::readBuffer(Buffer& buf){
	buf.read((void*)(&layerID), sizeof(Identifier::raw_type));
	buf.read((void*)(&demLayerID), sizeof(Identifier::raw_type));
	buf.read((void*)(&domdbID), sizeof(Identifier::raw_type));

	double v[4];
	buf.read((void*)v, sizeof(double) * 4);
	layerScope.set(v);

	double t[4];
	buf.read((void*)t, sizeof(double) * 4);
	dataScope.set(t);

	/*size_t s = 0;

	buf.read((void*)(&s),sizeof(size_t));
	if(s>0){

	buf.read((void*)(&layerName),s);
	}*/
	layerName = buf.readString();

	buf.read((void*)(&layerResolution), sizeof(double));

	buf.read((void*)(&xPixelsNumber), sizeof(int));
	buf.read((void*)(&yPixelsNumber), sizeof(int));
	buf.read((void*)(&xBlocksNumber), sizeof(int));
	buf.read((void*)(&yBlocksNumber), sizeof(int));
	buf.read((void*)(&scale), sizeof(int));

	return true;
}



end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace