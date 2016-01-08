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
#include <list>
#include <map>
#include "lock.h"
#include "feature.h"



begin_gtl_namespace
begin_gdb_namespace


/** @defgroup TemporarySets TempSet-临时要素集合，用于存放中间操作结果
*  @{
*/
class CORE_API TempSet
{
	/** @name 保护成员变量说明
	*  @{
	*/
	///**临时数据集的状态*/
	int _status;
	///**临时数据集的透明度*/
	float _alpha;
protected:
	typedef std::pair< FCLSID, FeatureVectorSharedPtr>   FSPAIR;
	typedef std::map< FCLSID, FeatureVectorSharedPtr >   FSMAP;

	/**要素类集列表*/
	FSMAP  _set;	
	/**临时数据集的名称*/
	std::string    _Name;
	/**要素类集列表*/
	int _drawMode;
	/**临时数据集的颜色*/
	int _color;
	
protected:
	/**保护构造函数，防止外面创建*/
	TempSet();

	/** @} */

public:
	/** @name 析构函数
	*  @{
	*/
	virtual ~TempSet();
	/** @} */

	/** 友元类 */
	friend class TemporarySets;
	/** @} */

	/** @name 操作函数集
	*  @{
	*/
	/**  设置临时数据集的绘制模式
	*	 @param [in]      mode  临时数据集的绘制模式
	*   @return 无
	*/
	inline void setDrawMode(const int &mode){ _drawMode = mode; }

	/**  获取临时数据集的绘制模式
	*	 @param [in]      无
	*    @return 临时数据集的绘制模式
	*/
	inline int getDrawMode(){ return _drawMode; }

	/**  设置临时数据集的颜色
	*	 @param [in]      color  临时数据集的颜色
	*   @return 无
	*/
	inline void setColor(const int &color){ _color = color; }

	/**  获取临时数据集的颜色
	*	 @param [in]      无
	*    @return 临时数据集的颜色
	*/
	inline int getColor(){ return _color; }

	/**  设置临时数据集的透明度
	*	 @param [in] alpha  临时数据集的透明度
	*   @return 无
	*/
	//inline void setAlpha(const float &alpha){ _alpha = alpha; }

	/**  获取临时数据集的透明度
	*	 @param [in]      无
	*    @return 临时数据集的透明度
	*/
	//inline float getAlpha(){ return _alpha; }

	/**  设置临时数据集的名称，在一个TemporarySets中式唯一的
	*	 @param [in]      strName  临时数据集的名称
	*   @return 无
	*/
	inline void setName(std::string strName){ _Name = strName; }

	/**  获取临时数据集的名称，在一个TemporarySets中式唯一的
	*	 @param [in]      无
	*    @return 临时数据集的名称
	*/
	inline std::string getName(){ return _Name; }

	/**  设置临时数据集的状态
	*	 @param [in] status  临时数据集的状态
	*    @return 无
	*/
	//inline void setStatus(int status){ _status = status; }

	/**  获取临时数据集的状态
	*	 @param 无
	*    @return 临时数据集的状态
	*/
	//inline int getStatus(){ return _status; }


	/**  获取指定要素类型的所有要素,返回的指针不用外部销毁
	*	 @param [in] fclsid 要素类型的唯一ID
	*    @return 临时数据集的要素类型为fclsid的Feature集合
	*/
	FeatureVectorSharedPtr getFeatures(FCLSID fclsid);

	/**  移除指定要素类型的所有要素
	*	 @param [in] fclsid 要素类型的唯一ID
	*    @return 无
	*/
	FeatureVectorSharedPtr removeFeatures(const FCLSID &fclsid);



	/**  在数据集合通过FCLSID确定应该插入哪个向量数组中，并加入
	*	 @param [in] fsp 要加入临时数据集合的Feature的共享指针
	*    @return 无
	*/
	void addFeature(FeatureSharedPtr fsp);
	/**  在数据集合通过FCLSID和FID确定应该移除哪个要素对象，并从临时集合中移除入
	*	 @param [in] fclsid 要素类型ID
	*	 @param [in] fid 要素ID
	*    @return 无
	*/
	FeatureSharedPtr removeFeature(const FCLSID &fclsid, const FID &fid);

	/**  获取数据集合中的所有Fclsid
	*	 @param [in] fclsids   用来存储所有FCLSID的vector引用
	*    @return 无
	*/
	void getAllFclsid(vector<FCLSID> &fclsids);
	/**  清空Map
	*	 @param 无
	*    @return 无
	*/
	void clear();

	/** @} */

};
typedef std::shared_ptr<TempSet> TempSetSharedPtr;
/** @} */


/** @addtogroup TemporarySets    TemporarySets
*  @{
*/
class CORE_API TemporarySets
{
	/** @name 保护成员变量说明
	*  @{
	*/
protected:
	/**临时数据集的列表*/
	std::list<TempSetSharedPtr>  _data;

	/** @} */
public:
	/** @name 构造与析构函数集
	*  @{
	*/
	TemporarySets(void);
	virtual ~TemporarySets(void);
	/** @} */
public:
	/** @name 操作函数集合
	*  @{
	*/
	/**  在集合中创建一个指定命名的临时数据集，名称是唯一标识，如果创建成功则返回指针，否则返回空指针
	*	 @param [in]  strSetName 要创建的临时数据集的名称
	*    @return 如果创建成功，则加入TemporarySets中，返回指针，否则返回空指针；注意返回的指针不需要外部进行销毁
	*/
	TempSetSharedPtr newTempSet(const std::string &strSetName);
	/**  从集合中查找一个指定命名的临时数据集，名称是唯一标识，如果查找到则返回指针，否则返回空指针
	*	 @param [in]  strSetName 临时数据集的名称
	*    @return 如果成功返回指针，否则返回空指针；注意返回的指针不需要外部进行销毁
	*/
	TempSetSharedPtr findTempSet(const std::string &strSetName);
	/**  从集合中查找第i个tempset
	*	 @param [in]    下标i
	*    @return 如果成功返回指针，否则返回空指针；注意返回的指针不需要外部进行销毁
	*/
	TempSetSharedPtr getTempSetAt(const size_t &i);
	/**  从集合中获取第i个tempset
	*	 @param [in]    下标i
	*    @return 如果成功返回指针，否则返回空指针；注意返回的指针不需要外部进行销毁
	*/
	TempSetSharedPtr operator () (size_t i);
	/**  从集合中删除第i个tempset
	*	 @param [in]    strSetName 临时数据集的名称
	*    @return 如果成功返回指针，否则返回空指针；注意返回的指针不需要外部进行销毁
	*/
	TempSetSharedPtr removeTempSet(const std::string &strSetName);
	/**  获取管理的临时集个数
	*	 @param 无
	*    @return 返回管理的临时集个数
	*/
	inline size_t size(){ return _data.size(); }
	/**  清空TemporarySets，该函数由析构函数自动调用，也可以由用户多次调用
	*	 @param [in]      无
	*    @return 无
	*/
	void clear();
	/** @} */

};

/** @} */

end_gdb_namespace
end_gtl_namespace

