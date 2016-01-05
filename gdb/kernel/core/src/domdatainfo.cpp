#include "config.h"
#include "terrain.h" 
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


DOMDataInfo::DOMDataInfo()
{
	/*memset( version, '\0', VERINFOLENGTH );*/
	xDistance = 0.0;
	yDistance = 0.0;
	resolution = 0.0;
	bitsCount = 24;
	coordinateUnit[0] = 'M';
	xOverlayDistance = 0.0;
	yOverlayDistance = 0.0;
	scale = 2000;

	xOverlayNumber = 0;
	yOverlayNumber = 0;
	compressFormat = 0x00;
	compressQuilty = 0;

	demLayersNumber = 0;
	originDEMCellSize = 0.0;
	paletteReady = 0;
	indexedLayersNumber = 0;
	grayLayersNumber = 0;

	userDefBits = 0;		//用户定义影象位数以递交的第一幅图象为准
	cvs8To24 = 1;
	memset(palette, 0, sizeof(PALETTEENTRY) * 256);

	blockFlagNumber = 4;
	power2Blk = 0;

	xSOPixelNumber = 0;
	ySOPixelNumber = 0;

	reserved1 = -1;
	reserved2 = -1;
	reserved3 = -1;
	reserved4 = -1;
}

bool DOMDataInfo::operator == (DOMDataInfo &other)
{
	if (fabs(xDistance - other.xDistance) < 0.0001 && fabs(yDistance - other.yDistance) < 0.0001 && fabs(resolution - other.resolution) < 0.0001
		&& !strcmp(coordinateUnit, other.coordinateUnit) && bitsCount == other.bitsCount && fabs(xOverlayDistance - other.xOverlayDistance) < 0.0001
		&& fabs(yOverlayDistance - other.yOverlayDistance) < 0.0001
		&& xOverlayNumber == other.xOverlayNumber && yOverlayNumber == other.yOverlayNumber
		&& compressFormat == other.compressFormat && compressQuilty == other.compressQuilty)
		return true;
	else
		return false;
}

bool DOMDataInfo::operator != (DOMDataInfo &other)
{
	if (fabs(xDistance - other.xDistance) > 0.0001 || fabs(yDistance - other.yDistance) > 0.0001 || fabs(resolution - other.resolution) > 0.0001
		|| strcmp(coordinateUnit, other.coordinateUnit) || bitsCount == other.bitsCount || fabs(xOverlayDistance - other.xOverlayDistance) > 0.0001
		|| fabs(yOverlayDistance - other.yOverlayDistance) > 0.0001
		|| xOverlayNumber != other.xOverlayNumber || yOverlayNumber != other.yOverlayNumber
		|| compressFormat != other.compressFormat || compressQuilty != other.compressQuilty)
		return true;
	else
		return false;
}


