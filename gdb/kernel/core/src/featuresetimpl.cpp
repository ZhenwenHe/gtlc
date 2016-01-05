#include "internal_featuresetimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

static  FeatureSetImpl::DatasetType g_DSType = FeatureSetImpl::SINGLE_FILE_DB;

FeatureSetImpl::DatasetType FeatureSetImpl::getDatasetType() //获取数据集类型 
{
	return g_DSType;
}
void  FeatureSetImpl::setDatasetType(FeatureSetImpl::DatasetType dst) //设置数据集类型
{
	g_DSType = dst;
}

FeatureSetImpl::FeatureSetImpl(){
	_fsid = Identifier::generate();
	_localLayer = NULL;
}
FeatureSetImpl::FeatureSetImpl(const FeatureClass & fcls){
	_fsid = Identifier::generate();
	_fclass = ((FeatureClass &)fcls).clone();
	//_fclass->setLocaFeatureSet(this->shared_from_this());
	_localLayer = NULL;
}
FeatureSetImpl::FeatureSetImpl(FeatureClassSharedPtr fcls){
	_fsid = Identifier::generate();
	_fclass = fcls;
	//_fclass->setLocaFeatureSet(this->shared_from_this());
	_localLayer = NULL;
}

void FeatureSetImpl::copy(const FeatureSetSharedPtr fSet, bool bContent){

	_fsid = fSet->getID();//要素集合ID
	_name = fSet->getName();//要素集合名称
	//要素对象对应的要素类

	_fclass = fSet->getFeatureClass();
	//_fclass->setLocaFeatureSet(std::shared_ptr<FeatureSetImpl>(this)/*this->shared_from_this()*/);

	FeatureSharedPtrVector & fs = fSet->getFeatures();
	//要素
	if (bContent)
	{
		_fset.resize(fs.size());
		std::copy(fs.begin(), fs.end(), _fset.begin());
	}

	_localLayer = fSet->getLayerID();//要素集合所在的图层
}
void FeatureSetImpl::copy(const FeatureSetRef fSet, bool bContent){

	_fsid = fSet.getID();//要素集合ID
	_name = fSet.getName();//要素集合名称
	//要素对象对应的要素类

	_fclass = fSet.getFeatureClass();
	//_fclass->setLocaFeatureSet(std::shared_ptr<FeatureSetImpl>(this)/*this->shared_from_this()*/);

	FeatureSharedPtrVector & fs = fSet.getFeatures();
	//要素
	if (bContent)
	{
		_fset.resize(fs.size());
		std::copy(fs.begin(), fs.end(), _fset.begin());
	}

	_localLayer = fSet.getLayerID();//要素集合所在的图层
}
void FeatureSetImpl::clear(){
	_fset.clear();
}
FeatureSetImpl::~FeatureSetImpl(){
	clear();
	_localLayer = NULL;
}


Feature & FeatureSetImpl::operator() (size_t s){
	return *(_fset[s]);
}
const Feature & FeatureSetImpl::operator()  (size_t s) const{
	return *(_fset[s]);
}


void FeatureSetImpl::write(std::ostream & f) {

	f.write((char*)(&_fsid), sizeof(FSID));

	int s = _name.size();
	f.write((char*)(&s), sizeof(int));
	f.write((char*)(_name.c_str()), s);



	_fclass->write(f);

	if (getDatasetType() == FeatureSetImpl::MULTI_FILE_DB)
		return;

	s = _fset.size();
	f.write((char*)(&s), sizeof(int));
	for (FeatureSharedPtrVector::iterator it = _fset.begin(); it != _fset.end(); it++){
		(*it)->write(f);
	}
}
void FeatureSetImpl::read(std::istream & f){
	int s = 0;
	char ca[255];

	f.read((char*)(&_fsid), sizeof(FSID));

	f.read((char*)(&s), sizeof(int));
	f.read((char*)(ca), s);
	_name.assign(ca, ca + s);



	_fclass->read(f);

	if (getDatasetType() == FeatureSetImpl::MULTI_FILE_DB)
		return;


	f.read((char*)(&s), sizeof(int));
	for (int index = 0; index < s; index++)
	{
		createFeature(0, "Feature", 0, 0)->read(f);
	}
}

//设置要素集合对应的要素类
void FeatureSetImpl::setFeatureClass(FeatureClassSharedPtr  f){
	_fclass = f;
	_fclass->setLocalFeatureSet(this->shared_from_this());
}
void FeatureSetImpl::attachFeatureClass(FeatureClass* f){
	_fclass = FeatureClassSharedPtr(f);
	_fclass->setLocalFeatureSet(shared_from_this());
}

//获取要素集合对应的要素类
FeatureClassSharedPtr  FeatureSetImpl::getFeatureClass(){
	return _fclass;
}

//添加要素对象
void FeatureSetImpl::addFeature(FeatureSharedPtr p){ _fset.push_back(p); }
/**在要素集合中删除指定要素ID的要素数据
* @param [in] fid  要素ID
* @return
*/
void FeatureSetImpl::eraseFeature(FID fid){

	for (FeatureSharedPtrVector::iterator it = _fset.begin(); it < _fset.end(); it++) {
		if ((*it)->getID() == fid) {
			_fset.erase(it);
			return;
		}
	}
}

FeatureSharedPtr FeatureSetImpl::removeFeature(FID fid){
	FeatureSharedPtr p;

	for (FeatureSharedPtrVector::iterator it = _fset.begin(); it < _fset.end(); it++) {
		if ((*it)->getID() == fid) {
			p = *it;
			_fset.erase(it);
			return p;
		}
	}
	return p;
}

FeatureSharedPtr FeatureSetImpl::createFeature(FID id, std::string name, int lod, REGID regid,
	int state /*= 0*/, GeometrySharedPtr pg/* = GeometrySharedPtr()*/, GDBTIME createTime/* = 0*/, GDBTIME updateTime/* = 0*/){

	FeatureSharedPtr f = Feature::create(_fclass, id, name, lod, regid, state, pg,/*pt,*/createTime, updateTime);
	f->setFeatureClass(_fclass);
	_fset.push_back(f);
	return f;
}
FeatureSharedPtr FeatureSetImpl::createFeature(std::string name, int lod, REGID regid,
	int state/* = 0*/, GeometrySharedPtr pg/* = GeometrySharedPtr()*/, GDBTIME createTime /*= 0*/, GDBTIME updateTime/* = 0*/){
	return createFeature(Identifier::generate(), name, lod, regid, state, pg,createTime, updateTime);
}

FeatureSharedPtr FeatureSetImpl::createFeature(FeatureRef f){
	FeatureSharedPtr pf = Feature::create(f);
	pf->setFeatureClass(_fclass);
	_fset.push_back(pf);
	return pf;
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
