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
#include "buffer.h"
#include "material.h"
#include "matrix4x4.h"
#include "indexbuffer.h"
#include "connector.h" 
#include "terrainproxy.h"



begin_gtl_namespace
begin_gdb_namespace

class  TerrainBuilder;
typedef std::shared_ptr<TerrainBuilder>  TerrainBuilderSharedPtr;
/** @addtogroup terrain TerrainBuilder-地形数据库管理指针　
*  @{
*/
class CORE_API TerrainBuilder
{
protected:
	/** 打开DEM数据库
	* @param [in]  dbinfo		DEM数据库登录信息（文件系统为文件的路径；数据库系统为登录的用户名录及数据库名称等）
	* @return
	*/
	virtual bool openDEMDB(const Connector & conn) = 0;


	/** 打开DOM数据库
	* @param [in]  dbinfo		DOM数据库登录信息（文件系统为文件的路径；数据库系统为登录的用户名录及数据库名称等）
	* @return
	*/
	virtual bool openDOMDB(const Connector & conn) = 0;


	/** 关闭DEM数据库
	* @param [in]  demid		DEM数据库ID
	* @return
	*/
	virtual bool closeDEMDB() = 0;


	/** 关闭DOM数据库
	* @param [in]  domid		DOM数据库ID
	* @return
	*/
	virtual bool closeDOMDB() = 0;

	/** 创建DEM数据库
	* @param [in]  demDBName 		DEM数据库名称
	* @param [in]  demDatainfo 		DEM数据描述信息
	* @param [in]  scope 			DEM数据库范围
	* @return
	*/
	virtual bool createDEMDB(const Connector & conn, DEMDBInfoSharedPtr demdbinfo) = 0;

	/** 创建DOM数据库
	* @param [in]  domDBName 		DOM数据库名称
	* @param [in]  domDatainfo 		DOM数据描述信息
	* @param [in]  scope 			DOM数据库范围
	* @return
	*/
	virtual bool createDOMDB(const Connector & conn, DOMDBInfoSharedPtr domdbinfo) = 0;

	/** 创建DEM新金字塔层
	* @param [in]  nLayerID			DEM金字塔层ID
	* @param [in]  szLayerName		DEM层名称
	* @param [in]  LayerInfo		DEM层信息
	* @return
	*/
	virtual bool newDEMLayer(Identifier::raw_type  nLayerID, DEMLayerInfoSharedPtr LayerInfo, int indexPosition = -1) = 0;


	/** 创建DOM新金字塔层
	* @param [in]  nLayerID			DOM金字塔层ID
	* @param [in]  szLayerName		DOM层名称
	* @param [in]  LayerInfo		DOM层信息
	* @return
	*/
	virtual bool newDOMLayer(Identifier::raw_type  nLayerID, DOMLayerInfoSharedPtr LayerInfo, int indexPosition = -1) = 0;

	/**清空DEM数据库
	* @param [in]  demid			DEM数据库ID
	* @return
	*/
	virtual bool clearDEMDBData() = 0;

	/**清空DOM数据库
	* @param [in]  domdbid			DOM数据库ID
	* @return
	*/
	virtual bool clearDOMDBData() = 0;

	/** 删除DEM数据库
	* @param [in]  demid			DEM数据库ID
	* @return
	*/
	virtual bool deleteDEMDB() = 0;

	/** 删除DOM数据库
	* @param [in]  domid			DOM数据库ID
	* @return
	*/
	virtual bool deleteDOMDB() = 0;



	/** 平移DEM数据库
	* @param [in]	x		x方向平移长度
	* @param [in]	y		y方向平移长度
	* @return
	*/
	virtual bool translateDEMDB(double x, double y) = 0;

	/** 平移DOM数据库
	* @param [in]	x		x方向平移长度
	* @param [in]	y		y方向平移长度
	* @return
	*/
	virtual bool translateDOMDB(double x, double y) = 0;


public:
	/**  判断工程是否为空
	* @param 无
	* @return 如果为空返回true，否则返回false
	*/
	virtual bool isEmpty() = 0;

	/** 获取当前层的ID
	* @param [in]  无
	* @return 当前层的ID
	*/
	virtual Identifier::raw_type getCurrentLayerID() = 0;

	/** 设置当前层的ID
	* @param [in]  int layerID
	* @return
	*/
	virtual bool setCurrentLayerID(Identifier::raw_type layerID) = 0;

	/** 关闭数据库
	* @param [in]  无
	* @return
	*/
	virtual bool close() = 0;

	/** 打开数据库
	* @param [in]  Connector: conn
	* @return
	*/
	virtual bool open(const Connector &) = 0;

	/** 创建数据库
	* @param [in]  Connector: conn
	* @return
	*/
	virtual bool create(const Connector &, DEMDBInfoSharedPtr &, DOMDBInfoSharedPtr &) = 0;
	/** 创建DEM数据库
	* @param [in]  Connector: conn
	* @return
	*/
	virtual bool create(const Connector &, DEMDBInfoSharedPtr &) = 0;

