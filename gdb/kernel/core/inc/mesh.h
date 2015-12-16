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
#include "bitset.h"
#include "Buffer.h"
#include "material.h"
#include "matrix4x4.h"
#include "describablebuffer.h"


begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class Mesh;
class Block;
class Cell;


typedef std::shared_ptr<Cell> CellSharedPtr;
typedef std::shared_ptr<Mesh> MeshSharedPtr;
typedef std::shared_ptr<Block> BlockSharedPtr;

/** @defgroup Mesh 网格相关类  
*  @{
*/
enum {
	MESH_TYPE_UNKONWN=0,
	MESH_TYPE_ECLIPSE,
	MESH_TYPE_DEM
};
/** @} */


/** @addtogroup Mesh Cell-网格单元  
*  @{
*/
class GV3DCORE_API Cell {
	unsigned long long _x,_y,_z;
	std::vector<double>  _propValues;
	std::vector<Vertex3d> _vertices;
	bool                  _valid;
public:
	Cell();
	virtual ~Cell();
	virtual void reset(unsigned long long ix,unsigned long long iy,unsigned long long iz,
		std::vector<double>& p, std::vector<Vertex3d> & v);
	virtual std::vector<double> & getPropValues();
	virtual std::vector<Vertex3d> & getVertices();
	virtual void copy(const Cell & c);
	virtual void clear();
	virtual Cell operator = (const Cell & c);
	Cell(const Cell & c);
	virtual void  getEnvelope(Envelope3d & e3d) ;
	virtual void output(ofstream & f);
public:
	/** 获取对象写入到Buffer中所占用的字节数，便于从Buffer对象中构建几何对象
     * @param  [in] 无 
     * @return 返回几何对象写入到Buffer中所占用的字节数
	 */
    virtual size_t sizeBuffer() ;
	/** 从Buffer中读取信息填充本对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf);
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf);
	/* 将本类的信息写入输出流中，便于文件读写或缓冲区写操作 
     * @param  [in,out] f  std::ostream &，标准输出流对象引用
     * @return 成功返回true，否则返回false
    */
    virtual void write(std::ostream & f);
	/** 从标准输入流中提取本类的信息  
     * @param  [in,out] f  std::istream &，标准输入流对象引用
     * @return 成功返回true，否则返回false
    */
    virtual void read(std::istream & f);
public:
	inline unsigned long long getX() {return _x;}
	inline unsigned long long getY() {return _y;}
	inline unsigned long long getZ() {return _z;}

	inline void setX(unsigned long long v) {_x=v;}
	inline void setY(unsigned long long v) {_y=v;}
	inline void setZ(unsigned long long v) {_z=v;}

	inline bool isValid() {return _valid;}
	inline void setValid(bool b) { _valid=b;}

	friend class Mesh;
};
/** @} */


/** @addtogroup Mesh Cell-网格单元  
*  @{
*/
class GV3DCORE_API Block {
	Identifier                 _identifier;//唯一标识
	unsigned long long         _layerID;//所在层的ID
	unsigned long long         _xCellNumber;//X方向逻辑单元数
	unsigned long long         _yCellNumber;//Y方向逻辑单元数
	unsigned long long         _zCellNumber;//Z方向逻辑单元数   
	/** 上级网格的X方向的开始和结束单元下标，从0开始,为半开区间，默认是情况下_endX = _beginX+_xCellNumber */
	unsigned long long _beginX;
	/** 上级网格的Y方向的开始和结束单元下标，从0开始，为半开区间，默认是情况下_endY = _beginY+_yCellNumber */
	unsigned long long _beginY;
	/** 上级网格的Z方向的开始和结束单元下标，从0开始，为半开区间，默认是情况下_endZ = _beginZ+_zCellNumber */
	unsigned long long _beginZ;

	std::vector<CellSharedPtr>  _cells;

