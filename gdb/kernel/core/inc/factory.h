/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose. 
* It is provided "as is" without express or implied warranty.
*/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Buffer.h"
#include "material.h"
#include "matrix4x4.h"
#include "describablebuffer.h"
#include "builder.h"
#include "render.h"
#include "geometry2d.h"
#include "image.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class Factory;

typedef std::shared_ptr<Factory>  FactorySharedPtr;
/** @defgroup Factory  Factory-类工厂基类 
*  @{
*/
class GV3DCORE_API Factory{
protected:
	int _type;
public:
	enum {
		FACTORYTYPE_2D_GEOMETRY,//二维几何库
		FACTORYTYPE_2D_RENDER,  //二维显示库
		FACTORYTYPE_3D_GEOMETRY,//三维几何库
		FACTORYTYPE_FDB_BUILDER,//文件数据库
		FACTORYTYPE_RDB_BUILDER,//关系数据库
		FACTORYTYPE_CDB_BUILDER,//集群数据库
		FACTORYTYPE_DDB_BUILDER,//分布式并行数据库
		FACTORYTYPE_3D_RENDER,
		FACTORYTYPE_MESH,//网格库
		FACTORYTYPE_FMESH_BUILDER,//文件网格数据库
		FACTORYTYPE_RMESH_BUILDER,//基于关系数据库的网格数据库
		FACTORYTYPE_CMESH_BUILDER,//基于集群的网格数据库
		FACTORYTYPE_DMESH_BUILDER,//基于分布式数据库的网格数据库

		FACTORYTYPE_FTERRAIN_BUILDER,//文件地形数据库
		FACTORYTYPE_RTERRAIN_BUILDER,//基于关系数据库的地形数据库
		FACTORYTYPE_CTERRAIN_BUILDER,//基于集群的地形数据库
		FACTORYTYPE_DTERRAIN_BUILDER,//基于分布式数据库的地形数据库

		FACTORYTYPE_IMAGE
	};
public:
	Factory();
	virtual ~Factory();
	inline int getType(){return _type;}
public:
	static void setFactory(FactorySharedPtr p);
	static Factory * getFactory(int);
	/*static void*  loadModule(const char *  lib);
	static void  freeModule(void * module);*/
	static std::map<int,FactorySharedPtr> &   getFactoryMap();
};
/** @} */

/** @addtogroup Factory  Geometry2DFactory-2D几何工厂类 
*  @{
*/
class GV3DCORE_API Geometry2dFactory: public Factory
{
public:
	Geometry2dFactory() 
	{
		_type = FACTORYTYPE_2D_GEOMETRY;
	}
public:
	/** 根据类型，创建 Element2d
	* @param [in] type: 元素类型
	* @return Element2d对象指针
	*/	
	virtual  GeometrySharedPtr newGeometry(int signal)=0; 
	/** 根据Buffer，创建 Element2d
	* @param [in] buf: 元素对象的缓冲区
	* @return Element2d对象指针
	*/		
	virtual  GeometrySharedPtr newGeometry( Buffer & buf)=0; 
	
	/** 根据类型，创建 Element2d
	* @param [in] type: 元素类型
	* @return Element2d对象指针
	*/	
	virtual Element2dSharedPtr newElement( int type ) = 0;

	/** 根据Buffer，创建 Element2d
	* @param [in] buf: 元素对象的缓冲区
	* @return Element2d对象指针
	*/	
	virtual Element2dSharedPtr newElement( Buffer & buf) = 0;

	/** 创建 ElementCollection2d
	* @return ElementCollection2d指针
	*/	
	virtual ElementCollection2dSharedPtr newElementCollection( ) = 0;

	/** 创建 GeometryCollection2d
	* @return GeometryCollection2d指针
	*/	
	virtual GeometryCollection2dSharedPtr newGeometryCollection( ) = 0;

	/** 创建 Pen2d
	* @return Pen2d指针
	*/	
	virtual Pen2dSharedPtr newPen( ) = 0;

	/** 创建 Symbol2d */	
	virtual Symbol2dSharedPtr newSymbol( double width, double height ) = 0;

	/** 创建 SymbolLibrary2d */	
	virtual SymbolLibrary2dSharedPtr newSymbolLibrary( ) = 0;

	/** 创建 SymbolManager2d */	
	virtual SymbolManager2dSharedPtr newSymbolManager( int nType ) = 0;

