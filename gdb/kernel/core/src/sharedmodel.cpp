#include "sharedmodel.h"
#include "internal_SharedModelImpl.h"
#include "factory.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

////////////////////////////////////////////////////////////////
SharedModelSharedPtr SharedModel::create(){
#if(USING_DEFAULT_3DGEOMETRY==0)
	return  ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newSharedModel();
#else
	return SharedModelSharedPtr(new SharedModelImpl());
#endif

}
SharedModelSharedPtr SharedModel::create(Identifier::raw_type d, std::string name, int type) {
#if(USING_DEFAULT_3DGEOMETRY==0)
	SharedModelSharedPtr p = ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newSharedModel();
#else
	SharedModelSharedPtr  p(new SharedModelImpl());
#endif
	p->setID(d);
	p->setName(name);
	p->setType(type);
	return p;
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace 