#include "material.h"
#include "factory.h"
#include "internal_complexmaterialimpl.h"
#include "internal_simplematerialimpl.h"


begin_gtl_namespace
begin_gdb_namespace

///////////////////////////////////////////////////////////////
MaterialSharedPtr Material::create(int type){
#if (USING_3DGEOMETRY_TYPE==0)
	if (type == 0)
		return  ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newSimpleMaterial();
	else
		return  ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newComplexMaterial();
#else
	if (type == 0)
		return  MaterialSharedPtr(new SimpleMaterialImpl());
	else
		return  MaterialSharedPtr(new ComplexMaterialImpl());
#endif
	
}

end_gdb_namespace
end_gtl_namespace



