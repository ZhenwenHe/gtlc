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
#include "buffer.h"
#include "material.h"
#include "matrix4x4.h"
#include "indexbuffer.h"
#include "meshproxy.h"



begin_gtl_namespace
begin_gdb_namespace

class MeshBuilder;
typedef std::shared_ptr<MeshBuilder> MeshBuilderSharedPtr;
/** @addtogroup Mesh MeshBuilder-MeshBuilder网格生成管理
*  @{
*/
class CORE_API MeshBuilder {
protected:
	/** 查询接口指针 */
	MeshProxySharedPtr _proxySptr;
	/** 分层数 */
	unsigned long long _layerNumber;
	/** X、Y、Z方向上每个块中的单元格数 */
	unsigned long long _cellNumberPreBlock[3];

	Envelope3d   _totalEnvelope;

	unsigned long long         _propNumberPreCell;//每个单元的属性个数
	std::vector<std::string>   _propNames;//每个属性的名称，数组中元素个数等于_propNumberPreCell
	std::vector<double>        _propRanges;//记录每个属性的最小值和最大值，数组的个数为2*_propNumberPreCell;

protected://转换和显示网格专用的一些变量
	int                        _indexOfProp4Color;//采用哪个属性来确定颜色 
	ColorSeries                _colorSeries;//显示网格用的色系 
public:
	virtual bool open(const Connector& conn) = 0;
	virtual bool create(const Connector& conn) = 0;
	virtual bool append(MeshSharedPtr msp) = 0;//添加网格到网格数据库
	virtual void close() = 0;
	/**  判断工程是否为空
	* @param 无
	* @return 如果为空返回true，否则返回false
	*/
	virtual bool isEmpty() = 0;

	virtual MeshProxySharedPtr getProxy() = 0;
	virtual BlockSharedPtr getBlock(const Identifier & id) = 0;
	virtual void setup(std::vector<MeshSharedPtr> & msps,
		unsigned long long xCellNumberPreBlock, unsigned long long yCellNumberPreBlock, unsigned long long zCellNumberPreBlock,
		unsigned long long layerNumber) = 0;
	virtual Envelope3d & calculateEnvelope() = 0;

public:
	MeshBuilder();

	inline unsigned long long getLayerNumber() { return _layerNumber; }
	inline unsigned long long getCellNumberPreBlock(unsigned long long & x, unsigned long long & y, unsigned long long & z) {
		x = _cellNumberPreBlock[0];
		y = _cellNumberPreBlock[1];
		z = _cellNumberPreBlock[2];
		return x*y*z;
	}
	inline void getPropRange(const int i, double & minz, double & maxz){
		minz = _propRanges[i * 2];
		maxz = _propRanges[i * 2 + 1];
	}
	virtual Envelope3d & getEnvelope(){ return _totalEnvelope; }


};
/** @} */


end_gdb_namespace
end_gtl_namespace


