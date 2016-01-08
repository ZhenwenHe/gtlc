#include "config.h"
#include "terrain.h" 
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


DEMBlock::DEMBlock()
{
	blockID = 0;
	layerID = 0;
	row = 0;
	column = 0;
	full = 1;
	pointNumber = 0;
	maxZ = -99999.0;
	minZ = 99999.0;
	valid = 0;
	rawData = NULL;
}


void DEMBlock::clear()
{
	valid = 0;
	SAFE_DELETE(rawData);
}

DEMBlock::~DEMBlock(){
	clear();
}


bool DEMBlock::writeBuffer(Buffer& buf){
	buf.write((void*)(&blockID), sizeof(Identifier::raw_type));
	buf.write((void*)(&layerID), sizeof(Identifier::raw_type));
	buf.write((void*)(&row), sizeof(int));
	buf.write((void*)(&column), sizeof(int));
	buf.write((void*)(&full), sizeof(int));

	double v[4];
	scope.get(v);
	buf.write((void*)v, sizeof(double) * 4);

	buf.write((void*)(&pointNumber), sizeof(int));
	buf.write((void*)(&maxZ), sizeof(float));
	buf.write((void*)(&minZ), sizeof(float));
	buf.write((void*)(&valid), sizeof(int));
	//这里需要检查rawData
	buf.write((void*)(rawData), sizeof(float)*pointNumber);

	return true;
}

bool DEMBlock::readBuffer(Buffer& buf){
	buf.read((void*)(&blockID), sizeof(Identifier::raw_type));
	buf.read((void*)(&layerID), sizeof(Identifier::raw_type));
	buf.read((void*)(&row), sizeof(int));
	buf.read((void*)(&column), sizeof(int));
	buf.read((void*)(&full), sizeof(int));

	double v[4];
	buf.read((void*)v, sizeof(double) * 4);
	scope.set(v);

	buf.read((void*)(&pointNumber), sizeof(int));
	buf.read((void*)(&maxZ), sizeof(float));
	buf.read((void*)(&minZ), sizeof(float));
	buf.read((void*)(&valid), sizeof(int));
	//这里需要检查rawData
	if (rawData == 0)
		rawData = new float[pointNumber];
	buf.read((void*)(rawData), sizeof(float)*pointNumber);
	return true;
}


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace