#include "internal_projectimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

std::string g_version = "1.0.0.0";

ProjectImpl::ProjectImpl(){
	_prjid = Identifier::generate();
	_name = numberToString<char, unsigned long long >(_prjid);

	//memset(_bound,0,sizeof(double)*6);
	_bound[0] = _bound[2] = _bound[4] = INITMIN;
	_bound[1] = _bound[3] = _bound[5] = INITMAX;

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
	// 索引不再属于工程 [2009-8-19 liuyuanyuan del]
	/*_indextree = NULL;*/
}

ProjectImpl::ProjectImpl(PRJID id, std::string name, double * bound){
	_prjid = id;
	_name = name;
	if (bound)
		memcpy(_bound, bound, sizeof(double) * 6);
	else
	{
		_bound[0] = _bound[2] = _bound[4] = INITMIN;
		_bound[1] = _bound[3] = _bound[5] = INITMAX;
	}

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
	// 索引不再属于工程 [2009-8-19 liuyuanyuan del]
	/*_indextree = NULL;*/
}
void ProjectImpl::copy(const ProjectImpl & p){
	_prjid = p._prjid;
	_name = p._name;
	_polybound = p._polybound; //工程的多边形边界 
	_regs = p._regs;//分区集合
	_cur_index = p._cur_index;//当前分区索引，默认操作在当前区

	if (p._bound)
		memcpy(_bound, p._bound, sizeof(double) * 6);
	else
	{
		_bound[0] = _bound[2] = _bound[4] = INITMIN;
		_bound[1] = _bound[3] = _bound[5] = INITMAX;
	}

	// 索引不再属于工程 [2009-8-19 liuyuanyuan del]
	/*if (p._indextree)
	{
	INDEX_TYPE type = p._indextree->getType();
	_indextree = getIdxPtr1(type);
	if(_indextree!=NULL){
	_indextree->copy(_indextree);
	}
	}
	else if(_indextree){
	delete _indextree;
	_indextree = NULL;
	}*/
}
ProjectImpl::ProjectImpl(const ProjectImpl & p){
	copy(p);
}
ProjectImpl ProjectImpl::operator=(const ProjectImpl &){
	return *this;
}