	cn::edu::cug::gdb::Envelope3d   _envelope;//块边界
public:
	Block(unsigned long long cx,  unsigned long long cy, unsigned long long cz);
	virtual ~Block();
	virtual void output(ofstream & f);
	static void output(std::vector<BlockSharedPtr> & blks,const std::string &filename);
public:
	/** 获取对象写入到Buffer中所占用的字节数，便于从Buffer对象中构建几何对象
     * @param  [in] 无 
     * @return 返回几何对象写入到Buffer中所占用的字节数
	 */
    virtual size_t sizeBuffer() ;
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) ;
	/* 将本类的信息写入输出流中，便于文件读写或缓冲区写操作 
     * @param  [in,out] f  std::ostream &，标准输出流对象引用
     * @return 成功返回true，否则返回false
    */
    virtual void write(std::ostream & f);
	/** 从标准输入流中提取本类的信息  
     * @param  [in,out] f  std::istream &，标准输入流对象引用
     * @return 成功返回true，否则返回false
    */
    virtual void read(std::istream & f);
public:
	virtual void clear();
	virtual void addCell(CellSharedPtr c);	
	
public:
	inline Identifier & getIdentifier() { return _identifier; }
	inline Envelope3d & getEnvelope() {return _envelope;}
	inline unsigned long long  getXCellNumber(){ return _xCellNumber;}
	inline unsigned long long  getYCellNumber(){ return _yCellNumber;}
	inline unsigned long long  getZCellNumber(){ return _zCellNumber;}
	inline unsigned long long   getXBegin() {return _beginX;}
	inline unsigned long long   getYBegin() {return _beginY;}
	inline unsigned long long   getZBegin() {return _beginZ;}
	inline unsigned long long   getXEnd() {return _beginX + _xCellNumber;}
	inline unsigned long long   getYEnd() {return _beginY + _yCellNumber;}
	inline unsigned long long   getZEnd() {return _beginZ + _zCellNumber;}

	inline void   setXBegin(unsigned long long v) { _beginX=v;}
	inline void   setYBegin(unsigned long long v) { _beginY=v;}
	inline void   setZBegin(unsigned long long v) { _beginZ=v;}

	inline unsigned long long   getLayerID() {return _layerID;}
	inline void setLayerID(unsigned long long lid) {_layerID=lid;}

	inline std::vector<CellSharedPtr> & getCells(){return _cells;}


	friend class Mesh;
};
/** @} */


/** @addtogroup Mesh Mesh-网格基类 
*  @{
*/
class GV3DCORE_API Mesh{	
protected:
	Cell                       _currentCell;//当前网格对象，不用存储，只是为了操作方便的临时对象；

	int                        _meshType;	   //网格类型
	gdb::Vertex3d              _origin;// 原点坐标  
	unsigned long long         _xCellNumber;//X方向逻辑单元数
	unsigned long long         _yCellNumber;//Y方向逻辑单元数
	unsigned long long         _zCellNumber;//Z方向逻辑单元数
	gdb::Bitset                _validBitset;//每个单元的有效性true标识有效，false标识无效
	unsigned long long         _propNumberPreCell;//每个单元的属性个数
	std::vector<std::string>   _propNames;//每个属性的名称，数组中元素个数等于_propNumberPreCell
	double *                   _propValues;//网格的所有属性值，每个单元的_propNumberPreCell个属性存放在一起，存放优先顺序为X方向，Y方向，Z方向
	/** 下级网格对象数组*/
	std::vector<MeshSharedPtr>  _children;
protected://如果是子网格，则下面变量生效
	Mesh *                    _parent;
	/** 上级网格的X方向的开始和结束单元下标，从0开始,为半开区间，默认是情况下_endX = _beginX+_xCellNumber,如果不相等，则标识为加密或抽稀疏子网格*/
	unsigned long long _beginX, _endX;
	//newXCellNumb; =_xCellNumber//X方向上子网格单元数
	/** 上级网格的Y方向的开始和结束单元下标，从0开始，为半开区间，默认是情况下_endY = _beginY+_yCellNumber,如果不相等，则标识为加密或抽稀疏子网格*/
	unsigned long long _beginY, _endY;
	//newYCellNumb; =_yCellNumber//Y方向上子网格单元数
	/** 上级网格的Z方向的开始和结束单元下标，从0开始，为半开区间，默认是情况下_endZ = _beginZ+_zCellNumber,如果不相等，则标识为加密或抽稀疏子网格*/
	unsigned long long _beginZ, _endZ;
	//newZCellNumb;= _zCellNumber//Z方向上子网格单元数
public:
	Mesh();	
	Mesh(unsigned long long cx, unsigned long long cy,unsigned long long cz,unsigned long long propNumberPreCell=1);
	Mesh(const Mesh & p);
	static MeshSharedPtr create(int type );
public:
	virtual void clear();
	virtual void copy(const Mesh & p);
	virtual void reset(unsigned long long cx, unsigned long long cy,unsigned long long cz,unsigned long long propNumberPreCell=1);
	virtual void getPropRanges(std::vector<double> & ranges);
public:
	inline double * getCellValues(unsigned long long ix, unsigned long long iy,unsigned long long iz) {
		return _propValues+(iz*_yCellNumber*_xCellNumber+iy*_xCellNumber+ix)*_propNumberPreCell;
	}
	inline bool isCellValid(unsigned long long ix, unsigned long long iy,unsigned long long iz) {
		return _validBitset.get( (size_t)(iz*_yCellNumber*_xCellNumber+iy*_xCellNumber+ix) );
	}
	inline void setCellValid(unsigned long long ix, unsigned long long iy,unsigned long long iz,bool flag) {
		return _validBitset.set( (size_t)(iz*_yCellNumber*_xCellNumber+iy*_xCellNumber+ix),flag );
	}
	inline void setPropValues(const double * values){ 
		memcpy(_propValues,values, (size_t)(_xCellNumber*_yCellNumber*_zCellNumber*_propNumberPreCell) ); }
	
