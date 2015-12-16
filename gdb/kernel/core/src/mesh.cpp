#include "mesh.h"
#include "factory.h"
#include "internal_EclipseMeshImpl.h"
 
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

Mesh::Mesh( ){
	_meshType = MESH_TYPE_UNKONWN;
	_xCellNumber=0;
	_yCellNumber=0;
	_zCellNumber=0;
	_propNumberPreCell = 0;
	_propValues=0;
	_parent=0;

	_beginX=0;
	_endX=_beginX+_xCellNumber;
	_beginY=0;
	_endY=_beginY+_yCellNumber;
	_beginZ=0;
	_endZ=_beginZ+_zCellNumber;

}
void Mesh::clear() {
	_xCellNumber=0;
	_yCellNumber=0;
	_zCellNumber=0;
	_propNumberPreCell = 0;
	_validBitset.clear();
	_propNames.clear();
	if(_propValues)
		delete []  _propValues;//new double [_xCellNumber*_yCellNumber*_zCellNumber*_propNumberPreCell];
	_propValues=0;
	_parent=0;
	_children.clear();


	_beginX=0;
	_endX=_beginX+_xCellNumber;
	_beginY=0;
	_endY=_beginY+_yCellNumber;
	_beginZ=0;
	_endZ=_beginZ+_zCellNumber;

}
void Mesh::copy(const Mesh & p){
	_meshType = p._meshType;
	reset(p._xCellNumber,p._yCellNumber,p._zCellNumber,p._propNumberPreCell);
	_validBitset = p._validBitset;
	_propNames.resize(p._propNumberPreCell);
	std::copy(p._propNames.begin(),p._propNames.end(),_propNames.begin());
	memcpy(_propValues,p._propValues,_xCellNumber*_yCellNumber*_zCellNumber*_propNumberPreCell);

	_children.resize(p._children.size());
	//只拷贝指针，浅拷贝
	std::copy(p._children.begin(),p._children.end(),_children.begin());

	_beginX=p._beginX;
	_endX=p._endX;
	_beginY=p._beginY;
	_endY= p._endY;
	_beginZ=p._beginZ;
	_endZ=p._endZ;

	_origin = p._origin;
}
Mesh::Mesh(unsigned long long cx, unsigned long long cy,unsigned long long cz,unsigned long long propNumberPreCell ){
	_xCellNumber=cx;
	_yCellNumber=cy;
	_zCellNumber=cz;
	_propNumberPreCell = propNumberPreCell;
	_validBitset.set(_xCellNumber*_yCellNumber*_zCellNumber-1,false);
	_propNames.resize(_propNumberPreCell);
	_propValues = new double [_xCellNumber*_yCellNumber*_zCellNumber*_propNumberPreCell];

	_beginX=0;
	_endX=_beginX+_xCellNumber;
	_beginY=0;
	_endY=_beginY+_yCellNumber;
	_beginZ=0;
	_endZ=_beginZ+_zCellNumber;

	_meshType = MESH_TYPE_UNKONWN;
}
Mesh::Mesh(const Mesh & p){
	copy(p);
}

void Mesh::reset(unsigned long long cx, unsigned long long cy,unsigned long long cz,unsigned long long propNumberPreCell){
	clear();
	_xCellNumber=cx;
	_yCellNumber=cy;
	_zCellNumber=cz;
	_propNumberPreCell = propNumberPreCell;
	_validBitset.set(_xCellNumber*_yCellNumber*_zCellNumber-1,false);
	_propNames.resize(_propNumberPreCell);
	_propValues = new double [_xCellNumber*_yCellNumber*_zCellNumber*_propNumberPreCell];
	_children.clear();

	_beginX=0;
	_endX=_beginX+_xCellNumber;
	_beginY=0;
	_endY=_beginY+_yCellNumber;
	_beginZ=0;
	_endZ=_beginZ+_zCellNumber;

	_origin = Vertex3d(0,0,0);

}

void Mesh::getPropRanges(std::vector<double> & ranges){
	ranges.resize(_propNumberPreCell*2);
	for(int i=0;i<_propNumberPreCell;i++){
		ranges[i*2]=DBL_MAX;
		ranges[i*2+1]=-(DBL_MAX-1);
	}

	for(unsigned long long z=0;z<_zCellNumber;z++){
		for(unsigned long long y=0;y<_yCellNumber;y++){
			for(unsigned long long x=0;x<_xCellNumber;x++){
				Cell & c = getCurrentCell(x,y,z);
				if(!c.isValid()) continue;
				std::vector<double> & v = c.getPropValues();
				for(int i=0;i<_propNumberPreCell;i++){
					ranges[i*2]=min(ranges[i*2],v[i]);
					ranges[i*2+1]=max(ranges[i*2+1],v[i]);
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////
MeshBuilder::MeshBuilder() {

	_propNumberPreCell = 0;//每个单元的属性个数
	_indexOfProp4Color = 0;
}

/////////////////////////////////////////////////////////////////////////////////

MeshSharedPtr Mesh::create(int type ){	
#if (USING_DEFAULT_MESHLIB==0)
	return ((MeshFactory*)Factory::getFactory(Factory::FACTORYTYPE_MESH))->newMesh(type);
#else// 
	if (type == MESH_TYPE_ECLIPSE)
	{
		return MeshSharedPtr(new EclipseMeshImpl());
	}
	else if (type == MESH_TYPE_DEM)
	{
		return MeshSharedPtr();
	} 
	else
	{
		return MeshSharedPtr( );
	}
#endif
	
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
