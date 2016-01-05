#include "selector.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

Selector::Selector(void):FeatureWeakPtrVector(){
}

Selector::~Selector(void){
	BaseType::clear();
}
FeatureSharedPtr Selector::operator() (size_t i) {
	if(i>=0 && i<BaseType::size())
		return BaseType::at(i).lock();
	else
		return FeatureSharedPtr();
}
size_t Selector::size(){
	return BaseType::size();
}
FeatureSharedPtr Selector::getAt (size_t i) {
	if(i>=0 && i<BaseType::size())
		return BaseType::at(i).lock();
	else
		return FeatureSharedPtr();
}
void Selector::append(FeatureSharedPtr p){
	if(exist(p))
		return;
	p->setStatus(p->getStatus()| 0x00000004);
	BaseType::push_back(FeatureWeakPtr(p));
}

void Selector::append(FeatureSharedPtrVector p) {
	for(FeatureSharedPtrVector::iterator it = p.begin();it!=p.end();it++)
		append(*it);
}

FeatureSharedPtr Selector::removeAt(size_t i){
	FeatureWeakPtr p =BaseType::at(i);
	BaseType::erase(BaseType::begin()+i);
	FeatureSharedPtr sp =  p.lock();
	sp->setStatus(sp->getStatus() & 0xffffffb);
	return sp;
}

void Selector::clear( ){
	if(empty())
		return;
	FeatureSharedPtr sp;
	for(BaseType::iterator it = BaseType::begin();it!=BaseType::end();it++){
		if(!it->expired()){
			sp = it->lock();;
			sp->setStatus(sp->getStatus() & 0xffffffb);
		}	
	}		
	BaseType::clear();
}
bool Selector::empty(){
	return BaseType::empty();
}
FeatureSharedPtr Selector::findFeature(FID fid){
	FeatureSharedPtr p;
	for(BaseType::iterator it = BaseType::begin();it!=BaseType::end();it++){
		if(!it->expired()){
			p = it->lock();;
			if(fid ==p->getID())
				return p;
		}	
	}		
	return p;
}

FeatureSharedPtr Selector::removeFeature(FID fid){
	FeatureSharedPtr p;
	for(BaseType::iterator it = BaseType::begin();it!=BaseType::end();it++){
		if(!it->expired()){
			p = it->lock();;
			if(fid ==p->getID()){
				BaseType::erase(it);
				return p;
			}
		}	
	}		
	return FeatureSharedPtr();
}

long Selector::getFeatures(FCLSID fclsid,FeatureSharedPtrVector & vv){
	vv.clear();
	FeatureSharedPtr p;
	for(BaseType::iterator it = BaseType::begin();it!=BaseType::end();it++){
		if(!it->expired()){
			p = it->lock();;
			if(fclsid ==p->getFeatureClassID())
				vv.push_back(p);
		}	
		else{
			BaseType::erase(it);
		}
	}
	return  vv.size();
}

long Selector::getFeatures(FeatureSharedPtrVector & vv){
	vv.clear();
	FeatureSharedPtr p;
	for(BaseType::iterator it = BaseType::begin();it!=BaseType::end();it++){
		if(!it->expired()){
			p = it->lock();
			vv.push_back(p);
		}	
		else{
			BaseType::erase(it);
		}
	}
	return  vv.size();
}

bool Selector::exist(FID  fid){
	FeatureSharedPtr p;
	for(BaseType::iterator it = BaseType::begin();it!=BaseType::end();it++){
		if(!it->expired()){
			p = it->lock();;
			if(p->getID()==fid)
				return true;
		}	
		else{
			BaseType::erase(it);
		}
	}
	return  false;
}


bool Selector::exist(FeatureSharedPtr  pp){
	return exist(pp->getID());
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
