#include "config.h"
#include "terrain.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

DEMDBInfo::DEMDBInfo()
{
	demdbID = 0;
	/*strcpy_s(demdbName,MAX_NAME,"");
	strcpy_s(demdbPath,MAX_PATH,"");*/
	//layerNumber = 0;
	lowestResolution = 0.0;
	highestResolution = 0.0;

	srefID = 0;
	createTime = 0;
	updateTime = 0;
	////////
	demDataInfo = DEMDataInfoSharedPtr(new DEMDataInfo());
	demLayerInfos.push_back(DEMLayerInfoSharedPtr(new DEMLayerInfo()));
}

bool DEMDBInfo::operator == (DEMDBInfo &other)
{
	if (demdbID == other.demdbID && !strcmp(demdbName.c_str(), other.demdbName.c_str())
		&& !strcmp(demdbPath.c_str(), other.demdbPath.c_str()) && demLayerInfos.size() != demLayerInfos.size() && fabs(lowestResolution - other.lowestResolution) < 0.0001
		&& fabs(highestResolution - other.highestResolution) < 0.0001 && srefID == other.srefID  && createTime == other.createTime)
		return true;
	else
		return false;
}

bool DEMDBInfo::operator != (DEMDBInfo &other)
{
	if (demdbID != other.demdbID || strcmp(demdbName.c_str(), other.demdbName.c_str())
		|| strcmp(demdbPath.c_str(), other.demdbPath.c_str()) || demLayerInfos.size() != demLayerInfos.size() || fabs(lowestResolution - other.lowestResolution) > 0.0001
		|| fabs(highestResolution - other.highestResolution) > 0.0001 || srefID != other.srefID || createTime != other.createTime)
		return true;
	else
		return false;
}


DEMDBInfo DEMDBInfo::operator=(const DEMDBInfo &other)
{
	demdbID = other.demdbID;
	/*strcpy_s(demdbName,MAX_NAME,other.demdbName);
	strcpy_s(demdbPath,MAX_PATH,other.demdbPath);*/
	demdbPath = other.demdbPath;
	demdbName = other.demdbName;

	//layerNumber = other.layerNumber;
	lowestResolution = other.lowestResolution;
	highestResolution = other.highestResolution;
	demdbScope = other.demdbScope;

	demDataInfo = DEMDataInfoSharedPtr(new DEMDataInfo(*other.demDataInfo.get()));
	demLayerInfos.clear();
	for (int i = 0; i<other.demLayerInfos.size(); i++){
		DEMLayerInfoSharedPtr sp(new DEMLayerInfo(*other.demLayerInfos[i].get()));
		demLayerInfos.push_back(sp);
	}
	srefID = other.srefID;
	createTime = other.createTime;
	updateTime = other.updateTime;
	return *this;
}


void DEMDBInfo::clear()
{
	demdbID = 0;
	/*strcpy_s(demdbName,MAX_NAME,"");
	strcpy_s(demdbPath,MAX_PATH,"");*/
	demdbName.clear();
	demdbPath.clear();
	//layerNumber = 0;
	lowestResolution = 0.0;
	highestResolution = 0.0;

	srefID = 0;
	createTime = 0;
	updateTime = 0;

	demLayerInfos.clear();
	demDataInfo = DEMDataInfoSharedPtr();

}

bool DEMDBInfo::writeBuffer(Buffer& buf){

	buf.write((void*)(&demdbID), sizeof(Identifier::raw_type));
	//这个得到长度不知是否正确-------需要检查
	/*size_t s =  demdbName.size();
	buf.write((void*)(&s),sizeof(size_t));
	buf.write((void*)(&demdbName),s);*/
	buf.write(demdbName);
	/*s = demdbPath.size();
	buf.write((void*)(&s),sizeof(size_t));
	buf.write((void*)(&demdbPath),s);*/
	buf.write(demdbPath);

	double v[4];
	demdbScope.get(v);
	buf.write((void*)v, sizeof(double) * 4);

	buf.write((void*)(&lowestResolution), sizeof(double));
	buf.write((void*)(&highestResolution), sizeof(double));

	demDataInfo->writeBuffer(buf);

	unsigned int _demLayerNumber = demLayerInfos.size();
	buf.write((void*)(&_demLayerNumber), sizeof(unsigned int));
	for (unsigned int i = 0; i<_demLayerNumber; i++)
	{
		demLayerInfos[i]->writeBuffer(buf);
	}

	buf.write((void*)(&srefID), sizeof(Identifier::raw_type));
	buf.write((void*)(&createTime), sizeof(Identifier::raw_type));
	buf.write((void*)(&updateTime), sizeof(Identifier::raw_type));

	return true;
}

bool DEMDBInfo::readBuffer(Buffer& buf){
	buf.read((void*)(&demdbID), sizeof(Identifier::raw_type));
	//这个得到长度不知是否正确-------需要检查
	/*size_t s = 0;
	buf.read((void*)(&s),sizeof(size_t));
	buf.read((void*)(&demdbName),s);*/
	demdbName = buf.readString();
	/*s = 0;
	buf.read((void*)(&s),sizeof(size_t));
	buf.read((void*)(&demdbPath),s);*/
	demdbPath = buf.readString();

	double v[4];
	buf.read((void*)v, sizeof(double) * 4);
	demdbScope.set(v);

	buf.read((void*)(&lowestResolution), sizeof(double));
	buf.read((void*)(&highestResolution), sizeof(double));

	demDataInfo->readBuffer(buf);

	unsigned int  _demLayerNumber = 0;
	buf.read((void*)(&_demLayerNumber), sizeof(unsigned int));

	//先清空默认的demlayerInfos
	demLayerInfos.clear();

	for (unsigned int i = 0; i<_demLayerNumber; i++)
	{
		DEMLayerInfoSharedPtr _tmpDemisp(new DEMLayerInfo());
		_tmpDemisp->readBuffer(buf);
		demLayerInfos.push_back(_tmpDemisp);
	}

	buf.read((void*)(&srefID), sizeof(Identifier::raw_type));
	buf.read((void*)(&createTime), sizeof(Identifier::raw_type));
	buf.read((void*)(&updateTime), sizeof(Identifier::raw_type));

	return true;
}


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace