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
#include "terrain.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace



class TerrainProxy;
typedef std::shared_ptr<TerrainProxy>  TerrainProxySharedPtr;

//////////////////////////////////////////////////////////
/** @addtogroup terrain TerrainProxy-地形查询接口　
*  @{
*/
class CORE_API TerrainProxy{
public:
	/** 读取指定金字塔层和矩形范围的DEM数据
	* @param [in]  box2d			DEM金字塔层范围
	* @param [in]  nLayerID			DEM金字塔的层ID
	* @param [out]  pDemBlocks		DEM金字塔的数据块数据
	* @return
	*/
	virtual bool extractDEM(const Envelope2d& box2d, Identifier::raw_type  nLayerID, DEMGridSharedPtr & demObj) = 0;

	/** 精确读取指定金字塔层和矩形范围的DOM数据(影像数据)
	* @param [in]	box2d			DOM金字塔层范围
	* @param [in]	nLayerID		DOM金字塔的层ID
	* @param [out]  domObj			dom数据
	* @return
	*/
	virtual bool extractDOM(const Envelope2d& box2d, Identifier::raw_type  nLayerID, DOMGridSharedPtr & domObj) = 0;

	/** 通过层ID、行号和列号获得数据块ID
	* @param [in]	nLayerID		层ID
	* @param [in]	nRow			行号
	* @param [in]	nCol			列号
	* @param [out]  blkID			数据块ID
	* @return
	*/
	virtual bool reverseRowColtoBlockID(Identifier::raw_type  nLayerID, int nRow, int nCol, Identifier::raw_type & blkID) = 0;

	/** 通过数据块ID获得层ID、行号和列号
	* @param [in]	blkID			数据块ID
	* @param [out]	nLayerID		层ID
	* @param [out]	nRow			行号
	* @param [out]  nCol			列号
	* @return
	*/
	virtual bool reverseBlockIDtoRowCol(Identifier::raw_type blkID, Identifier::raw_type& nLayerID, int& nRow, int& nCol) = 0;

	/** 获取指定层ID和范围的数据块ID
	* @param [in]	nLayerID		层ID
	* @param [in]	box2d			范围
	* @param [out]  blkIDs			DEM数据块ID集合
	* @param [in]	nDataType		DEM还是DOM;0:DEM;1:DOM
	* @param [in]	nType			读取类型:0:读取完全在BOX内部的数据块；1：包括相交的数据块
	* @return
	*/
	virtual bool getBlockIDsByScope(Identifier::raw_type  nLayerID, const Envelope2d & box2d, std::vector<Identifier::raw_type>& blkIDs, int nDataType, int nType) = 0;

	/** 获得地形最高分辨率
	* @param [out]  fRes		地形最高分辨率
	* @param [in]	nDataType	DEM还是DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getHighestResolution(double& fRes, int nDataType) = 0;

	/** 获得地形最低分辨率
	* @param [out]  fRes		DEM最低分辨率
	* @param [in]	nDataType	DEM还是DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getLowestResolution(double& fRes, int nDataType) = 0;

	/** 获得DEM数据块的最大和最小高程
	* @param [out]  fMax		地形最高分辨率
	* @param [out]  fLowestRes	地形最高分辨率
	* @param [in]	nDataType	DEM还是DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getDEMBlockMaxAndMinZ(Identifier::raw_type blkid, float& blkMaxZ, float& blkMinZ) = 0;

	/** 获得地形粗分辨率的数据块ID集合
	* @param [out]  blkIDs		DEM/DOM数据块ID集合
	* @param [int]  nDataType	DEM还是DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getPyramidTopBlockIDs(vector<Identifier::raw_type>& blkIDs, Identifier::raw_type & nLayerID, int nDataType) = 0;


	/** 读取指定金字塔层的DEM数据，DemBlock 对象在函数内部通过 new 产生，由上层函数调用者维护并负责释放
	* @param [in]  nLayerID			DEM金字塔层ID
	* @param [out]  pDemBlocks		DEM金字塔的数据块数据集合
	* @return
	*/
	virtual bool getDEMDataByLayerID(Identifier::raw_type  nLayerID, vector<DEMBlockSharedPtr>& pDemBlocks) = 0;


	/** 读取指定数据块ID的DEM数据，DemBlock 对象在函数内部通过 new 产生，由上层函数调用者维护并负责释放
	* @param [in]	blkID		数据块ID
	* @param [out]  pDemBlock	DEM金字塔的数据块数据
	* @return
	*/
	virtual bool getDEMDataByBlockID(Identifier::raw_type blkID, DEMBlockSharedPtr& pDemBlock) = 0;

	/** 读取指定数据块ID和指定索引的DEM数据，DemHeight 对象在函数外部通过 new 产生，由上层函数调用者维护并负责释放
	* @param [in]	blkID		数据块ID
	* @param [in]	nStartIndex		起始索引号
	* @param [in]	nLength		索引长度
	* @param [out]  pDemHeight	DEM金字塔的高程数据
	* @return
	*/
	virtual bool getDEMDataByIndex(Identifier::raw_type blkID, int nStartIndex, int nLength, float*& pDemHeight) = 0;

