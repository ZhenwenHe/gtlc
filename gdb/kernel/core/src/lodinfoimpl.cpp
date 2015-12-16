#include "internal_loadinfoimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
LODInfoImpl::LODInfoImpl(){
	_fid = 0;//要素对象ID
	_cur_level = 0;
}

LODInfoImpl::~LODInfoImpl(){

	// 清空infos
	_infos.clear();
}

bool LODInfoImpl::addInfo(const LOD_INFO &lodinfo, GeometrySharedPtr g){
	if (g)	{
		// 先在里面查找有没有该级别的对象，有则先删除，没有这直接插入
		for (std::vector<Info>::iterator it = _infos.begin(); it < _infos.end(); it++) {
			if (it->first.lodLevel == lodinfo.lodLevel) {

				// 再从vector中清除该位置的LOD_INFO
				_infos.erase(it);
				break;
			}
		}

		// 插入新的Lod信息
		Info info;
		info.first = lodinfo;
		info.second = g;

		// 查找时是根据LOD_PARAM中定义的级别进行的，因此此处可以随便插入
		_infos.push_back(info);

		return true;
	}
	else{
		return false;
	}
}


// 供feature使用，只将LOD的级别及每个级别的对应的参数数据写入文件:该信息随着Geometry对象走
size_t LODInfoImpl::sizeBuffer(){

	// 级别的个数
	int nLevels = getLevels();
	// 记录占用空间的长度
	size_t s = sizeof(int)//级别个数空间
		+ sizeof(LOD_INFO) * nLevels;

	return s;
}
bool LODInfoImpl::writeBuffer(Buffer& buf){

	// 级别的个数 
	int nLevels = getLevels();
	// 记录占用空间的长度
	size_t s = sizeBuffer();

	int * pi = (int *) new unsigned char[s];
	if (pi == NULL)/**若申请不到足够空间，返回false ——PY add2009.12.22*/
	{
		return false;
	}
	buf.attach(pi, s);

	*pi = (int)nLevels;//写入级别个数
	pi++;

	LOD_INFO *pInf = (LOD_INFO*)pi;
	//开始写入相关参数信息
	for (std::vector<Info>::iterator it = _infos.begin(); it != _infos.end(); it++){
		*pInf = it->first;
		pInf++;
	}

	//写入其他信息
	return true;
}

bool LODInfoImpl::readBuffer(const Buffer & buf){
	if (0 == buf.size())/**若buf为空，返回false ——PY add2009.12.22*/
	{
		return false;
	}
	int * pi = (int *)(((Buffer&)buf).data());
	int nSize = (int)*pi;//读出LOD的级别
	pi++;

	_infos.resize(nSize);//设置存储容器的个数

	LOD_INFO *pInf = (LOD_INFO*)pi;
	for (std::vector<Info>::iterator it = _infos.begin(); it != _infos.end(); it++) {
		it->first = *pInf;
		pInf++;
	}

	//读出其他信息
	return true;
}

// 将LOD的相关信息写入文件，该信息存储在LOD文件中
void LODInfoImpl::write(std::ostream & f){

	// 像写表数据一样，一行一行的写
	for (std::vector<Info>::iterator it = _infos.begin(); it != _infos.end(); it++){
		// 写入对象的ID
		f.write((char*)(&_fid), sizeof(FID));

		// 写入对象的LOD级别
		f.write((char*)(&(it->first.lodLevel)), sizeof(FID));

		// 写入空间数据
		GeometrySharedPtr pGom = it->second;

		// 如果存在，则写入
		int nHasGeom = 0;
		int nGeoType = 0;
		if (pGom){
			// 空间数据是否存在
			nHasGeom = 1;
			f.write((char*)(&(nHasGeom)), sizeof(int));
			nGeoType = pGom->getType();
			f.write((char*)(&(nGeoType)), sizeof(int));
			pGom->write(f);
		}
		else{ // 空间数据不存在
			nHasGeom = 0;
			f.write((char*)(&(nHasGeom)), sizeof(int));
		}
	}
}

// LOD的Read函数，根据需要进行读入，暂时不提供
void LODInfoImpl::read(std::istream & f){

	//	//读入关联的对象Id
	//	f.read((char*)(&_fid),sizeof(FID));
	//
	//	//读入总共的级别
	//	int nLevles = 0;
	//	f.read((char*)(&nLevles),sizeof(int));
	//	_infos.resize(nLevles);
	//
	//	// 是否拥有空间数据库
	//	int nHasGeom = 0;
	//	// 空间数据的几何类型
	//#ifndef USE_WHU_GEOMETRY
	//
	//	GEOM_TYPE nGeoType = GEOM_TYPE_UNKNOWN;
	//#else
	//	int nGeoType = 0;
	//#endif //USE_WHU_GEOMETRY
	//
	//	//写入每级对应的距离和空间数据
	//	for(std::vector<Info>::iterator it = _infos.begin();it!=_infos.end();it++){
	//		f.read((char*)(&(it->first)),sizeof(double)); //距离
	//		f.read((char*)(&(nHasGeom)),sizeof(int));
	//
	//		// 读取空间数据
	//		if (nHasGeom){			
	//			f.read((char*)(&(nGeoType)),sizeof(int));
	//#ifndef USE_WHU_GEOMETRY
	//			Geometry *pGeom = Geometry::newGeometry((GEOM_TYPE)nGeoType);
	//#else
	//			ct3d::Geometry *pGeom = ct3d::Geometry::newGeometry((int)nGeoType);
	//#endif //USE_WHU_GEOMETRY
	//
	//
	//			pGeom->read(f);
	//			it->second = pGeom;
	//		}		
	//	}	
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