bool DOMDataInfo::writeBuffer(Buffer& buf){
	/*size_t s = version.size();
	buf.write((void*)(&s),sizeof(size_t));
	buf.write((void*)(&version),s);*/
	buf.write(version);

	buf.write((void*)(&xDistance), sizeof(double));
	buf.write((void*)(&yDistance), sizeof(double));
	buf.write((void*)(&resolution), sizeof(double));

	buf.write((void*)(&bitsCount), sizeof(int));
	buf.write((void*)(&coordinateUnit), sizeof(char) * 1);
	buf.write((void*)(&xOverlayDistance), sizeof(double));
	buf.write((void*)(&yOverlayDistance), sizeof(double));
	buf.write((void*)(&scale), sizeof(int));

	buf.write((void*)(&xOverlayNumber), sizeof(int));
	buf.write((void*)(&yOverlayNumber), sizeof(int));
	buf.write((void*)(&compressFormat), sizeof(unsigned char));
	buf.write((void*)(&compressQuilty), sizeof(int));

	buf.write((void*)(&demLayersNumber), sizeof(int));
	buf.write((void*)(&originDEMCellSize), sizeof(double));
	buf.write((void*)(&paletteReady), sizeof(unsigned char));
	buf.write((void*)(&indexedLayersNumber), sizeof(int));
	buf.write((void*)(&grayLayersNumber), sizeof(int));

	buf.write((void*)(&userDefBits), sizeof(unsigned char));
	buf.write((void*)(&cvs8To24), sizeof(unsigned char));

	/*for(int i = 0;i<256;i++)
	{
	PALETTEENTRY  temPal;
	temPal = palette[i];
	buf.write((void*)(temPal.peBlue),sizeof(BYTE));
	buf.write((void*)(temPal.peGreen),sizeof(BYTE));
	buf.write((void*)(temPal.peRed),sizeof(BYTE));
	buf.write((void*)(temPal.peFlags),sizeof(BYTE));
	}*/
	buf.write((void*)palette, sizeof(PALETTEENTRY) * 256);

	buf.write((void*)(&blockFlagNumber), sizeof(unsigned char));
	buf.write((void*)(&power2Blk), sizeof(unsigned char));
	buf.write((void*)(&xSOPixelNumber), sizeof(unsigned char));
	buf.write((void*)(&ySOPixelNumber), sizeof(unsigned char));

	buf.write((void*)(&reserved1), sizeof(char));
	buf.write((void*)(&reserved2), sizeof(char));
	buf.write((void*)(&reserved3), sizeof(char));
	buf.write((void*)(&reserved4), sizeof(char));

	return true;
}

bool DOMDataInfo::readBuffer(Buffer& buf){
	/*size_t s = 0;
	buf.read((void*)(&s),sizeof(size_t));
	buf.read((void*)(&version),s);*/
	version = buf.readString();

	buf.read((void*)(&xDistance), sizeof(double));
	buf.read((void*)(&yDistance), sizeof(double));
	buf.read((void*)(&resolution), sizeof(double));

	buf.read((void*)(&bitsCount), sizeof(int));
	buf.read((void*)(&coordinateUnit), sizeof(char) * 1);
	buf.read((void*)(&xOverlayDistance), sizeof(double));
	buf.read((void*)(&yOverlayDistance), sizeof(double));
	buf.read((void*)(&scale), sizeof(int));

	buf.read((void*)(&xOverlayNumber), sizeof(int));
	buf.read((void*)(&yOverlayNumber), sizeof(int));
	buf.read((void*)(&compressFormat), sizeof(unsigned char));
	buf.read((void*)(&compressQuilty), sizeof(int));

	buf.read((void*)(&demLayersNumber), sizeof(int));
	buf.read((void*)(&originDEMCellSize), sizeof(double));
	buf.read((void*)(&paletteReady), sizeof(unsigned char));
	buf.read((void*)(&indexedLayersNumber), sizeof(int));
	buf.read((void*)(&grayLayersNumber), sizeof(int));

	buf.read((void*)(&userDefBits), sizeof(unsigned char));
	buf.read((void*)(&cvs8To24), sizeof(unsigned char));

	/*for(int i = 0;i<256;i++)
	{
	PALETTEENTRY  temPal;

	buf.read((void*)(temPal.peBlue),sizeof(BYTE));
	buf.read((void*)(temPal.peGreen),sizeof(BYTE));
	buf.read((void*)(temPal.peRed),sizeof(BYTE));
	buf.read((void*)(temPal.peFlags),sizeof(BYTE));
	palette[i]=temPal;
	}*/
	buf.read((void*)palette, sizeof(PALETTEENTRY) * 256);

	buf.read((void*)(&blockFlagNumber), sizeof(unsigned char));
	buf.read((void*)(&power2Blk), sizeof(unsigned char));
	buf.read((void*)(&xSOPixelNumber), sizeof(unsigned char));
	buf.read((void*)(&ySOPixelNumber), sizeof(unsigned char));

	buf.read((void*)(&reserved1), sizeof(char));
	buf.read((void*)(&reserved2), sizeof(char));
	buf.read((void*)(&reserved3), sizeof(char));
	buf.read((void*)(&reserved4), sizeof(char));

	return true;
}


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace