#include "config.h"
#include "terrain.h" 
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

DEMInfo::DEMInfo()
{
	x0 = 0.0;
	y0 = 0.0;
	cellSize = 0.0;
	minZ = 0.0;
	maxZ = 0.0;
	columnNumber = 0;
	rowNumber = 0;
}


DEMInfo::DEMInfo(double x0, double y0, double	cellSize, double minZ, double maxZ, int column, int row)
{
	this->x0 = x0;
	this->y0 = y0;
	this->cellSize = cellSize;
	this->minZ = minZ;
	this->maxZ = maxZ;
	this->columnNumber = column;
	this->rowNumber = row;
}


bool DEMInfo::writeBuffer(Buffer& buf){

	buf.write((void*)(&x0), sizeof(double));
	buf.write((void*)(&y0), sizeof(double));
	buf.write((void*)(&cellSize), sizeof(double));
	buf.write((void*)(&minZ), sizeof(double));
	buf.write((void*)(&maxZ), sizeof(double));
	buf.write((void*)(&columnNumber), sizeof(int));
	buf.write((void*)(&rowNumber), sizeof(int));

	return true;
}

bool DEMInfo::readBuffer(Buffer& buf){
	buf.read((void*)(&x0), sizeof(double));
	buf.read((void*)(&y0), sizeof(double));
	buf.read((void*)(&cellSize), sizeof(double));
	buf.read((void*)(&minZ), sizeof(double));
	buf.read((void*)(&maxZ), sizeof(double));
	buf.read((void*)(&columnNumber), sizeof(int));
	buf.read((void*)(&rowNumber), sizeof(int));

	return true;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace