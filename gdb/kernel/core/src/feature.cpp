#include "internal_featureimpl.h"
 
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

struct FeatureDeletor{
	void operator ()(Feature* p){
		delete dynamic_cast<FeatureImpl*> (p);
	}
};

FeatureSharedPtr Feature::create() {
	return FeatureSharedPtr(new FeatureImpl(), FeatureDeletor());
}
FeatureSharedPtr Feature::create(FeatureClassSharedPtr fc, FID id1,
	std::string name, int lod, REGID regid, int state, GeometrySharedPtr pg,
	GDBTIME createTime, GDBTIME updateTime, unsigned long buffersize)
{
	return FeatureSharedPtr(
		new FeatureImpl(fc, id1, name, lod, regid, state, pg, createTime, updateTime, buffersize),
		FeatureDeletor());
}

//static FeatureSharedPtr create(FCLSID fclsid,FID id1, std::string name, int lod, REGID regid, int state=0, GeometrySharedPtr pg =GeometrySharedPtr(), 
//	GDBTIME createTime=0 ,GDBTIME updateTime=0,unsigned long buffersize = 0)
//{
//	return FeatureSharedPtr (
//		new FeatureImpl(fclsid, id1, name,  lod,  regid,  state, pg ,  createTime , updateTime,  buffersize),
//		FeatureDeletor());
//}
FeatureSharedPtr Feature::create(FCLSID fclsid, FID id1, std::string name, int lod, REGID regid, int state, GeometrySharedPtr pg,
	GDBTIME createTime, GDBTIME updateTime, unsigned long buffersize)
{
	return FeatureSharedPtr(
		new FeatureImpl(fclsid, id1, name, lod, regid, state, pg, createTime, updateTime, buffersize),
		FeatureDeletor());
}

FeatureSharedPtr Feature::create(FeatureRef f){
	return FeatureSharedPtr(
		new FeatureImpl((FeatureImpl&)f),
		FeatureDeletor());
}


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
