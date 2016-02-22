#include "geometry.h"
#include "geometry3d.h"
#include "internal_geometryimpl.h"
#include "factory.h"


begin_gtl_namespace
begin_gdb_namespace


GeometrySharedPtr Geometry::create(int signal){
#if(USING_3DGEOMETRY_TYPE==0)
	if (signal >= (int)GEOMTYPE_2D_GEOMETRY)
		return ((Geometry2dFactory*)Factory::getFactory(Factory::FACTORYTYPE_2D_GEOMETRY))->newGeometry(signal);
	else
		return ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newGeometry(signal);
#else//use the geometries in this project		
	return GeometrySharedPtr(GeometryImpl::create(signal));
#endif
}


GeometrySharedPtr Geometry::create(Buffer & buf){
#if(USING_3DGEOMETRY_TYPE==0)
	if (*((int*)buf.data()) >= (int)GEOMTYPE_3D_GEOMETRY)
		return   ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newGeometry(buf);
	else
		return ((Geometry2dFactory*)Factory::getFactory(Factory::FACTORYTYPE_2D_GEOMETRY))->newGeometry(buf);
#else//use the geometries in this project	
	return GeometrySharedPtr(GeometryImpl::create(buf));
#endif
}

bool Geometry::copy(   const GeometrySharedPtr g) {
	return copy(g.get());
}

bool Geometry::queryInterface(int signal, void ** p){

	if (signal == GEOTYPE_3D_TRIANGLEMESH)//11	
	{
		*p = (void*) dynamic_cast<TriangleMesh3d*>(this);
	}
	else if (signal == GEOTYPE_3D_POINT)//0
	{
		*p = (void*) dynamic_cast<Point3d*>(this);
	}
	else if (signal == GEOTYPE_3D_SINGLEPOINT)//0
	{
		*p = (void*) dynamic_cast<SinglePoint3d*>(this);
	}
	else if (signal == GEOTYPE_3D_MULTIPOINT)//0
	{
		*p = (void*) dynamic_cast<MultiPoint3d*>(this);
	}
	else if (signal == GEOTYPE_3D_ANNOTATION)//21
	{
		*p = (void*) dynamic_cast<Annotation3d*>(this);
	}
	else if (signal == GEOTYPE_3D_REFERENCE)//20
	{
		*p = (void*) dynamic_cast<Reference3d*>(this);
	}
	else if (signal == GEOTYPE_3D_VOLUME)//1
	{
		*p = (void*) dynamic_cast<Volume3d*>(this);
	}
	else if (signal == GEOTYPE_3D_SURFACE)//2
	{
		*p = (void*) dynamic_cast<Surface3d*>(this);
	}
	else if (signal == GEOTYPE_3D_CURVE)//3
	{
		*p = (void*) dynamic_cast<Curve3d*>(this);
	}
	else if (signal == GEOTYPE_3D_GROUP)//4
	{
		*p = (void*) dynamic_cast<Group3d*>(this);
	}
	else if (signal == GEOTYPE_3D_VOXEL)//5
	{
		*p = (void*) dynamic_cast<Voxel3d*>(this);
	}
	else if (signal == GEOTYPE_3D_SOLID)//6
	{
		*p = (void*) dynamic_cast<Solid3d*>(this);
	}
	else if (signal == GEOTYPE_3D_MULTISOLID)//7
	{
		*p = (void*) dynamic_cast<MultiSolid3d*>(this);
	}
	else if (signal == GEOTYPE_3D_PRISM)//8
	{
		*p = (void*) dynamic_cast<Prism3d*>(this);
	}
	else if (signal == GEOTYPE_3D_TETRAHEDRON)//9
	{
		*p = (void*) dynamic_cast<Tetrahedron3d*>(this);
	}
	else if (signal == GEOTYPE_3D_PARAMETERIZEDSURFACE)//10
	{
		*p = (void*) dynamic_cast<ParameterizedSurface3d*>(this);
	}
	else if (signal == GEOTYPE_3D_TRIANGLE)//12
	{
		*p = (void*) dynamic_cast<Triangle3d*>(this);
	}
	else if (signal == GEOTYPE_3D_MULTIPOLYGON)//13
	{
		*p = (void*) dynamic_cast<MultiPolygon3d*>(this);
	}
	else if (signal == GEOTYPE_3D_POLYGON)//14
	{
		*p = (void*) dynamic_cast<Polygon3d*>(this);
	}
	else if (signal == GEOTYPE_3D_LINEARRING)//15
	{
		*p = (void*) dynamic_cast<LinearRing3d*>(this);
	}
	else if (signal == GEOTYPE_3D_PARAMETERIZEDCURVE)//16
	{
		*p = (void*) dynamic_cast<ParameterizedCurve3d*>(this);
	}
	else if (signal == GEOTYPE_3D_LINESTRING)//17
	{
		*p = (void*) dynamic_cast<LineString3d*>(this);
	}
	else if (signal == GEOTYPE_3D_MULTIPOLYLINE)//18
	{
		*p = (void*) dynamic_cast<MultiPolygon3d*>(this);
	}
	else
	{
		*p = 0;
	}
	if (*p == 0)
		return true;
	else
		return true;
}
end_gdb_namespace
end_gtl_namespace