	/** 创建 GeometrySelectManager2d */	
	virtual GeometrySelectManager2dSharedPtr newGeometrySelectManager2d() = 0;

	/** 创建射线 Ray */	
	virtual RaySharedPtr  newRay(const Vertex3d& o, const Vertex3d & d) = 0;

	/** 创建椎体 Frustum */	
	virtual FrustumSharedPtr  newFrustum() = 0;

	/** 创建路径 */	
	virtual Path2dSharedPtr  newPath() = 0;
	/** 创建路径集合 */	
	virtual PathCollection2dSharedPtr  newPathCollection() = 0;
	/** 创建环 */	
	virtual Ring2dSharedPtr  newRing() = 0;
	/** 创建环集合 */	
	virtual RingCollection2dSharedPtr  newRinghCollection() = 0;
};
/** @} */



/** @addtogroup Factory  Render2dFactory-2D显示工厂类 
*  @{
*/
class GV3DCORE_API Render2dFactory: public cn::edu::cug::gdb::Factory
{
public:
	Render2dFactory()
	{
		_type = FACTORYTYPE_2D_RENDER;
	}

public:
	/** 创建 Render2d
	* @param [in] type: 元素类型
	* @param [out] 
	* @return Render2d对象指针
	*/	
	virtual  Render2dSharedPtr newRender(int signal)=0; 
	
	
	/** 创建 Coordsys2d
	* @param [in] type: 元素类型
	* @param [out] 
	* @return Coordsys2d对象指针
	*/	
	virtual  Coordsys2dSharedPtr newCoordsys()=0; 
};
/** @} */



/** @addtogroup Factory  Geometry3dFactory-3D几何工厂类 
*  @{
*/
class GV3DCORE_API Geometry3dFactory: public Factory{
public:
	Geometry3dFactory(){
		_type = FACTORYTYPE_3D_GEOMETRY;
	}
public:
	virtual  MaterialSharedPtr newSimpleMaterial()=0;
	virtual  MaterialSharedPtr newComplexMaterial()=0;
	virtual  TextureSharedPtr  newTexture()=0;
	virtual  SharedModelSharedPtr newSharedModel()=0;
	virtual  GeometrySharedPtr newGeometry(int signal)=0; 
	virtual  GeometrySharedPtr newGeometry( Buffer & buf)=0; 
	virtual  VertexVisitorSharedPtr newVertexVisitor(int signal)=0;
	virtual  RaySharedPtr  newRay(const Vertex3d& o, const Vertex3d & d)=0;
	virtual  FrustumSharedPtr  newFrustum()=0;
	virtual DEMGridSharedPtr newDEMGrid()=0;
	virtual DOMGridSharedPtr newDOMGrid()=0;
};
/** @} */



/** @addtogroup Factory  ImageFactory-图像工厂类 
*  @{
*/
class GV3DCORE_API ImageFactory: public Factory{
public:
	ImageFactory(){
		_type = FACTORYTYPE_IMAGE;
	}
public:
	 
	virtual ImageSharedPtr newImage()=0;
	virtual ImageSharedPtr newImage(unsigned w, unsigned h, unsigned bpp, void * pdata)=0;
};
/** @} */



/** @addtogroup Factory  FDBBuilderFactory-文件数据库工厂类 
*  @{
*/
class GV3DCORE_API FDBBuilderFactory: public Factory{
public:
	FDBBuilderFactory(){
		_type = FACTORYTYPE_FDB_BUILDER;
	}
public:
	virtual  BuilderSharedPtr newBuilder()=0;
};
/** @} */



/** @addtogroup Factory  RDBBuilderFactory-关系数据库工厂类 
*  @{
*/
class GV3DCORE_API RDBBuilderFactory: public Factory{
public:
	RDBBuilderFactory(){
		_type = FACTORYTYPE_RDB_BUILDER;
	}
public:
	virtual  BuilderSharedPtr newBuilder()=0;
};
/** @} */



/** @addtogroup Factory  CDBBuilderFactory-集群数据库工厂类 
*  @{
*/
class GV3DCORE_API CDBBuilderFactory: public Factory{//集群数据库
public:
	CDBBuilderFactory(){
		_type = FACTORYTYPE_CDB_BUILDER;
	}
public:
	virtual  BuilderSharedPtr newBuilder()=0;
};
/** @} */



