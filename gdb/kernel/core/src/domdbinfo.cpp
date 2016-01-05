#include "config.h"
#include "terrain.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

//////////////////////////////////////////////////////////
DOMDBInfo::DOMDBInfo()
{
	domdbID = 0;
	demdbID = 0;
	/*strcpy_s(domdbName,MAX_NAME,"");
	strcpy_s(domdbPath,MAX_PATH,"");*/
	//layerNum = 0;
	lowestResolution = 0.0;
	highestResolution = 0.0;

	domDataInfo = DOMDataInfoSharedPtr(new DOMDataInfo());

	DOMLayerInfoSharedPtr sp(new DOMLayerInfo());
	domLayerInfos.push_back(sp);


	srefID = 0;
	createTime = 0;
	updateTime = 0;

}


DOMDBInfo DOMDBInfo::operator=(const DOMDBInfo &other)
{
	domdbID = other.domdbID;
	demdbID = other.demdbID;
	domdbName = other.domdbName;
	domdbPath = other.domdbPath;
	//layerNum = other.layerNum;
	lowestResolution = other.lowestResolution;
	highestResolution = other.highestResolution;
	domdbScope = other.domdbScope;

	domDataInfo = DOMDataInfoSharedPtr(new DOMDataInfo(*other.domDataInfo.get()));
	domLayerInfos.clear();
	for (int i = 0; i<other.domLayerInfos.size(); i++){
		DOMLayerInfoSharedPtr sp(new DOMLayerInfo(*other.domLayerInfos[i].get()));
		domLayerInfos.push_back(sp);
	}
	srefID = other.srefID;
	createTime = other.createTime;
	updateTime = other.updateTime;

	return *this;
}

void DOMDBInfo::clear()
{
	domdbID = 0;
	demdbID = 0;
	/*strcpy_s(domdbName,MAX_NAME,"");
	strcpy_s(domdbPath,MAX_PATH,"");*/
	domdbName.clear();
	domdbPath.clear();
	domdbScope = Envelope2d();

	domDataInfo = DOMDataInfoSharedPtr();

	/*for (int i = 0; i< layerNum; i++)
	{
	domLayerInfo[i].clear();
	}*/

	domLayerInfos.clear();

	//layerNum = 0;
	lowestResolution = 0.0;
	highestResolution = 0.0;

	srefID = 0;
	createTime = 0;
	updateTime = 0;
}

bool DOMDBInfo::writeBuffer(Buffer& buf){
	buf.write((void*)(&domdbID), sizeof(Identifier::raw_type));
	buf.write((void*)(&demdbID), sizeof(Identifier::raw_type));
	//这个得到长度不知是否正确-------需要检查
	/*size_t s = domdbName.size();
	buf.write((void*)(&s),sizeof(size_t));
	buf.write((void*)(&domdbName),s);*/
	buf.write(domdbName);

	/*s = domdbPath.size();
	buf.write((void*)(&s),sizeof(size_t));
	buf.write((void*)(&domdbPath),s);*/
	buf.write(domdbPath);

	double v[4];
	domdbScope.get(v);
	buf.write((void*)v, sizeof(double) * 4);

	buf.write((void*)(&lowestResolution), sizeof(double));
	buf.write((void*)(&highestResolution), sizeof(double));

	domDataInfo->writeBuffer(buf);

	unsigned int domLayerNumber = domLayerInfos.size();
	buf.write((void*)(&domLayerNumber), sizeof(unsigned int));
	for (unsigned int i = 0; i<domLayerNumber; i++)
	{
		domLayerInfos[i]->writeBuffer(buf);
	}

	buf.write((void*)(&srefID), sizeof(Identifier::raw_type));
	buf.write((void*)(&createTime), sizeof(Identifier::raw_type));
	buf.write((void*)(&updateTime), sizeof(Identifier::raw_type));

	return true;
}

bool DOMDBInfo::readBuffer(Buffer& buf){
	buf.read((void*)(&domdbID), sizeof(Identifier::raw_type));
	buf.read((void*)(&demdbID), sizeof(Identifier::raw_type));
	//这个得到长度不知是否正确-------需要检查
	/*size_t s =0;
	buf.read((void*)(&s),sizeof(size_t));
	buf.read((void*)(&domdbName),s);*/
	domdbName = buf.readString();
	/*s = 0;
	buf.read((void*)(&s),sizeof(size_t));
	buf.read((void*)(&domdbPath),s);*/
	domdbPath = buf.readString();

	double v[4];
	buf.read((void*)v, sizeof(double) * 4);
	domdbScope.set(v);

	buf.read((void*)(&lowestResolution), sizeof(double));
	buf.read((void*)(&highestResolution), sizeof(double));

	domDataInfo->readBuffer(buf);

	unsigned int domLayerNumber = 0;
	buf.read((void*)(&domLayerNumber), sizeof(unsigned int));

	//先清空默认的demlayerInfos
	domLayerInfos.clear();

	for (unsigned int i = 0; i<domLayerNumber; i++)
	{
		DOMLayerInfoSharedPtr _tmpDomisp(new DOMLayerInfo());
		_tmpDomisp->readBuffer(buf);
		domLayerInfos.push_back(_tmpDomisp);
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