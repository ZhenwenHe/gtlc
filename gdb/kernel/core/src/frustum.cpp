#include "frustum.h"
#include "internal_frustumimpl.h"
#include "factory.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

///////////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<Frustum> Frustum::create(){
#if (USING_DEFAULT_3DGEOMETRY==1)
	FrustumImpl* p = new FrustumImpl();
	return FrustumSharedPtr( p);
#else
	FrustumSharedPtr p = ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newFrustum(); 
	return p;
#endif
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