/** @addtogroup Factory  CDBBuilderFactory-分布式数据库工厂类 
*  @{
*/
class GV3DCORE_API DDBBuilderFactory: public Factory{ 
public:
	DDBBuilderFactory(){
		_type = FACTORYTYPE_DDB_BUILDER;
	}
public:
	virtual  BuilderSharedPtr  newBuilder()=0;
};
/** @} */



/** @addtogroup Factory  Render3dFactory-3D渲染工厂类 
*  @{
*/
class GV3DCORE_API Render3dFactory : public gdb::Factory {
public:
	Render3dFactory(){
		_type = FACTORYTYPE_3D_RENDER;
	}
	virtual  RenderSharedPtr newRender()=0;

};
/** @} */
typedef std::shared_ptr<Render3dFactory> Render3dFactorySharedPtr;


/** @addtogroup Factory  MeshFactory-网格工厂类 
*  @{
*/
class GV3DCORE_API MeshFactory : public gdb::Factory {
public:
	MeshFactory(){
		_type = FACTORYTYPE_MESH;
	}
	virtual  MeshSharedPtr newMesh(int signal)=0;
};
/** @} */
typedef std::shared_ptr<MeshFactory> MeshFactorySharedPtr;

/** @addtogroup Factory  FMeshBuilderFactory-文件版网格库工厂类 
*  @{
*/
class GV3DCORE_API FMeshBuilderFactory: public Factory{
public:
	FMeshBuilderFactory(){
		_type = FACTORYTYPE_FMESH_BUILDER;
	}
public:
	virtual  MeshBuilderSharedPtr newBuilder()=0;
};
/** @} */
/** @addtogroup Factory  RMeshBuilderFactory-关系数据库版网格库工厂类 
*  @{
*/
class GV3DCORE_API RMeshBuilderFactory: public Factory{
public:
	RMeshBuilderFactory(){
		_type = FACTORYTYPE_RMESH_BUILDER;
	}
public:
	virtual  MeshBuilderSharedPtr newBuilder()=0;
};
/** @} */
/** @addtogroup Factory  CMeshBuilderFactory-集群版网格库工厂类 
*  @{
*/
class GV3DCORE_API CMeshBuilderFactory: public Factory{
public:
	CMeshBuilderFactory(){
		_type = FACTORYTYPE_CMESH_BUILDER;
	}
public:
	virtual  MeshBuilderSharedPtr newBuilder()=0;
};
/** @} */
/** @addtogroup Factory  DMeshBuilderFactory-分布式版网格库工厂类 
*  @{
*/
class GV3DCORE_API DMeshBuilderFactory: public Factory{//集群数据库
public:
	DMeshBuilderFactory(){
		_type = FACTORYTYPE_DMESH_BUILDER;
	}
public:
	virtual  MeshBuilderSharedPtr  newBuilder()=0;
};
/** @} */

/** @addtogroup Factory  FTerrainBuilderFactory-文件版地形库工厂类 
*  @{
*/
class GV3DCORE_API FTerrainBuilderFactory: public Factory{
public:
	FTerrainBuilderFactory(){
		_type = FACTORYTYPE_FTERRAIN_BUILDER;
	}
public:
	virtual  TerrainBuilderSharedPtr newBuilder()=0;
};
/** @} */
/** @addtogroup Factory  RTerrainBuilderFactory-关系数据库版地形库工厂类 
*  @{
*/
class GV3DCORE_API RTerrainBuilderFactory: public Factory{
public:
	RTerrainBuilderFactory(){
		_type = FACTORYTYPE_RTERRAIN_BUILDER;
	}
public:
	virtual  TerrainBuilderSharedPtr newBuilder()=0;
};
/** @} */
/** @addtogroup Factory  CTerrainBuilderFactory-集群版地形库工厂类 
*  @{
*/
class GV3DCORE_API CTerrainBuilderFactory: public Factory{
public:
	CTerrainBuilderFactory(){
		_type = FACTORYTYPE_CTERRAIN_BUILDER;
	}
public:
	virtual  TerrainBuilderSharedPtr newBuilder()=0;
};
/** @} */
/** @addtogroup Factory  DTerrainBuilderFactory-分布式版地形库工厂类 
*  @{
*/
class GV3DCORE_API DTerrainBuilderFactory: public Factory{
public:
	DTerrainBuilderFactory(){
		_type = FACTORYTYPE_DTERRAIN_BUILDER;
	}
public:
	virtual  TerrainBuilderSharedPtr  newBuilder()=0;
};
/** @} */
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
