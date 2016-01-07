#include "frustum.h"
#include "internal_frustumimpl.h"
#include "factory.h"


begin_gtl_namespace
begin_gdb_namespace

///////////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<Frustum> Frustum::create(){
#if (USING_3DGEOMETRY_TYPE==1)
	FrustumImpl* p = new FrustumImpl();
	return FrustumSharedPtr( p);
#else
	FrustumSharedPtr p = ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newFrustum(); 
	return p;
#endif
}
end_gdb_namespace
end_gtl_namespace