	/** 读取指定金字塔层和数据块ID的DEM数据，DemBlock 对象在函数内部通过 new 产生，由上层函数调用者维护并负责释放
	* @param [in]	nLayerID	层ID
	* @param [in]	blkIDs		数据块ID
	* @param [out]  pDemBlocks	DEM金字塔的数据块数据
	* @return
	*/
	virtual bool getDEMDataByBlockIDs(Identifier::raw_type  nLayerID, vector<Identifier::raw_type> blkIDs, vector<DEMBlockSharedPtr>& pDemBlocks) = 0;


	/** 读取指定金字塔层和矩形范围的DOM数据，ImageBlock 对象在函数内部通过 new 产生，由上层函数调用者维护并负责释放
	* @param [in]  box2d			DOM金字塔层范围
	* @param [in]  nLayerID			DOM金字塔的层ID
	* @param [out]  pImageBlocks	DOM金字塔的数据块数据
	* @param [in]  nType			金字塔的读取类型
	* @return
	*/
	virtual bool getDOMDataByBox(const Envelope2d & box2d, Identifier::raw_type  nLayerID, vector<DOMBlockSharedPtr>& pDomBlocks, int nType) = 0;


	/** 读取指定金字塔层的DOM数据，DomBlock 对象在函数内部通过 new 产生，由上层函数调用者维护并负责释放
	* @param [in]  nLayerID			DOM金字塔层ID
	* @param [out]  pDomBlocks		DOM金字塔的数据块数据集合
	* @return
	*/
	virtual bool getDOMDataByLayerID(Identifier::raw_type  nLayerID, vector<DOMBlockSharedPtr>& pDomBlocks) = 0;

	//##ModelId=4A15463701CD
	/** 读取指定金字塔层和数据块ID的DOM数据，ImageBlock 对象在函数内部通过 new 产生，由上层函数调用者维护并负责释放
	* @param [in]	blkID		DOM数据块ID
	* @param [out]  pImageBlock	DOM金字塔的数据块数据
	* @return
	*/
	virtual bool getDOMDataByBlockID(Identifier::raw_type blkID, DOMBlockSharedPtr& pDomBlock) = 0;

	/** 读取指定金字塔层和数据块ID的DEM数据，DemBlock 对象在函数内部通过 new 产生，由上层函数调用者维护并负责释放
	* @param [in]	nLayerID	层ID
	* @param [in]	blkIDs		数据块ID
	* @param [out]  pDemBlocks	DOM金字塔的数据块数据
	* @return
	*/
	virtual bool getDOMDataByBlockIDs(Identifier::raw_type  nLayerID, vector<Identifier::raw_type> blkIDs, vector<DOMBlockSharedPtr>& pDomBlocks) = 0;

	/** 得到上边的数据块编号
	* @param [in]	nCurBlkID			当前数据块ID
	* @param [out]  nUpperBlkID			上边数据块ID
	* @param [int]  nDataType			DEM还是DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getTopBlockID(Identifier::raw_type nCurBlkID, Identifier::raw_type& nUpperBlkID, int nDataType) = 0;

	/** 得到下边的数据块编号
	* @param [in]	nCurBlkID			当前数据块ID
	* @param [out]  nLowerBlkID			下边的数据块ID
	* @param [int]  nDataType			DEM还是DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getBottomBlockIDs(Identifier::raw_type nCurBlkID, vector<Identifier::raw_type>& nLowerBlkIDs, int nDataType) = 0;


	/** 得到右边的数据块编号
	* @param [in]	nCurBlkID			当前数据块ID
	* @param [out]  nRightBlkID			右边的数据块ID
	* @param [int]  nDataType			DEM还是DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getRightBlockID(Identifier::raw_type nCurBlkID, Identifier::raw_type& nRightBlkID, int nDataType) = 0;


	/** 得到左边的数据块编号
	* @param [in]	nCurBlkID			当前数据块ID
	* @param [out]  nLeftBlkID			左边的数据块ID
	* @param [int]  nDataType			DEM还是DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getLeftBlockID(Identifier::raw_type nCurBlkID, Identifier::raw_type& nLeftBlkID, int nDataType) = 0;


	/** 得到儿子数据块编号
	* @param [in]	nCurLyrID			当前数据块所在层ID
	* @param [in]	nCurBlkID			当前数据块ID
	* @param [in]	nChildIDs			当前数据块的孩子数据块ID
	* @param [out]  nChildLyrID			儿子的数据块所在层ID
	* @param [int]  nDataType			DEM还是DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getChildBlockID(Identifier::raw_type nCurLyrID, Identifier::raw_type nCurBlkID, vector<Identifier::raw_type>& nChildIDs, Identifier::raw_type& nChildLyrID, int nDataType) = 0;

	/** 得到父亲数据块编号
	* @param [in]	nCurLyrID			当前数据块所在层ID
	* @param [in]	nCurBlkID			当前数据块ID
	* @param [in]	nParentID			当前数据块的父亲数据块ID
	* @param [out]  nParentLyrID		父亲的数据块所在层ID
	* @param [int]  nDataType			DEM还是DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getParentBlockID(Identifier::raw_type nCurLyrID, Identifier::raw_type nCurBlkID, Identifier::raw_type& nParentID, Identifier::raw_type& nParentLyrID, int nDataType) = 0;


	/** 得到金字塔的层数
	* @param [in]	nLayerNum			获取的总金字塔层数
	* @param [in]	nDataType			DEM还是DOM数据: 0 - DEM,1 - DOM
	* @return
	*/
	virtual bool getPyramidLayerNumber(int& nLayerNum, int nDataType) = 0;

