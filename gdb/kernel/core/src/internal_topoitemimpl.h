#pragma once
#include "topology.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


typedef std::pair<FID, FCLSID> FIDITEM;//要素ID和要素类ID的pair

//A - B
class TopoItemImpl : public  TopoItem{
public:
	int nType;
	FIDITEM fidItemMain;//A
	FIDITEM fidItemSub;//B

	
	TopoItemImpl();
	TopoItemImpl(int topoType, FID fidMain, FCLSID fclsidMain, FID fidSub, FCLSID fclsidSub);
public:
	virtual int getType() const { return nType; }
	virtual void setType(int t) { nType = t; }


	virtual std::pair<FID, FCLSID>  getAItem(){
		return fidItemMain;
	}
	virtual std::pair<FID, FCLSID>  getBItem(){
		return fidItemSub;
	}


	virtual void setAItem(std::pair<FID, FCLSID> & mainPair){
		fidItemMain = mainPair;
	}
	virtual void setBItem(std::pair<FID, FCLSID> & subPair){
		fidItemSub = subPair;
	}

	virtual Identifier::raw_type getAFeatureClassID() const {
		return fidItemMain.second;
	}
	virtual Identifier::raw_type getBFeatureClassID() const	{
		return fidItemSub.second;
	}
	virtual Identifier::raw_type getAFeatureID() const	{
		return fidItemMain.first;
	}
	virtual Identifier::raw_type getBFeatureID() const	{
		return fidItemSub.first;
	}

	virtual void setAFeatureClassID(Identifier::raw_type d){
		fidItemMain.second = d;
	}
	virtual void setBFeatureClassID(Identifier::raw_type d){
		fidItemSub.second = d;
	}

	virtual void setAFeatureID(Identifier::raw_type d){
		fidItemMain.first = d;
	}
	virtual void setBFeatureID(Identifier::raw_type d){
		fidItemSub.first = d;
	}

	virtual bool copy(const TopoItem & tt){
		TopoItemImpl & t = (TopoItemImpl&)tt;
		fidItemMain = t.fidItemMain;
		fidItemSub = t.fidItemSub;
		return true;
	}
	virtual bool isEqual(const TopoItem & tt){
		TopoItemImpl & t = (TopoItemImpl&)tt;
		if (fidItemMain.first == t.fidItemMain.first
			&&
			fidItemMain.second == t.fidItemMain.second
			&&
			fidItemSub.first == t.fidItemSub.first
			&&
			fidItemSub.second == t.fidItemSub.second
			&&
			nType == t.nType)
			return true;
		else
			return false;
	}
};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
