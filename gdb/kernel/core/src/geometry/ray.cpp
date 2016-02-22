#include "internal_rayimpl.h"
#include "factory.h"


begin_gtl_namespace
begin_gdb_namespace
std::shared_ptr<Ray> Ray::create(const Vertex3d& o, const Vertex3d& d){
#if(USING_3DGEOMETRY_TYPE==1)
	RayImpl * r = new RayImpl(o, d);
	return RaySharedPtr(r);
#else
	RaySharedPtr p = ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newRay(o, d);
	p->set(o, d);
	return p;
#endif
}
/////////////////////////////////////////////////////////////////////////////////////////
end_gdb_namespace
end_gtl_namespace


