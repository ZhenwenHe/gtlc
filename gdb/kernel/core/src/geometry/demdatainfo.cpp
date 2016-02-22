#include "config.h"
#include "terrain.h" 


begin_gtl_namespace
begin_gdb_namespace

DEMDataInfo::DEMDataInfo()
{

	rectified = 0;
	xDistance = yDistance = 0.0;
	cellSize = 0.0;

	coordinateUnit[0] = 'M';

	elevationOffset = 0.0;
	xOverlayDistance = 0.0;
	xOverlayDistance = 0.0;
	scale = 2000;
	zMax = -9999.0;
	zMin = 99999.0;

	byteNumber = 4;
	zoomFactor = 1;
	blockFlagNumber = 4;
	haveMMZ = 1;
	compress = 0x00;

	threshold = 10.0;

	projectionParameter.ellipsoid = 51;		//IDELP_50
	projectionParameter.projection = 11;		//IDCAT_GAUSSKRUGER
	projectionParameter.longitude = 117;
	projectionParameter.zoneWidth = 6;

	pyramid = -1;
	preprocess = -1;
	domResolution = -1;
	memset(reserved, -1, sizeof(char) * 8);
}

bool DEMDataInfo::operator == (DEMDataInfo &other)
{
	if (fabs(xDistance - other.xDistance) < 0.0001 && fabs(yDistance - other.yDistance) < 0.0001 && fabs(cellSize - other.cellSize) < 0.0001
		&& !strcmp(coordinateUnit, other.coordinateUnit) && fabs(elevationOffset - other.elevationOffset) < 0.0001 && fabs(xOverlayDistance - other.xOverlayDistance) < 0.0001
		&& fabs(yOverlayDistance - other.yOverlayDistance) < 0.0001
		&& byteNumber == other.byteNumber && zoomFactor == other.zoomFactor)
		return true;
	else
		return false;
}

bool DEMDataInfo::operator != (DEMDataInfo &other)
{
	if (fabs(xDistance - other.xDistance) > 0.0001 || fabs(yDistance - other.yDistance) > 0.0001 || fabs(cellSize - other.cellSize) > 0.0001
		|| strcmp(coordinateUnit, other.coordinateUnit) || fabs(elevationOffset - other.elevationOffset) > 0.0001 || fabs(xOverlayDistance - other.xOverlayDistance) > 0.0001
		|| fabs(yOverlayDistance - other.yOverlayDistance) > 0.0001
		|| byteNumber == other.byteNumber && zoomFactor == other.zoomFactor)
		return true;
	else
		return false;
}



bool DEMDataInfo::writeBuffer(Buffer& buf){
	/*size_t s = version.size();
	buf.write((void*)(&s),sizeof(size_t));
	buf.write((void*)(&version),s);*/
	buf.write(version);

	buf.write((void*)(&xDistance), sizeof(double));
	buf.write((void*)(&yDistance), sizeof(double));
	buf.write((void*)(&cellSize), sizeof(double));
	//这边要检查一下
	buf.write((void*)(&coordinateUnit), sizeof(char) * 1);

	buf.write((void*)(&elevationOffset), sizeof(float));
	buf.write((void*)(&xOverlayDistance), sizeof(double));
	buf.write((void*)(&yOverlayDistance), sizeof(double));
	buf.write((void*)(&scale), sizeof(int));
	buf.write((void*)(&zMax), sizeof(float));
	buf.write((void*)(&zMin), sizeof(float));

	buf.write((void*)(&byteNumber), sizeof(unsigned char));
	buf.write((void*)(&zoomFactor), sizeof(unsigned char));
	buf.write((void*)(&blockFlagNumber), sizeof(unsigned char));
	buf.write((void*)(&haveMMZ), sizeof(unsigned char));
	buf.write((void*)(&compress), sizeof(unsigned char));

	buf.write((void*)(&threshold), sizeof(double));

	buf.write((void*)(&projectionParameter.ellipsoid), sizeof(int));
	buf.write((void*)(&projectionParameter.projection), sizeof(int));
	buf.write((void*)(&projectionParameter.longitude), sizeof(double));
	buf.write((void*)(&projectionParameter.zoneWidth), sizeof(int));

	buf.write((void*)(&invalidData), sizeof(long));
	buf.write((void*)(&pyramid), sizeof(char));
	buf.write((void*)(&preprocess), sizeof(char));
	buf.write((void*)(&domResolution), sizeof(char));

	buf.write((void*)(reserved), sizeof(char) * 8);
	return true;
}

bool DEMDataInfo::readBuffer(Buffer& buf){
	/*size_t s = 0;
	buf.read((void*)(&s),sizeof(size_t));
	buf.read((void*)(&version),s);*/
	version = buf.readString();

	buf.read((void*)(&xDistance), sizeof(double));
	buf.read((void*)(&yDistance), sizeof(double));
	buf.read((void*)(&cellSize), sizeof(double));
	//这边要检查一下
	buf.read((void*)(&coordinateUnit), sizeof(char) * 1);

	buf.read((void*)(&elevationOffset), sizeof(float));
	buf.read((void*)(&xOverlayDistance), sizeof(double));
	buf.read((void*)(&yOverlayDistance), sizeof(double));
	buf.read((void*)(&scale), sizeof(int));
	buf.read((void*)(&zMax), sizeof(float));
	buf.read((void*)(&zMin), sizeof(float));

	buf.read((void*)(&byteNumber), sizeof(unsigned char));
	buf.read((void*)(&zoomFactor), sizeof(unsigned char));
	buf.read((void*)(&blockFlagNumber), sizeof(unsigned char));
	buf.read((void*)(&haveMMZ), sizeof(unsigned char));
	buf.read((void*)(&compress), sizeof(unsigned char));

	buf.read((void*)(&threshold), sizeof(double));

	buf.read((void*)(&projectionParameter.ellipsoid), sizeof(int));
	buf.read((void*)(&projectionParameter.projection), sizeof(int));
	buf.read((void*)(&projectionParameter.longitude), sizeof(double));
	buf.read((void*)(&projectionParameter.zoneWidth), sizeof(int));

	buf.read((void*)(&invalidData), sizeof(long));
	buf.read((void*)(&pyramid), sizeof(char));
	buf.read((void*)(&preprocess), sizeof(char));
	buf.read((void*)(&domResolution), sizeof(char));
	//这边要检查一下
	buf.read((void*)(reserved), sizeof(char) * 8);
	return true;
}


end_gdb_namespace
end_gtl_namespace