	/** 创建DOM数据库
	* @param [in]  Connector: conn
	* @return
	*/
	virtual bool create(const Connector &, DOMDBInfoSharedPtr &) = 0;

	/** 获取地形查询指针对象
	* @return
	*/
	virtual TerrainProxySharedPtr getProxy() = 0;



	/** 获取DEM数据库信息
	* @param [out]  demdbinfo 		DEM数据库信息
	* @return
	*/
	virtual DEMDBInfoSharedPtr getDEMDBInfo() = 0;

	/** 获取DEM数据库信息
	* @param [out]  domdbinfo 		DOM数据库信息
	* @return
	*/
	virtual DOMDBInfoSharedPtr getDOMDBInfo() = 0;

	/** 删除一个金字塔层
	* @param [in]  nLayerID			金字塔层ID
	* @return
	*/
	virtual bool deleteLayer(Identifier::raw_type  nLayerID) = 0;


	/** 提交单个DEM数据数据块
	* @param [in]  blkdata			DEM数据
	* @param [in]  bEnd				是否最后一个数据块
	* @return
	*/
	virtual bool submitDEMBlock(DEMBlockSharedPtr blkdata) = 0;


	/** 提交多个DEM数据数据块
	* @param [in]  nLayerID			DEM金字塔层ID
	* @param [in]  blkdatas			DEM数据集合
	* @return
	*/
	virtual bool submitDEMBlocks(Identifier::raw_type  nLayerID, std::vector<DEMBlockSharedPtr>& blkdatas) = 0;


	/** 提交DEM数据
	* @param [in]  szFileName		当前递交的DEM文件路径及名称，用于记录原始DEM数据来源
	* @param [in]  demObject		当前递交的DEM数据
	* @param [in]  nUpdateType		新递交数据与库中原有数据有重叠时的处理方式0:更新为新值 ；1:和已有值融合; 2不替换，保留已有值
	* @return
	*/
	virtual bool submitDEM(DEMGridSharedPtr demObj) = 0;

	/** 提交单个DOM数据数据块
	* @param [in]  blkdata			DOM数据
	* @return
	*/
	virtual bool submitDOMBlock(DOMBlockSharedPtr blkdata) = 0;

	/** 提交多个DOM数据数据块
	* @param [in]  nLayerID			DOM金字塔层ID
	* @param [in]  blkdatas			DOM数据集合
	* @return
	*/
	virtual bool submitDOMBlocks(Identifier::raw_type  nLayerID, std::vector<DOMBlockSharedPtr>& blkdatas) = 0;


	/** 提交DOM数据
	* @param [in]  nLayerID			DOM金字塔层ID
	* @param [in]  szFileName		当前递交的DOM文件路径及名称
	* @param [in]  imageData			当前递交的DOM数据指针
	* @param [in]  DomInfo			当前递交的DOM数据信息
	* @param [in]  scope			当前递交的DOM范围
	* @param [in]  nUpdateType		新递交数据与库中原有数据有重叠时的处理方式0:更新为新值 ；1:和已有值融合; 2不替换，保留已有值
	* @return
	*/
	virtual bool submitDOM(DOMGridSharedPtr domObj) = 0;

	/** 调整DEM数据库范围为正方形
	* @param [in]	srcDemScope			原始DEM数据库范围
	* @param [out]	adjustDemScope		调整后的DEM数据库范围
	* @return
	*/
	virtual bool adjustDEMDBScopeToMinSquare(const Envelope2d& srcDemScope, const Envelope2d& adjustDemScope) = 0;

	/** 调整DEM的格网间距，使DEM数据库的边长顶点数为2N+1
	* @param [in]	adjustDemScope		调整后的DEM数据库范围
	* @param [in]	srcCellsize			原始DEM数据库格网间距
	* @param [out]	adjustCellsize		调整后的DEM数据库格网间距
	* @return
	*/
	virtual bool adjustDEMCellSizeTo2N(const Envelope2d& adjustScope, double srcCellsize, double& adjustCellsize) = 0;

	/** 调整DEM的范围，使DEM数据库的边长顶点数为2N+1
	* @param [in]	srcCellsize			原始DEM数据库格网间距
	* @param [in]	srcDemScope			调整后的DEM数据库范围
	* @param [out]	adjustDemScope		调整后的DEM数据库范围
	* @return
	*/
	virtual bool adjustDEMDBScopeTo2NSquare(double cellsize, const Envelope2d& srcDemScope, const Envelope2d& adjustDemScope) = 0;

	/** 调整指定金字塔层范围
	* @param [in]  nLayerID			DEM金字塔层ID
	* @param [out] demLayerScope	调整后的范围
	* @return
	*/
	virtual bool adjustLayerScope(Identifier::raw_type  nLayerID, const Envelope2d& demLayerScope, int& colNum, int& rowNum) = 0;
	/** 得到connector
	* @return
	*/
	virtual  Connector & getConnector() = 0;
public:
	//用于判断DEM或DOM某层是否有数据
	virtual bool isDOMLayerEmpty(Identifier::raw_type layid) = 0;
	virtual bool isDEMLayerEmpty(Identifier::raw_type layid) = 0;
	//创建DEM金字塔
	virtual bool createDEMPyramid() = 0;
	virtual bool createDOMPyramid() = 0;


	virtual double getZValue(Identifier::raw_type layerID, unsigned long long row, unsigned long long col) = 0;
	virtual void setZValue(Identifier::raw_type layerID, unsigned long long row, unsigned long long col, double z) = 0;

	virtual DEMLayerInfoSharedPtr findDEMLayerInfo(Identifier::raw_type layerID) = 0;
	virtual DOMLayerInfoSharedPtr findDOMLayerInfo(Identifier::raw_type layerID) = 0;

	virtual DEMBlockSharedPtr getDEMBlock(Identifier::raw_type layid, int rowBlock, int colBlock) = 0;
	virtual DOMBlockSharedPtr getDOMBlock(Identifier::raw_type layid, int rowBlock, int colBlock) = 0;
	virtual DEMBlockSharedPtr getDEMBlock(Identifier::raw_type layid, Identifier::raw_type blockid) = 0;
	virtual DOMBlockSharedPtr getDOMBlock(Identifier::raw_type layid, Identifier::raw_type blockid) = 0;
	/** 通过数据块在整个数据库中的行号和列号，计算数据块的唯一ID
	* @param [in]	block_row			数据块在整个数据库中的行号
	* @param [in]	block_col			数据块在整个数据库中的列号
	* @return 数据块的唯一ID
	*/
	static unsigned long long calculate(int block_row, int block_col)
	{
		unsigned long long id = 0;
		id = block_row;
		id = id << 32;
		id += block_col;
		return id;
	}
	/** 通过数据块的唯一ID，计算数据块在整个数据库中的行号和列号
	* @param [in]	id      			数据块的唯一ID
	* @param [in]	block_row			数据块在整个数据库中的行号
	* @param [in]	block_col			数据块在整个数据库中的列号
	* @return
	*/
	static void calculate(unsigned long long id, int & block_row, int & block_col)
	{
		block_col = (int)id;
		block_row = (int)(id >> 32);
	}
	/** 通过某个点在整个数据库中的单元格行号和列号，每个数据块包含的单元格行数和列数，计算其所在数据块的唯一ID，并该点在数据块中的行号和列号
	* @param [in]	row      			某点在数据库中行号
	* @param [in]	col      			某点在数据库中列号
	* @param [in]	rowsPreblock      	每个数据块包含的单元格行数
	* @param [in]	colsPreblock      	每个数据块包含的单元格列数
	* @param [in,out]	block_id      		数据块的唯一ID
	* @param [in,out]	row_in_block		某点在数据块中的行号
	* @param [in,out]	col_in_block		某点在数据块中的列号
	* @return
	*/
	static void calculate(unsigned long long row, unsigned long long col,
		int rowsPreblock, int colsPreblock,
		unsigned long long & block_id, int & row_in_block, int & col_in_block)
	{
		int  block_row = (int)(row / (rowsPreblock - 1));
		int  block_col = (int)(col / (colsPreblock - 1));
		block_id = calculate(block_row, block_col);
		row_in_block = row % (rowsPreblock - 1);
		col_in_block = col % (colsPreblock - 1);
	}
	/** 通过某个点的坐标值计算它在整个数据库中的单元格行号和列号
	* @param [in]	edb2d        	数据库的范围
	* @param [in]	cal2d     	    用于计算在数据库中起始和终止行列的函数
	* @param [in]	cellSize      	单元格大小
	* @param [int,out]	beg_row      			在数据库中行号
	* @param [int,out]	beg_col      			在数据库中列号
	* @param [int,out]	end_row      			在数据库中行号
	* @param [int,out]	end_col      			在数据库中列号
	* @return                           成功返回true否则返回false
	*/
	static bool  calculate(Envelope2d &edb2d, Envelope2d &cal2d, int cellSize, int & beg_row, int & beg_col,
		int & end_row, int & end_col){
		if (Envelope2d::test(cal2d, edb2d) == 0)
			return false;
		else{
			beg_col = (int)((cal2d.minx - edb2d.minx) / cellSize);
			beg_row = (int)((cal2d.miny - edb2d.miny) / cellSize);
			end_col = (int)((cal2d.maxx - edb2d.minx) / cellSize);
			end_row = (int)((cal2d.maxy - edb2d.miny) / cellSize);
			return true;
		}
	}
};
/** @} */


end_gdb_namespace
end_gtl_namespace


