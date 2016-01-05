#include "internal_featureclassimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
FeatureClassImpl::FeatureClassImpl(){
	_id = Identifier::generate();
	_name = "";
	_code = "";
	_srefid = 0;
	//_gTab="";  
	//_aTab="";  
	_partitionnum = 0;
	_fclstype = 0;
}
FeatureClassImpl::FeatureClassImpl(std::string name, std::string code, int  geomtype, SREFID srefid, std::string gTab, std::string aTab, int partitionnum){
	_id = Identifier::generate();
	_name = name;
	_code = code;
	//_geom=geomtype;
	_srefid = srefid;
	/*_gTab=gTab;
	_aTab=aTab;  */
	_partitionnum = partitionnum;
	_fclstype = 0;
}
FeatureClassImpl::FeatureClassImpl(FCLSID id, std::string name, std::string code, int geomtype, SREFID srefid, std::string gTab, std::string aTab, int partitionnum) {
	_id = id;
	_name = name;
	_code = code;
	//_geom=geomtype;
	_srefid = srefid;
	//_gTab=gTab;  
	//_aTab=aTab;  
	_partitionnum = partitionnum;
	_fclstype = 0;
}
FeatureClassImpl::FeatureClassImpl(const FeatureClassImpl & fc){
	copy(fc);
}
void FeatureClassImpl::copy(const FeatureClassImpl & fc){
	_id = fc._id;
	_name = fc._name;
	_code = fc._code;
	//_geom=fc._geom;
	_srefid = fc._srefid;
	/*_gTab=fc._gTab;
	_aTab=fc._aTab;  */
	_partitionnum = fc._partitionnum;
	_proptypes.resize(fc._proptypes.size());
	std::copy(fc._proptypes.begin(), fc._proptypes.end(), _proptypes.begin());
	_localfeatureset = fc._localfeatureset;
	_fclstype = fc._fclstype;
}
FeatureClassImpl FeatureClassImpl::operator=(const FeatureClassImpl & fc){
	copy(fc);
	return *this;
}
void FeatureClassImpl::addPropType(const PropType & pt){
	_proptypes.insert(_proptypes.end(), pt);
}
void FeatureClassImpl::addPropType(std::string name, std::string code, std::string type, int length, int decimal, bool nullable){
	_proptypes.insert(_proptypes.end(), PropType(name, code, type, length, decimal, nullable));
}

void FeatureClassImpl::removePropType(std::string name){
	for (std::vector<PropType>::iterator it = _proptypes.begin(); it<_proptypes.end(); it++){
		if (name == it->getName()){
			_proptypes.erase(it);
			break;
		}
	}
}

bool FeatureClassImpl::operator==(const FeatureClassImpl & fc)
{
	bool bRet = false;
	// 判断要素类描述信息是否相等
	if (_name == fc._name && _code == fc._code /*&& _geom == fc._geom*/ && _srefid == fc._srefid){
		bRet = true;
	}
	else{
		return false;
	}

	// 判断要素类属性字段是否相等
	if (_proptypes.size() != fc._proptypes.size())
	{
		return false;
	}

	size_t nPos = 0;
	bool bVal = true;
	for (std::vector<PropType>::iterator it = _proptypes.begin(); it<_proptypes.end(); it++)
	{
		if ((*it) == fc._proptypes[nPos]){
			bVal = true;
		}
		else{
			return false;
		}

		nPos++;
	}

	// 返回
	return (bRet && bVal);
}