	//##ModelId=4A1546370205
	/** 得到DEM金子塔层的信息
	* @param [in]	nLayerID		获取的总金字塔层数
	* @param [in]	m_DemLayerInfo	金字塔层的数据
	* @return
	*/
	virtual bool getDEMLayerInfo(Identifier::raw_type  nLayerID, DEMLayerInfoSharedPtr& demLayerInfo) = 0;

	//##ModelId=4A1546370208
	/** 得到DOM金子塔层的信息
	* @param [in]	nLayerID		获取的总金字塔层数
	* @param [in]	m_DomLayerInfo	DOM金字塔层的数据
	* @return
	*/
	virtual bool getDOMLayerInfo(Identifier::raw_type  nLayerID, DOMLayerInfoSharedPtr& domLayerInfo) = 0;

	//##ModelId=4A154637020B
	/** 得到DEM数据库信息
	* @param [out]	demDBInfo		DEM数据库信息
	* @return
	*/
	virtual bool getDEMDBInfo(DEMDBInfoSharedPtr & demDBInfo) = 0;

	//##ModelId=4A154637020D
	/** 得到DOM数据库信息
	* @param [out]	domDBInfo		DOM数据库信息
	* @return
	*/
	virtual bool getDOMDBInfo(DOMDBInfoSharedPtr& domDBInfo) = 0;


	/** 读取指定数据块ID和指定索引的DEM预处理数据的误差数据，pErrorData 对象在函数外部通过 new 产生，由上层函数调用者维护并负责释放
	* @param [in]	blkID				数据块ID
	* @param [in]	nStartIndex			起始索引号
	* @param [in]	nLength				索引长度
	* @param [out]  pErrorData			DEM预处理数据的误差数据
	* @return
	*/
	virtual bool getErrorDataByIndex(Identifier::raw_type blkID, int nStartIndex, int nLength, float*& pErrorData) = 0;

	/** 读取指定数据块ID的预处理数据的boundingbox数据，pBoundingboxData 对象在函数外部通过 new 产生，由上层函数调用者维护并负责释放
	* @param [in]	blkID				数据块ID
	* @param [out]  pBoundingboxData	预处理数据的boundingbox数据
	* @return
	*/
	virtual bool getBoundingBoxData(Identifier::raw_type blkID, float*& pBoundingboxData) = 0;

	/** 得到提交源文件相关信息
	* @param [out]	srcInfo		源文件相关信息
	* @param [in]	nDataType	数据类型
	* @return
	*/
	virtual bool getSourceData(vector<SourceDataInfoSharedPtr>& srcInfo, int nDataType) = 0;

	/** 按照固定分辨率和范围分发DEM数据
	* @param [in]	dfCellSize  固定分辨率
	* @param [in]	szFullPathName	路径
	* @param [in]	Scope	分发范围
	* @return
	*/
	virtual bool queryDEMFile(double dfCellSize, char* szFullPathName, const Envelope2d&  Scope) = 0;

	/** 按照固定分辨率和范围分发DOM数据
	* @param [in]	dfResolution  固定分辨率
	* @param [in]	szFullPathName	路径
	* @param [in]	Scope	分发范围
	* @param [in]	nBitCount	影像位数
	* @return
	*/
	virtual bool queryDOMFile(double dfResolution, char* szFullPathName, const Envelope2d&  Scope, int nBitCount) = 0;
	/** 按照范围查询该范围包含多少块DEMBlock和DOMBlock,只要块的范围与box相交
	* @param [in]	box  查询范围
	* @param [in]	demblocks	DEM块,数据库的内部分块
	* @param [in]	domblocks	DOM块，数据库的内部分块
	* @return
	*/
	virtual bool queryByBox(const Envelope3d& box, std::vector<DEMBlockSharedPtr> &demblocks) = 0;
	virtual bool queryByBox(const Envelope3d& box, std::vector<DOMBlockSharedPtr> &domblocks) = 0;
	virtual bool queryByBox(const Envelope3d& box, std::vector<DEMBlockSharedPtr> &demblocks,
		std::vector<DOMBlockSharedPtr> &domblocks) = 0;

	virtual Envelope3d getEnvelope() = 0;
};
/** @} */


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
