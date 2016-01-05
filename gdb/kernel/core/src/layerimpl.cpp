#include "internal_layerimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 
LayerImpl::LayerImpl(){
	_layid = Identifier::generate();
	_cur_index = -1;
	_name = numberToString<char, unsigned long long>(_layid);

}

void LayerImpl::clear(){
	_set.clear();
}

LayerImpl::~LayerImpl(){
	clear();
}

LayerImpl::LayerImpl(/*std::string name,*/LYRID layid){
	_cur_index = -1;
	_layid = layid;
	_name = numberToString<char, LYRID>(layid);
}
void LayerImpl::copy(const LayerRef ly){
	const LayerImpl & lyr = (const LayerImpl &)ly;
	_layid = lyr._layid;
	_name = lyr._name;
	_set = lyr._set;
	_cur_index = lyr._cur_index;
	_localworkspace = lyr._localworkspace;
}
LayerImpl::LayerImpl(const LayerImpl & lyr){
	copy((const LayerRef)lyr);
}
LayerImpl LayerImpl::operator=(const LayerImpl &){
	return *this;
}

bool LayerImpl::operator ==(const LayerImpl& lay){
	if (_name != lay._name) {
		return false;
	}

	if (size() != lay.size()) {
		return false;
	}

	// 此时没有考虑要素类在图层中的顺序，存在问题 [2009-7-6 sunka add notes]
	/*size_t nPos = 0;
	for (std::vector<FeatureSetSharedPtr>::iterator it = _set.begin(); it<_set.end(); it++){
	FeatureSetSharedPtr fset =   lay(nPos);
	if ((*it)->getFClass()->getID() == fset->getFClass()->getID()) {

	}
	else
	{
	return false;
	}
	nPos ++;
	}*/

	return true;
}

size_t LayerImpl::indexFeatureSet(FSID id){
	for (size_t i = 0; i<_set.size(); i++){
		if (_set[i]->getID() == id)
			return i;
	}
	return -1;
}
FeatureSetSharedPtr LayerImpl::createFeatureSet(const FeatureClassRef fc)
{
	FeatureSetSharedPtr p = FeatureSet::create(fc);
	p->setLayerID(_layid);
	_set.push_back(p);
	if (_cur_index<0)
		_cur_index = _set.size() - 1;
	return p;
}

FeatureSetSharedPtr LayerImpl::addFeatureSet(FeatureSetSharedPtr fs){
	fs->setLayerID(_layid);
	_set.push_back(fs);
	return fs;
}


FeatureSetSharedPtr LayerImpl::operator() (size_t s){
	return _set[s];
}
const FeatureSet& LayerImpl::operator() (size_t s) const  {
	return  *(_set[s]);
}

void LayerImpl::eraseFeatureClassID(FCLSID fclsid)
{
	vector<FCLSID>::iterator iter;
	for (iter = m_fclsids.begin(); iter != m_fclsids.end();)
	{
		FCLSID id = *iter;
		if (fclsid == id)
		{
			m_fclsids.erase(iter);
			break;
		}
		else
			iter++;
	}

}

void LayerImpl::eraseFeatureSet(FSID fsid)
{
	vector<FeatureSetSharedPtr>::iterator iter;
	for (iter = _set.begin(); iter != _set.end();)
	{
		if ((*iter)->getID() == fsid)
		{
			_set.erase(iter);
			break;
		}
		else
			iter++;
	}
}

void LayerImpl::write(std::ostream & f)
{
	f.write((char*)(&_layid), sizeof(LYRID));

	int s = _name.size();
	f.write((char*)(&s), sizeof(int));
	f.write((char*)(_name.c_str()), s);

	s = _set.size();
	f.write((char*)(&s), sizeof(int));
	for (std::vector<FeatureSetSharedPtr>::iterator it = _set.begin(); it != _set.end(); it++){
		(*it)->write(f);
	}

	f.write((char*)(&_cur_index), sizeof(int));
}
void LayerImpl::read(std::istream & f)
{
	int s = 0;
	char ca[255];

	f.read((char*)(&_layid), sizeof(LYRID));

	f.read((char*)(&s), sizeof(int));
	f.read((char*)(ca), s);
	_name.assign(ca, ca + s);

	f.read((char*)(&s), sizeof(int));
	//_set.resize(s);
	for (int index = 0; index<s; index++)
	{
		//#ifndef USE_WHU_GEOMETRY
		//		FeatureClass  *pfclass = new FeatureClass("FEATURE","CODE",GEOM_TYPE_UNKNOWN);
		//#else
		//FeatureClass  *pfclass = new FeatureClass("FEATURE","CODE",ct3d::GEOTYPE_3D_GEOMETRY);
		//#endif //USE_WHU_GEOMETRY

		FeatureSetSharedPtr pFSet = FeatureSet::create(FeatureClass::create("FEATURE", "CODE"));
		pFSet->read(f);
		addFeatureSet(pFSet);
	}

	f.read((char*)(&_cur_index), sizeof(int));
}
void LayerImpl::load(std::string & sz)
{
	std::locale loc = std::locale::global(std::locale(""));//设置环境为系统环境
	std::ifstream iFile;
	iFile.open(sz.c_str(), std::ios::in | std::ios::binary);
	if (!iFile){
		//cerr << "error:unable to open input file:" << iFile <<endl;
		return;
	}

	read(iFile);
	iFile.close();
	std::locale::global(loc);

}
void LayerImpl::save(std::string & sz)
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
