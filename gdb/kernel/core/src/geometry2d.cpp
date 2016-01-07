#include "geometry.h"
#include "geometry2d.h"
#include "geometry3d.h"
#include "factory.h"



begin_gtl_namespace
begin_gdb_namespace

SymbolManager2dSharedPtr sPointSymbolManager;
SymbolManager2dSharedPtr sLineSymbolManager;
SymbolManager2dSharedPtr sRegionSymbolManager;

SymbolManager2dSharedPtr& SymbolManager2d::getPointSymbolManager()
{
	if (sPointSymbolManager.get() == NULL)
	{
		Geometry2dFactory * pFactoryGeom2d = (Geometry2dFactory *) 
			Factory::getFactory(Factory::FACTORYTYPE_2D_GEOMETRY);
		sPointSymbolManager = pFactoryGeom2d->newSymbolManager(SYMBOL_MANAGER_TYPE_2D_POINT);
	}
	return sPointSymbolManager;
}

SymbolManager2dSharedPtr& SymbolManager2d::getLineSymbolManager()
{
	if (sLineSymbolManager.get() == NULL)
	{
		Geometry2dFactory * pFactoryGeom2d = (Geometry2dFactory *) 
			Factory::getFactory(Factory::FACTORYTYPE_2D_GEOMETRY);
		sLineSymbolManager = pFactoryGeom2d->newSymbolManager(SYMBOL_MANAGER_TYPE_2D_LINE);
	}
	return sLineSymbolManager;
}

SymbolManager2dSharedPtr& SymbolManager2d::getRegionSymbolManager()
{
	if (sRegionSymbolManager.get() == NULL)
	{
		Geometry2dFactory * pFactoryGeom2d = (Geometry2dFactory *) 
			Factory::getFactory(Factory::FACTORYTYPE_2D_GEOMETRY);
		sRegionSymbolManager = pFactoryGeom2d->newSymbolManager(SYMBOL_MANAGER_TYPE_2D_REGION);
	}
	return sRegionSymbolManager;
}


Element2dSharedPtr Element2d::create(int f)
{
	return ((Geometry2dFactory*)Factory::getFactory(Factory::FACTORYTYPE_2D_GEOMETRY))->newElement(f);
}

end_gdb_namespace
end_gtl_namespace