Workspace & ProjectImpl::operator() (size_t s){
	return *(_regs[s]);
}
const Workspace & ProjectImpl::operator() (size_t s) const {
	return *(_regs[s]);
}
void ProjectImpl::addWorkspace(WorkspaceSharedPtr p){
	p->setLocalProject(this->shared_from_this());
	_regs.push_back(p);
}
void ProjectImpl::clear(){

	_regs.clear();
	// 索引不再属于工程 [2009-8-19 liuyuanyuan del]
	/*if (_indextree!=NULL){
	delete _indextree;
	_indextree = NULL;
	}*/
}
WorkspaceSharedPtr  ProjectImpl::getCurWorkspace(){
	if (_regs.empty())	{
		WorkspaceSharedPtr p = Workspace::create();
		_regs.push_back(p);
		_cur_index = 0;
		p->setLocalProject(shared_from_this());
	}

	return _regs[_cur_index];
}
ProjectImpl::~ProjectImpl(){
	clear();
}
void ProjectImpl::write(std::ostream & f)
{
	////写入加密标识
	//bool bencryptted = false;
	//f.write((char*)(&bencryptted),sizeof(bool));

	//写入全局的要素类型信息
	std::map<FCLSID, FeatureClassSharedPtr> & tmap = FeatureClass::getFeatureClassMap();
	int s = tmap.size();
	f.write((char*)(&s), sizeof(int));
	for (std::map<FCLSID, FeatureClassSharedPtr>::iterator it = tmap.begin(); it != tmap.end(); it++){
		it->second->write(f);
	}

	f.write((char*)(&_prjid), sizeof(PRJID));

	s = _name.size();
	f.write((char*)(&s), sizeof(int));
	f.write((char*)(_name.c_str()), s);

	f.write((char*)(&_bound), sizeof(double) * 6);

	//Vertex3dList  _polybound;//多边形区域
	s = _polybound.size();
	f.write((char*)(&s), sizeof(int));


	for (Vertex3dVector::iterator it = _polybound.begin(); it != _polybound.end(); it++){
		f.write((char*)(&(it->x)), sizeof(double));
		f.write((char*)(&(it->y)), sizeof(double));
		f.write((char*)(&(it->z)), sizeof(double));
	}

	// 索引不再属于工程 [2009-8-19 liuyuanyuan del]
	/*INDEX_TYPE indextype = INDEX_TYPE_UNKNOWN;
	if (_indextree!=NULL){
	indextype = _indextree->getType();
	f.write((char*)(&indextype),sizeof(int));
	_indextree->write(f);
	}
	else{
	f.write((char*)(&indextype),sizeof(int));
	}*/

	s = _regs.size();
	f.write((char*)(&s), sizeof(int));
	for (std::vector<WorkspaceSharedPtr>::iterator it = _regs.begin(); it != _regs.end(); it++){
		(*it)->write(f);
	}

	f.write((char*)(&_cur_index), sizeof(int));
}
int ProjectImpl::read(std::istream & f)
{
	////读出加密标识
	//bool bencryptted = false;
	//f.read((char*)(&bencryptted),sizeof(bool));

	//if (bencryptted)
	//{
	//	return 1;
	//}

	try
	{
		int s = 0;
		f.read((char*)(&s), sizeof(int));
		if (s>0){
			std::map<FCLSID, FeatureClassSharedPtr> & tmap = FeatureClass::getFeatureClassMap();
			tmap.clear();
			for (int i = 0; i<s; i++){
				FeatureClassSharedPtr fs = FeatureClass::create();
				fs->read(f);
				tmap.insert(std::pair<FCLSID, FeatureClassSharedPtr>(fs->getID(), fs));
			}
		}

		char ca[255];
		f.read((char*)(&_prjid), sizeof(PRJID));

		f.read((char*)(&s), sizeof(int));
		f.read((char*)(ca), s);
		_name.assign(ca, ca + s);

		f.read((char*)(&_bound), sizeof(double) * 6);


		f.read((char*)(&s), sizeof(int));
		_polybound.resize(s);

		for (Vertex3dVector::iterator it = _polybound.begin(); it != _polybound.end(); it++){
			f.read((char*)(&(it->x)), sizeof(double));
			f.read((char*)(&(it->y)), sizeof(double));
			f.read((char*)(&(it->z)), sizeof(double));
		}


		// 索引不再属于工程 [2009-8-19 liuyuanyuan del]
		/*INDEX_TYPE indextype;
		f.read((char*)(&indextype),sizeof(int));
		_indextree = getIdxPtr1(indextype);
		if (_indextree!=NULL)
		{
		_indextree->read(f);
		}*/


		f.read((char*)(&s), sizeof(int));
		//_regs.resize(s);
		for (int index = 0; index<s; index++)
		{
			WorkspaceSharedPtr pWorkspace = Workspace::create();
			pWorkspace->read(f);
			addWorkspace(pWorkspace);
		}

		f.read((char*)(&_cur_index), sizeof(int));
	}
	catch (std::exception* ex)
	{
		std::cout << "Error running demo : " << ex->what() << std::endl;
		return 2;
	}

	return 0;
}
void ProjectImpl::load(std::string & sz)
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
void ProjectImpl::save(std::string & sz)
{
	std::locale loc = std::locale::global(std::locale(""));//设置环境为系统环境
	std::ofstream oFile;
	oFile.open(sz.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
	write(oFile);
	oFile.close();
	std::locale::global(loc);
}

PRJID ProjectImpl::getID()const { return _prjid; }
void  ProjectImpl::setID(PRJID id){ _prjid = id; }

std::string ProjectImpl::getName() { return _name; }
void  ProjectImpl::setName(std::string v){ _name = v; }

//inline double * getBound() {return (double *) _bound;}
void	ProjectImpl::getBound(Envelope3d & vl) {
	vl.minx = _bound[0]; vl.maxx = _bound[1];
	vl.miny = _bound[2]; vl.maxy = _bound[3];
	vl.minz = _bound[4]; vl.maxz = _bound[5];
}
void ProjectImpl::setBound(Envelope3d & vl) {
	_bound[0] = vl.minx; _bound[1] = vl.maxx;
	_bound[2] = vl.miny; _bound[3] = vl.maxy;
	_bound[4] = vl.minz; _bound[5] = vl.maxz;
}
void ProjectImpl::setBound(double d[6]){
	memcpy((void*)_bound, d, sizeof(double) * 6);
}
void ProjectImpl::getBound(double * pv) {
	memcpy((void*)pv, _bound, sizeof(double) * 6);
}
std::vector<WorkspaceSharedPtr> & ProjectImpl::getWorkspaces(){ return _regs; }
Vertex3dVector & ProjectImpl::getPolyBound() { return _polybound; }

/** 设置工程的多边形边界
* @param [in] plybound 工程多边形边界
* @return
*/
void ProjectImpl::setPolyBound(Vertex3dVector & plybound) {
	_polybound.resize(plybound.size());
	std::copy(plybound.begin(), plybound.end(), _polybound.begin());
}


DDBProjectImpl::DDBProjectImpl(ProjectAdapterSharedPtr _projadapt)
{
	m_projadapt = _projadapt;

}

DDBProjectImpl::DDBProjectImpl(PRJID id, std::string name, double * bound)
{

}
void DDBProjectImpl::copy(const DDBProjectImpl &)
{

}
DDBProjectImpl::DDBProjectImpl(const DDBProjectImpl &)
{

}
DDBProjectImpl  DDBProjectImpl::operator=(const DDBProjectImpl &impl)
{
	//impl

	return *this;
}

DDBProjectImpl::~DDBProjectImpl()
{
	//if(m_projadapt)
	//	delete m_projadapt;
	//m_projadapt = NULL;
}

void DDBProjectImpl::setProjectAdapter(ProjectAdapterSharedPtr prjadapt)
{

	m_projadapt = prjadapt;
}

void DDBProjectImpl::load(std::string & sz)
{
	if (m_projadapt)
	{
		m_projadapt->load(sz);
	}

}
void DDBProjectImpl::save(std::string & sz)
{
	if (m_projadapt)
	{
		m_projadapt->save(sz);
	}

}



end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
