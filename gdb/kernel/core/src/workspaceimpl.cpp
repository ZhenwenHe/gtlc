#include "internal_workspaceimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 
 


WorkspaceImpl::WorkspaceImpl(WKSPID id, std::string name, std::string code, double * bound, int level, SREFID sid){
	_regid = id;
	_name = name;
	_code = code;
	_level = level;
	_srefid = sid;
	if (bound)
		memcpy(_bound, bound, sizeof(double) * 6);
	else
		memset(_bound, 0, sizeof(double) * 6);

	Vertex3dVector::value_type v1, v2, v3, v4;
	v1.x = _bound[0]; v1.y = _bound[1]; v1.z = 0;
	v2.x = _bound[3]; v2.y = _bound[1]; v2.z = 0;
	v3.x = _bound[3]; v3.y = _bound[4]; v3.z = 0;
	v4.x = _bound[0]; v4.y = _bound[4]; v4.z = 0;
	_polybound.push_back(v1);
	_polybound.push_back(v2);
	_polybound.push_back(v3);
	_polybound.push_back(v4);

	_cur_index = -1;
}

void WorkspaceImpl::clear(){
	_set.clear();
}
WorkspaceImpl::WorkspaceImpl(){
	_regid = Identifier::generate();
	_name = numberToString<char, unsigned long long >(_regid);
	_code = "WK";
	_level = 0;
	_srefid = 0;
	memset(_bound, 0, sizeof(double) * 6);
	_cur_index = -1;
}
WorkspaceImpl::~WorkspaceImpl(){
	clear();
}
void WorkspaceImpl::copy(const WorkspaceImpl & ws){
	_regid = ws._regid;
	_name = ws._name;
	_code = ws._code;
	_level = ws._level;
	_srefid = ws._srefid;
	if (ws._bound)
	{
		memcpy(_bound, ws._bound, sizeof(double) * 6);
	}
	else
	{
		memset(_bound, 0, sizeof(double) * 6);
	}
	_set = ws._set;
	_cur_index = ws._cur_index;
	_localproject = ws._localproject;


	// 索引不再属于工作区 [2009-8-19 liuyuanyuan del]
	/*_rtree.copy((SpatialIndex*)(&(ws._rtree)));*/
}
WorkspaceImpl::WorkspaceImpl(const WorkspaceImpl & ws){
	copy(ws);
}
WorkspaceImpl WorkspaceImpl::operator=(const WorkspaceImpl &){
	return *this;
}
void WorkspaceImpl::addLayer(LayerSharedPtr p){
	p->setLocalWorkspace(shared_from_this());
	_set.push_back(p);
}
void WorkspaceImpl::setCurLayer(std::string layername){

}

void WorkspaceImpl::setCurLayer(size_t s){

}

LayerSharedPtr WorkspaceImpl::operator() (size_t s){
	return _set[s];
}

const LayerRef WorkspaceImpl::operator() (size_t s) const {
	return *(_set[s]);
}

bool WorkspaceImpl::operator==(const WorkspaceImpl & ws){
	if (
		//_regid = ws._regid;
		_name != ws._name ||
		_code != ws._code ||
		//_level = ws._level;
		_srefid != ws._srefid)
	{
		return false;
	}
	// 判断包围盒是否都重叠
	if (_bound && ws._bound)
	{
		for (size_t i = 0; i < 6; i++)
		{
			if (fabs(_bound[i] - ws._bound[i]) > 0.000001)
			{
				return false;
			}
		}
	}

	// 判断里面的图层是否一致
	if (_set.size() != ws.size()){
		return false;
	}
	size_t nPos = 0;
	LayerSharedPtr p;
	for (std::vector<LayerSharedPtr>::iterator it = _set.begin(); it<_set.end(); it++){
		if ((*it)->isEqual(p))
		{
		}
		else
		{
			return false;
		}
	}

	return true;
}

