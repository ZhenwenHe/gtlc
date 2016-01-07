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
#include <map>

#include "connector.h"
#include "meshbuilder.h"
#include "terrainbuilder.h"
#include "builder.h"




begin_gtl_namespace
begin_gdb_namespace

class Builders;
/** @defgroup Builders Builders-空间数据库编辑接口集合 
*  @{
*/

typedef std::shared_ptr<Builders> BuildersSharedPtr;

class CORE_API Builders  {
protected:
	BuilderSharedPtr        _modelBuilder;
	MeshBuilderSharedPtr    _meshBuilder;
	TerrainBuilderSharedPtr _terrainBuilder;
public:	
	Builders();
	virtual ~Builders();
	Builders(const Builders& b);
public:	
	virtual void copy(const Builders& b);	
	virtual void close();
	virtual bool open(const Connector & conn);
	virtual Builders operator=(const Builders & b);
public:	
	inline BuilderSharedPtr getModelBuilder() {return _modelBuilder;}
	inline MeshBuilderSharedPtr getMeshBuilder() {return _meshBuilder;}
	inline TerrainBuilderSharedPtr getTerrainBuilder() { return _terrainBuilder;}
	inline void setModelBuilder(BuilderSharedPtr p) {  _modelBuilder=p;}
	inline void setMeshBuilder(MeshBuilderSharedPtr p) {_meshBuilder=p;}
	inline void setTerrainBuilder(TerrainBuilderSharedPtr p) {_terrainBuilder=p;}
	inline Selector & getSelector(){return _modelBuilder->getSelector();}
	inline void getEnvelope(Envelope3d & r);
	inline Envelope3d getEnvelope();
};
/** @} */ 

inline void Builders::getEnvelope(Envelope3d & r){
	Envelope3d e ;
	if(_modelBuilder){
		if(!_modelBuilder->isEmpty()){
			e = _modelBuilder->getProxy()->getEnvelope();
			r = e;
		}			
	}
	if(_meshBuilder){
		if(!_meshBuilder->isEmpty()){
			e = _meshBuilder->getEnvelope();
			r.extend(e);
		}			
	}
	if(_terrainBuilder){
		if(!_terrainBuilder->isEmpty()){
			e = _terrainBuilder->getProxy()->getEnvelope();
			r.extend(e);
		}			
	}
}
inline Envelope3d Builders::getEnvelope ( ){
	Envelope3d e ;
	getEnvelope(e);
	return e;
}
end_gdb_namespace
end_gtl_namespace


