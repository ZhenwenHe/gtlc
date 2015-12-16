#include "TemporarySet.h"


begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

TempSet::TempSet(){
	_status = 0;	
	_drawMode=0;
	_color=0;
	_alpha=0;
}

FeatureVectorSharedPtr TempSet::getFeatures(FCLSID fclsid) {
	FSMAP::iterator it = _set.find(fclsid);
	if(it!=_set.end())
		return it->second;
	else
		return FeatureVectorSharedPtr();
}

FeatureVectorSharedPtr TempSet::removeFeatures(const FCLSID &fclsid){
	FeatureVectorSharedPtr p;
	FSMAP::iterator it = _set.find(fclsid);
	if(it!=_set.end()){
		p = it->second;
		_set.erase(it);
	}
	return p;
}

void TempSet::addFeature(FeatureSharedPtr fsp) {
	FSMAP::iterator it = _set.find(fsp->getFeatureClassID());
	if(it!=_set.end())
		it->second->push_back(fsp);
	else{		
		FeatureVectorSharedPtr p(new std::vector<FeatureSharedPtr>());
		_set.insert( FSPAIR( fsp->getFeatureClassID(),p));
		_set[fsp->getFeatureClassID()]->push_back(fsp);
	}
}

FeatureSharedPtr TempSet::removeFeature(const FCLSID &fclsid,const FID &fid){
	FeatureSharedPtr p;
	FSMAP::iterator it = _set.find(fclsid);
	if(it!=_set.end()){
		for (vector<FeatureSharedPtr>::iterator itF = it->second->begin(); itF != it->second->end(); ++itF){
			if ((*itF)->getID() == fid){
				p = *itF;
				it->second->erase(itF);
				break;
			}
		}
	}
	return p;
}

void TempSet::clear( ) {
	_set.clear();
}

TempSet::~TempSet( ) {
	_set.clear();
}

void TempSet::getAllFclsid(vector<FCLSID> &fclsids){
	for (FSMAP::iterator it = _set.begin(); it != _set.end(); ++it){
		fclsids.push_back(it->first);
	}
}
////////////////////////////////////////////////////////////////////////////////////////
TemporarySets::TemporarySets(void)
{
}

TemporarySets::~TemporarySets(void)
{
	clear();
}

TempSetSharedPtr TemporarySets::newTempSet(const std::string &strSetName){
	std::list<TempSetSharedPtr>::iterator it = _data.begin();
	for (;it != _data.end(); ++it){
		if (strSetName == (*it)->getName()){			
			return TempSetSharedPtr();
		}
	}
	TempSetSharedPtr pTempset(new TempSet);
	pTempset->setName(strSetName);
	_data.push_back(pTempset);//将新创建的指针管理起来
	return pTempset;
}

TempSetSharedPtr TemporarySets::findTempSet(const std::string &strSetName){
	std::list<TempSetSharedPtr>::iterator it = _data.begin();
	for (;it != _data.end(); ++it){
		if (strSetName == (*it)->getName()){			
			return *it;
		}
	}
	return TempSetSharedPtr ();
}

TempSetSharedPtr TemporarySets::getTempSetAt(const size_t &i){
	if (i<_data.size())
	{
		std::list<TempSetSharedPtr>::iterator it = _data.begin();
		for (size_t j = 0;j < i; ++j)++it;
		return *it;
	}
	return TempSetSharedPtr();
}
TempSetSharedPtr TemporarySets::operator () (size_t i){
	if (i<_data.size())
	{
		std::list<TempSetSharedPtr>::iterator it = _data.begin();
		for (size_t j = 0;j < i; ++j)++it;
		return *it;
	}
	return TempSetSharedPtr();
}

TempSetSharedPtr TemporarySets::removeTempSet(const std::string &strSetName){	
	TempSetSharedPtr p;
	for (std::list<TempSetSharedPtr>::iterator it = _data.begin();it != _data.end();it++){
		if (strSetName == (*it)->getName()){
			p = *it;		
			_data.erase(it);			
			return p;
		}
	}
	return p;
}

void TemporarySets::clear(){
	/*std::list<TempSet*>::iterator it = _data.begin();
	for (;it != _data.end(); ++it){
		delete *it;
	}*/		
	_data.clear();
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