	inline const double * getPropValues()const {return _propValues;}
	inline std::vector<std::string> & getPropNames() {return _propNames;}
	inline unsigned long long  getXCellNumber(){ return _xCellNumber;}
	inline unsigned long long  getYCellNumber(){ return _yCellNumber;}
	inline unsigned long long  getZCellNumber(){ return _zCellNumber;}
	inline unsigned long long  getPropNumberPreCell(){ return _propNumberPreCell;}
	inline std::vector<MeshSharedPtr> & getChildren() { return _children;}
	inline Cell & getCurrentCell() {return _currentCell;}
	inline int getType() { return _meshType;}
	inline Vertex3d & getOrigin() { return _origin;}
	inline Bitset& getValidBitset(){return _validBitset;}
public:	
	inline unsigned long long   getXBegin() {return _beginX;}
	inline unsigned long long   getYBegin() {return _beginY;}
	inline unsigned long long   getZBegin() {return _beginZ;}
	inline unsigned long long   getXEnd() {return _endX;}
	inline unsigned long long   getYEnd() {return _endY;}
	inline unsigned long long   getZEnd() {return _endZ;}
	inline Mesh * getParent(){return _parent;}
public:
	virtual Cell& getCurrentCell(unsigned long long ix, unsigned long long iy,unsigned long long iz)=0;
	virtual CellSharedPtr getCell(unsigned long long ix, unsigned long long iy,unsigned long long iz)=0;
	virtual MeshSharedPtr submesh(
		unsigned long long bx, unsigned long long ex,
		unsigned long long by, unsigned long long ey,
		unsigned long long bz, unsigned long long ez,
		unsigned long long cx, unsigned long long cy,unsigned long long cz)=0;
	virtual void queryInterface(int type, void ** ppInterface)=0;
	virtual void getBlocks(unsigned long long xCellNumberPreBlock,unsigned long long yCellNumberPreBlock,unsigned long long zCellNumberPreBlock,std::vector<BlockSharedPtr> & blocks)=0;
	virtual Envelope3d getEnvelope()=0;
	virtual void setOrigin(const Vertex3d & v)=0; 
	
};
/** @} */ 


/** @addtogroup EclipseMesh EclipseMesh-Eclipse网格  
*  @{
*/
class GV3DCORE_API EclipseMesh : public Mesh {
public:
	virtual double * reallocZValues()=0;
	virtual unsigned long long getZValuesNumber()=0;
	virtual double * getZValues()=0;	
	virtual std::vector<gdb::Vertex3d> & getTopCtrlVertices()=0;
	virtual std::vector<gdb::Vertex3d> & getBottomCtrlVertices()=0;
	virtual bool readAscii(const std::string & )=0;
};
/** @} */


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