void FeatureClassImpl::write(std::ostream & f){


	f.write((char*)(&_id), sizeof(FCLSID));
	int s = _name.size();
	f.write((char*)(&s), sizeof(int));
	f.write((char*)(_name.c_str()), s);
	s = _code.size();
	f.write((char*)(&s), sizeof(int));
	f.write((char*)(_code.c_str()), s);
	//删除要素类的类型  [sunka, 2010-3-1]
	//f.write((char*)(&_geom),sizeof(int));
	f.write((char*)(&_srefid), sizeof(SREFID));
	//删除两个表  [sunka, 2010-3-1]
	/*s = _gTab.size();
	f.write((char*)(&s),sizeof(int));
	f.write((char*)(_gTab.c_str()),s);
	s = _aTab.size();
	f.write((char*)(&s),sizeof(int));
	f.write((char*)(_aTab.c_str()),s);*/

	f.write((char*)(&_partitionnum), sizeof(int));

	s = _proptypes.size();
	f.write((char*)(&s), sizeof(int));

	for (std::vector<PropType>::iterator it = _proptypes.begin(); it != _proptypes.end(); it++){
		it->write(f);
	}

	//要素类型所在的工程
	f.write((char*)(&_prjID), sizeof(PRJID));

	//标识要素类型为地上还是地下类型
	f.write((char*)(&_fclstype), sizeof(int));

	//要素类型对应要素集合管理类型
	f.write((char*)(&_fsettype), sizeof(int));

	//要素类型对应集合数据表管理类型
	f.write((char*)(&_fsetproptype), sizeof(int));
}
void FeatureClassImpl::read(std::istream & f){

	f.read((char*)(&_id), sizeof(FCLSID));
	int s = 0;
	char ca[255];
	f.read((char*)(&s), sizeof(int));
	f.read((char*)(ca), s);
	_name.assign(ca, ca + s);

	f.read((char*)(&s), sizeof(int));
	f.read((char*)(ca), s);
	_code.assign(ca, ca + s);
	//删除要素类的类型  [sunka, 2010-3-1]
	//f.read((char*)(&_geom),sizeof(int));
	f.read((char*)(&_srefid), sizeof(SREFID));

	//删除_gTab  [sunka, 2010-3-1]
	//f.read((char*)(&s),sizeof(int));
	//f.read((char*)(ca),s);
	//_gTab.assign(ca,ca+s);

	//f.read((char*)(&s),sizeof(int));
	//f.read((char*)(ca),s);
	//_aTab.assign(ca,ca+s);

	f.read((char*)(&_partitionnum), sizeof(int));


	f.read((char*)(&s), sizeof(int));
	_proptypes.resize(s);
	for (std::vector<PropType>::iterator it = _proptypes.begin(); it != _proptypes.end(); it++){
		it->read(f);
	}

	f.read((char*)(&_prjID), sizeof(PRJID));	//要素类型所在的工程

	f.read((char*)(&_fclstype), sizeof(int));//标识要素类型为地上还是地下类型

	f.read((char*)(&_fsettype), sizeof(int));//要素类型对应要素集合管理类型

	f.read((char*)(&_fsetproptype), sizeof(int));//要素类型对应集合数据表管理类型
}

size_t FeatureClassImpl::sizeBuffer()
{
	size_t s = sizeof(FCLSID) //要素类型id
		+ 4              //要素类型名称占位
		+ _name.size()   //要素类型名称
		+ 4              //要素类型编码占位
		+ _code.size()   //要素类型编码
		//删除要素类的类型  [sunka, 2010-3-1]
		//+ sizeof(int)    //要素类型对应的几何类型
		+ sizeof(SREFID) //要素类型对应空间参考系统id
		//+ 4              //要素类型的几何表管理表名称占位
		//+ _gTab.size()   //要素类型的几何表管理表名称
		//+ 4              //要素类型的属性表管理表名称占位
		//+ _aTab.size()   //要素类型的属性表管理表名称
		+sizeof(int)    //要素类型对应的要素集合表的分区个数
		+ sizeof(PRJID)  //要素类型所在的工程
		+ sizeof(int)    //标识要素类型为地上还是地下类型
		+ sizeof(int)    //要素类型对应要素集合管理类型
		+ sizeof(int);   //要素类型对应集合数据表管理类型
	return s;

}
void FeatureClassImpl::writeBuffer(Buffer &buf)
{
	size_t s = sizeBuffer();
	size_t len = 0;
	char *pi = (char *) new unsigned char[s];
	buf.attach(pi, s);
	char *ptr = pi;

	*(FCLSID*)ptr = _id;
	ptr += sizeof(FCLSID);

	*(int *)ptr = _name.size();
	ptr += 4;
	memcpy(ptr, _name.c_str(), _name.size());
	ptr += _name.size();

	*(int *)ptr = _code.size();
	ptr += 4;
	memcpy(ptr, _code.c_str(), _code.size());
	ptr += _code.size();

	//删除要素类的类型  [sunka, 2010-3-1]
	/**(int *)ptr = _geom;
	ptr += 4;*/

	*(SREFID*)ptr = _srefid;
	ptr += sizeof(SREFID);

	//删除_gTab  [sunka, 2010-3-1]
	/**(int *)ptr = _gTab.size();
	ptr += 4;
	memcpy(ptr, _gTab.c_str(), _gTab.size());
	ptr += _gTab.size();

	*(int *)ptr = _aTab.size();
	ptr += 4;
	memcpy(ptr, _aTab.c_str(), _aTab.size());
	ptr += _aTab.size();*/

	*(int *)ptr = _partitionnum;
	ptr += 4;

	*(PRJID*)ptr = _prjID;
	ptr += sizeof(PRJID);

	*(int *)ptr = _fclstype;
	ptr += 4;

	*(int*)ptr = _fsettype;
	ptr += 4;

	*(int*)ptr = _fsetproptype;
	ptr += 4;

}

void FeatureClassImpl::readBuffer(Buffer &buf)
{
	int lenName, lenCode;
	char name[255], code[255];
	buf.read((char*)&_id, sizeof(FCLSID));

	buf.read((char*)&lenName, sizeof(int));
	buf.read((char*)name, lenName);
	_name.assign(name, name + lenName);

	buf.read((char*)&lenCode, sizeof(int));
	buf.read((char*)code, lenCode);
	_code.assign(code, code + lenCode);
	//删除要素类的类型  [sunka, 2010-3-1]
	//buf.read((char*)&_geom, sizeof(int));
	buf.read((char*)&_srefid, sizeof(SREFID));

	//删除gTab  [sunka, 2010-3-1]
	/*buf.read((char*)&lenGTab, sizeof(int));
	buf.read((char*)gTab, lenGTab);
	_gTab.assign(gTab, gTab+lenGTab);

	buf.read((char*)&lenATab, sizeof(int));
	buf.read((char*)aTab, lenATab);
	_aTab.assign(aTab, aTab+lenATab);*/

	buf.read((char*)&_partitionnum, sizeof(int));
	buf.read((char*)&_prjID, sizeof(PRJID));
	buf.read((char*)&_fclstype, sizeof(int));
	buf.read((char*)&_fsettype, sizeof(int));
	buf.read((char*)&_fsetproptype, sizeof(int));
}

//要素类型编号
FCLSID FeatureClassImpl::getID(){ return _id; }
void  FeatureClassImpl::setID(FCLSID id){ _id = id; }

//要素类型名称
std::string FeatureClassImpl::getName(){ return _name; }
void  FeatureClassImpl::setName(std::string v){ _name = v; }

//要素类型编码
std::string FeatureClassImpl::getCode(){ return _code; }
void  FeatureClassImpl::setCode(std::string v){ _code = v; }

//空间参考系统ID
void  FeatureClassImpl::setSpatialReferenceID(SREFID srefID){ _srefid = srefID; }
SREFID  FeatureClassImpl::getSpatialReferenceID(){ return _srefid; }

//几何表的分区个数
void  FeatureClassImpl::setPartitionNumber(int v){ _partitionnum = v; }
int  FeatureClassImpl::getPartitionNumber(){ return _partitionnum; }

//对应的属性字段集合
std::vector<PropType> & FeatureClassImpl::getPropTypes(){ return _proptypes; }

//要素类型所在的工程
PRJID FeatureClassImpl::getPrjID(){ return _prjID; }
void  FeatureClassImpl::setPrjID(PRJID prjID){ _prjID = prjID; }

//设置要素类的状态
int FeatureClassImpl::getFclsType(){ return _fclstype; }
void  FeatureClassImpl::setFclsType(int fclstype){ _fclstype = fclstype; }
/**获取要素类型的要素集合管理类型
* @param [in]
* @return  void
*/
int FeatureClassImpl::getFsetType() { return _fsettype; }

/**设置要素类型的要素集合管理类型
* @param [in] v  要素类型的要素集合管理类型
* @return  void
*/
void FeatureClassImpl::setFsetType(int v) { _fsettype = v; }

/**获取要素类型的属性字段建表类型
* @return
*/
int FeatureClassImpl::getFsetPropType() { return _fsetproptype; }

/**设置要素类型属性字段建表类型
* @param [in] v		属性字段建表类型
* @return
*/
void FeatureClassImpl::setFsetPropType(int v) { _fsetproptype = v; }
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