void WorkspaceImpl::unifyID() {//将本工作区内所有对象的ID统一设置为当前工作区的ID
	for (vector<LayerSharedPtr>::iterator itLay = _set.begin(); itLay != _set.end(); itLay++) {
		vector<FeatureSetSharedPtr> & fSets = (*itLay)->getFeatureSetVector();
		for (vector<FeatureSetSharedPtr>::iterator itFset = fSets.begin(); itFset != fSets.end(); itFset++) {
			vector<FeatureSharedPtr> features = (*itFset)->getFeatures();
			for (vector<FeatureSharedPtr>::iterator itFeature = features.begin(); itFeature != features.end(); itFeature++) {
				(*itFeature)->setWorkspaceID(_regid);
			}
		}
	}
}
FeatureSetSharedPtr WorkspaceImpl::findFeatureSet(FCLSID fclsid){//根据要素类型ID，获取指定的要素集合
	for (vector<LayerSharedPtr>::iterator itLay = _set.begin(); itLay != _set.end(); itLay++) {
		vector<FeatureSetSharedPtr> & fSets = (*itLay)->getFeatureSetVector();
		for (vector<FeatureSetSharedPtr>::iterator itFset = fSets.begin(); itFset != fSets.end(); itFset++) {
			if ((*itFset)->getFeatureClass()->getID() == fclsid)
				return *itFset;
		}
	}
	return FeatureSetSharedPtr();
}
void WorkspaceImpl::write(std::ostream & f)
{
	f.write((char*)(&_regid), sizeof(REGID));

	int s = _name.size();
	f.write((char*)(&s), sizeof(int));
	f.write((char*)(_name.c_str()), s);

	s = _code.size();
	f.write((char*)(&s), sizeof(int));
	f.write((char*)(_code.c_str()), s);

	f.write((char*)(&_bound), sizeof(double) * 6);


	s = _polybound.size();
	f.write((char*)(&s), sizeof(int));
	for (Vertex3dVector::iterator it = _polybound.begin(); it != _polybound.end(); it++){
		f.write((char*)(&(it->x)), sizeof(double));
		f.write((char*)(&(it->y)), sizeof(double));
		f.write((char*)(&(it->z)), sizeof(double));
	}

	f.write((char*)(&_srefid), sizeof(SREFID));

	f.write((char*)(&_level), sizeof(int));
	// 索引不再属于工作区 [2009-8-19 liuyuanyuan del]
	/*_rtree.write(f);*/

	s = _set.size();
	f.write((char*)(&s), sizeof(int));
	for (std::vector<LayerSharedPtr>::iterator it = _set.begin(); it != _set.end(); it++){
		(*it)->write(f);
	}

	f.write((char*)(&_cur_index), sizeof(int));
}
void WorkspaceImpl::read(std::istream & f)
{
	int s = 0;
	char ca[255];

	f.read((char*)(&_regid), sizeof(REGID));

	f.read((char*)(&s), sizeof(int));
	f.read((char*)(ca), s);
	_name.assign(ca, ca + s);

	f.read((char*)(&s), sizeof(int));
	f.read((char*)(ca), s);
	_code.assign(ca, ca + s);

	f.read((char*)(&_bound), sizeof(double) * 6);

	// _polybound;//多边形区域
	f.read((char*)(&s), sizeof(int));
	_polybound.resize(s);
	for (Vertex3dVector::iterator it = _polybound.begin(); it != _polybound.end(); it++){
		f.read((char*)(&(it->x)), sizeof(double));
		f.read((char*)(&(it->y)), sizeof(double));
		f.read((char*)(&(it->z)), sizeof(double));
	}

	f.read((char*)(&_srefid), sizeof(SREFID));

	f.read((char*)(&_level), sizeof(int));
	// 索引不再属于工作区 [2009-8-19 liuyuanyuan del]
	/*_rtree.read(f);*/

	f.read((char*)(&s), sizeof(int));
	//_set.resize(s);
	for (int index = 0; index<s; index++)
	{
		LayerSharedPtr pLayer = Layer::create();
		pLayer->read(f);
		addLayer(pLayer);
	}

	f.read((char*)(&_cur_index), sizeof(int));
}

void WorkspaceImpl::load(std::string & sz)
{
	std::locale loc = std::locale::global(std::locale(""));//设置环境为系统环境
	std::ifstream iFile;
	iFile.open(sz.c_str(), std::ios::in | std::ios::binary);
	if (!iFile){
		//std::cerr << "error:unable to open input file:" << iFile <<endl;
		return;
	}

	read(iFile);
	iFile.close();
	std::locale::global(loc);

}
void WorkspaceImpl::save(std::string & sz)
{
	std::locale loc = std::locale::global(std::locale(""));//设置环境为系统环境
	std::ofstream oFile;
	oFile.open(sz.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
	write(oFile);
	oFile.close();
	std::locale::global(loc);
}
 
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
