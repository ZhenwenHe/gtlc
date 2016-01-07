#include "sharedmodel.h"
#include "internal_sharedmodelimpl.h"
#include "factory.h"


begin_gtl_namespace
begin_gdb_namespace

////////////////////////////////////////////////////////////////
SharedModelSharedPtr SharedModel::create(){
#if(USING_3DGEOMETRY_TYPE==0)
	return  ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newSharedModel();
#else
	return SharedModelSharedPtr(new SharedModelImpl());
#endif

}
SharedModelSharedPtr SharedModel::create(Identifier::raw_type d, std::string name, int type) {
#if(USING_3DGEOMETRY_TYPE==0)
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
end_gtl_namespace

 