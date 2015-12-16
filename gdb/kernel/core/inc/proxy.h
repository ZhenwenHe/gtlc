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


#include "Selector.h"
#include "TemporarySet.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


class QResult;

/** @defgroup proxy Proxy-数据查询接口  
    *  @{
    */
class GV3DCORE_API Proxy {	
	
	/** @name 构造与析构函数 
		*  @{
		*/
protected:	
	Proxy();
public:
	virtual ~Proxy();
	/** @} */ 
	/** @name 保护成员变量说明  
	*  @{
	*/
protected:
	/** 临时层指针 */
	TemporarySets * _tempsets;
	/** @} */ 

protected:
	/** @name 保护成员函数说明  
	*  @{
	*/

public:
	virtual void setTemporarySets(TemporarySets * p) {_tempsets = p;}

	/** 获取临时要素集
	* @param  无
	* @return tempSets 临时要素集的引用
	*/	
	virtual TemporarySets& getTemporarySets()  { return * _tempsets;}
	/** @} */ 

		/** @name 所有的查询操作函数 
	*  @{
	*/
	/** 查询指定包围盒内的所有要素对象
	* @param  [in] bound 包围盒范围
	* @param  [in] fcid 要素类ID集合
	* @param  [out] objs 查询到的所有要素对象集合
	* @param  [in] readProps 是否读取要素对象的属性
	* @param  [in] r 过滤半径，如果对象的包围盒半径小于r则不调度
	* @param  [in] removes包含在数组中的FID不需要进行调度
	* @return 如果查询成功，返回true，否则返回false
	*/
	virtual bool queryByBox(const Envelope3dRef bound, 		
		const std::vector<FCLSID> & fcid,
		std::vector<FeatureSharedPtr> & objs,
		const bool readProps = true,const double &r = 0.0,std::vector<FID> * removes=0)=0;

	/** 查询指定包围盒内的所有要素对象
	* @param  [in] bound 包围盒范围
	* @param  [in] fcid 要素类ID集合
	* @param  [in] readProps 是否读取要素对象的属性
	* @return 查询结果的共享指针
	*/
	virtual std::shared_ptr<QResult> queryByBox(const Envelope3dRef bound, 		
		const std::vector<FCLSID> & fcid, const bool readProps = true)=0;

	/** 查询指定点选的所有要素对象
	* @param  [in] point 选择的顶点
	* @param  [in] fcid 要素类ID集合
	* @param  [out] objs 查询到的所有要素对象集合
	* @param  [in] readProps 是否读取要素对象的属性
	* @return 如果查询成功，返回true，否则返回false
	*/
	virtual bool queryByPoint(const Vertex3dRef point, 
		const std::vector<FCLSID> & fcid, 
		std::vector<FeatureSharedPtr> & objs, const bool readProps = true)=0;	

	/** 查询线通过的所有要素对象
	* @param  [in] mpline 选择的线
	* @param  [in] fcid 要素类ID集合
	* @param  [out] objs 查询到的所有要素对象集合
	* @param  [in] readProps 是否读取要素对象的属性
	* @return 如果查询成功，返回true，否则返回false
	*/	
	virtual bool queryByLine(const Vertex3dVector & mpline ,
		const std::vector<FCLSID> & fcid, 
		std::vector<FeatureSharedPtr> & objs, const bool readProps = true)=0;

	/** 查询线通过的所有要素对象
	* @param  [in] mpline 选择的线
	* @param  [in] fcid 要素类ID集合
	* @param  [in] readProps 是否读取要素对象的属性
	* @return 查询结果的共享指针
	*/
	virtual std::shared_ptr<QResult> queryByLine(const Vertex3dVector & mpline ,
		const std::vector<FCLSID> & fcid, const bool readProps = true)=0;

	/** 查询指定多边形范围内的所有要素对象
	* @param  [in] mpgon 选择的多边形范围
	* @param  [in] fcid 要素类ID集合
	* @param  [out] objs 查询到的所有要素对象集合
	* @param  [in] readProps 是否读取要素对象的属性
	* @return 如果查询成功，返回true，否则返回false
	*/
	virtual bool queryByPolygon(const Vertex3dVector & mpgon,
		const std::vector<FCLSID> & fcid, 
		std::vector<FeatureSharedPtr> & objs, const bool readProps = true)=0;

	/** 查询指定多边形范围内的所有要素对象
	* @param  [in] mpgon 选择的多边形范围
	* @param  [in] fcid 要素类ID集合
	* @param  [in] readProps 是否读取要素对象的属性
	* @return 查询结果的共享指针
	*/
	virtual std::shared_ptr<QResult> queryByPolygon(const Vertex3dVector & mpgon,
		const std::vector<FCLSID> & fcid, const bool readProps = true)=0;

	/** 根据对象的 FID 查询指定对象
	* @param  [in] fid 需要查找对象的FID
	* @param  [in] fcid 需要查找对象的要素类的FCLSID
	* @param  [out] obj 查询得到的要素对象
	* @param  [in] readProps 是否读取要素对象的属性
	* @return 如果查询成功，返回true，否则返回false
	*/
	virtual bool queryByFeatureID(const FID &fid, const FCLSID &fcid,  FeatureSharedPtr & obj, const bool readProps = true)=0; 

	/** 根据多个对象的fids查询多个对象
	* @param  [in] fids 需要查找对象所有FID集合
	* @param  [in] fcid 需要查找对象的该要素类的FCLSID
	* @param  [out] objs 查询得到的要素对象
	* @param  [in] readProps 是否读取要素对象的属性
	* @return 如果查询成功，返回true，否则返回false
	*/
	virtual bool queryByFeatureIDs(const std::vector<FID> &fids,const FCLSID &fcid,
		vector <FeatureSharedPtr> &objs, const bool readProps = true)=0;

	/** 根据要素类获取ID
	* @param  [in] fcid 需要查找的要素类的FCLSID
	* @param  [out] obj 得到的要素对象
	* @param  [in] readProps 是否读取要素对象的属性
	* @return 如果查询成功，返回true，否则返回false
	*/
	virtual bool queryByFeatureClassID(const FCLSID &fcid, vector<FeatureSharedPtr> & obj,const bool readProps = true)=0; 

	/** 根据要素类ID获取对象ID集合
	* @param  [in] fcid 需要查找的要素类的FCLSID
	* @param  [out] fids 查询得到的要素对象ID集合
	* @return 如果查询成功，返回true，否则返回false
	*/
	virtual bool queryFeatureIDsByFeatureClassID(const FCLSID &fcid, vector<FID> & fids)=0; 

	/** 查询指定要素ID的指定LOD级别的要素对象
	* @param  [in] fid 需要查找的要素对象id
	* @param  [in] flod 需要查找的要素对象的指定LOD级别
	* @param  [in] fcid 需要查找的要素对象所在要素类的FCLSID
	* @return 查找到的要素对象
	*/
	virtual FeatureSharedPtr loadLOD( const FID &fid, const FLOD &flod, const FCLSID &fcid)=0;
	
	/** 查询指定要素ID的指定LOD级别的要素对象
	* @param  [in] fid 需要查找的要素对象id
	* @param  [in] flod 需要查找的要素对象的指定LOD级别
	* @param  [in] fcid 需要查找的要素对象所在要素类的FCLSID
	* @param  [out] obj 查找到的要素对象
	* @return 无
	*/
	virtual void loadLOD( const FID &fid, const FLOD &flod, const FCLSID &fcid, FeatureSharedPtr & obj)=0;

	/** 查询指定要素ID的指定LOD级别的要素对象集合(flod必须为const类型)
	* @param  [in] fid 需要查找的要素对象id集合
	* @param  [in] flod 需要查找的要素对象的指定LOD级别
	* @param  [in] fcid 需要查找的要素对象所在要素类的FCLSID
	* @param  [out] objs 查找到的要素对象集合
	* @return 无
	*/
	virtual void loadLOD( const std::vector<FID> & fid, const FLOD & flod, const FCLSID &fcid,
		std::vector<FeatureSharedPtr> &objs)=0;

	/** 查询指定要素类型ID的指定LOD级别的要素对象集合(flod必须为非const类型)
	* @param  [in] fcid 需要查找的要素对象所在要素类的FCLSID集合
	* @param  [in] flod 需要查找的要素对象的指定LOD级别
	* @param  [out] objs 查找到的要素对象集合
	* @return 无
	*/
	virtual void loadLOD( const std::vector<FCLSID> & fcid,
		const FLOD & flod, 
		std::vector<FeatureSharedPtr> &objs)=0;

	/** 查询指定要素对象集合
	* @param  [in] objs 查找到的要素对象集合
	* @return 无
	*/
	virtual void loadLOD( std::vector<FeatureSharedPtr> &objs)=0;

	/** 获取工程内所有的要素类型集合
	* @param  [out] fcls_set 查找到的所有要素类型集合
	* @return 无
	*/
	virtual void getAllFeatureClass(std::vector<FeatureClassSharedPtr> & fcls_set)=0;

	/** 根据要素类ID获取要素类
	* @param  [in] fcid 需要查找的要素类型ID
	* @param  [out] fcls 查找到的要素类型
	* @return 如果查询成功，则返回true，否则返回false
	*/
	virtual bool getFeatureClass(const FCLSID &fcid, FeatureClassSharedPtr &fcls)=0; 

	/** 从纹理库中获取纹理对象
	* @param  [in] texid 需要查找的纹理ID
	* @return 查找到的纹理对象
	*/
	virtual TextureSharedPtr getTexture(const TEXID &texid ,int iLevel = 0 )=0;

	/** 从纹理库中获取纹理对象
	* @param  [in] texids 需要查找的所有纹理ID集合
	* @param  [out] Textures 查找到的纹理
	* @return 无
	*/
	virtual void getTexture(const vector<TEXID> &texids , vector<TextureSharedPtr> &Textures,int iLevel = 0 )=0;
	
	/** 从材质库中获取材质对象
	* @param  [in] matid 需要查找的纹理ID
	* @return 查找到的材质对象
	*/
	virtual MaterialSharedPtr getMaterial(const MATID &matid )=0;

	/** 从材质库中获取材质对象
	* @param  [in] matids 需要查找的所有材质ID集合
	* @param  [out] Mats 查找到的纹理集合
	* @return 无
	*/
	virtual void getMaterial(const vector<MATID> &matids , vector<MaterialSharedPtr> &Mats )=0;

	/** 从共享模型库中获取共享模型信息
	* @param  [in] mdlid 需要查找的共享模型ID
	* @return 查找到的共享模型
	*/
	virtual SharedModelSharedPtr getSharedModel(const MDLID  &mdlid)=0;

	/** 从共享模型库中获取共享模型信息
	* @param  [in] Modleids 需要查找的所有共享模型ID集合
	* @param  [out] Modles 查找到的纹理集合
	* @return 无
	*/
	virtual void getSharedModel(const vector<MDLID> &Modleids , vector<SharedModelSharedPtr> &Modles )=0;

	/** 判断指定的时间以后，指定的FID要素对象是否改变，如果改变返回TRUE，并且返回一个新的要素对象，否则返回FALSE，第三个参数为空	
	* @param  [in] t 指定的时间
	* @param  [in] fid 指定要素对象ID
	* @param  [in] outpf 获取的已经更新的要素对象
	* @return 如果操作成功，则返回true，否则返回false
	*/
	virtual bool isUpdated(const time_t &t , const FID &fid,FeatureSharedPtr & outpf)=0;

	
	/** 获取所有texture的ID
	* @param  [in] g_texID 获取到的所有texture的ID
	* @return 如果操作成功，则返回true，否则返回false
	*/
	virtual bool getAllTextureID( std::vector<TEXID> &g_texID)=0;

	/** 获取所有material的ID
	* @param [in]  g_matID 获取到的所有material的ID
	* @return 如果操作成功，则返回true，否则返回false
	*/
	virtual bool getAllMaterialID( std::vector<MATID> &g_matID) = 0;

	/** 获取所有共享模型ID
	* @param [in]  g_mdlID 获取到的所有共享模型的ID
	* @return 如果操作成功，则返回true，否则返回false
	*/
	virtual bool getAllSharedModelID( std::vector<MDLID> &g_mdlID) = 0;

	/** 通过SQL语句查询要素对象
	* @param [in]  fclsid 所需要查找的要素对象类型ID
	* @param [in]  sql 查询对象的SQL语句
	* @param [out]  fResult 获取到的所有要素对象集合
	* @return 如果操作成功，则返回true，否则返回false
	*/
	virtual bool queryBySQL(const FCLSID &fclsid,std::string sql,std::vector<FeatureSharedPtr> &fResult) = 0;

	/** 获取工程最大包围盒
	* @param 无
	* @return 获取的工程包围盒
	*/
	virtual Envelope3d getEnvelope()= 0;

	/**  根据传入的FCLSID的值获取所有features的个数
	 *	 @param [in]      fclsid   要素类型的唯一ID
	 *   @return 获取指定类型的要素对象个数    
	 */
	virtual int getFeatureCount(const FCLSID &fclsid) = 0;

	/**  获取工程下所有模型数据统计信息
	*	 @param [in]      info    空间数据库统计信息；               
	*                     info[0] 要素类型种类个数为 m
	*                     info[1] 要素类型1的要素个数
	*                     info[2] 要素类型2的要素个数
	*                     ...... 
	*                     info[m] 要素类型m的要素个数
	*                     info[m+1] 顶点数；
	*                               顶点的有效纹理坐标数；
	*                               顶点的有效颜色数；
	*								面对象数；
	*                               法向数；
	*                               转化为等价三角面片数；
	*                               材质数
	*								纹理数
	*   @return 返回统计信息    
	*/
	virtual int getStatisticsInfo(std::vector<unsigned long long > & info) = 0;
	/** @} */ 


	/** @name 拓扑操作相关函数
	*  @{
	*/	
	/** 获取所有TOPO节点，为入库出库所用
	* @param  [out] nodes 语义拓扑的结点
	* @return 0 -- 表示失败，1 -- 表示返回成功
	*/
	virtual int getAllTopos(std::vector<TopoItemSharedPtr>& nodes) = 0;

	/** 获取所有TOPO语义信息，为入库出库所用
	* @param  [out] infos 语义拓扑信息
	* @return 0 -- 表示失败，1 -- 表示返回成功
	*/
	virtual int getAllTopos(std::vector<TopoInfoSharedPtr>&infos) = 0;
	
	/** 查询指定要素ID和指定语义拓扑类型的语义拓扑结点集合
	* @param [in]		fid			要素ID
	* @param [in]		type		语义拓扑查询类型, 为0时表示所有类型
	* @param [in]		fclsids		需要查询的有语义拓扑关系的要素类型
	* @param [out]	 topologyItems	返回的语义拓扑的结点
	* @param [in]		queryFlag	查询标识
	* @return  0 -- 表示失败，1 -- 表示返回成功
	*/	
	virtual long queryTopos(const FID &fid, const int &type, const vector<FCLSID>& fclsids, vector<TopoItemSharedPtr>& topologyItems, int queryFlag = 0) = 0;
	
	/** 查询指定要素ID和指定语义拓扑类型的语义拓扑结点集合
	* @param [in]		fid			要素ID
	* @param [in]		type		语义拓扑查询类型, 为0时表示所有类型
	* @param [out]	topologyItems	返回的语义拓扑的结点
	* @param [in]		queryFlag	查询标识
	* @return  0 -- 表示失败，1 -- 表示返回成功
	*/	
	virtual long queryTopos(const FID& fid, const int &type, vector<TopoItemSharedPtr>& topologyItems, int queryFlag = 0) = 0;
	
	/** 查询指定要素ID和指定类型的语义拓扑信息数据
	* @param [in]		fid			要素ID
	* @param [in]		type		语义拓扑信息类型
	* @param [out]	topologyInfo		返回的语义拓扑信息对象
	* @return  0 -- 表示失败，1 -- 表示返回成功
	*/	
	virtual long queryTopo(const FID &fid, const int &type, TopoInfoSharedPtr& topologyInfo) = 0;
	

	/** 查询指定要素ID和指定语义拓扑类型的语义拓扑结点集合
	* @param [in]		fids		要素ID集合
	* @param [in]		type		语义拓扑查询类型
	* @param [out]	topologyInfos	返回的语义拓扑信息对象集合
	* @return  0 -- 表示失败，1 -- 表示返回成功
	*/	
	virtual long queryTopos(const vector<FID>& fids,const  int &type, vector<TopoInfoSharedPtr>& topologyInfos) = 0;
	/** 获取纹理的所有级别，内部调用  
	* @param  [in] fid 待获取纹理对象ID
	* @param  [out] levelSet 纹理级别集合
	* @return true  成功,false 失败
	*/
	virtual bool getAllTextureLevel(const TEXID &texid,std::set<int> &levelSet) = 0;



	virtual void getSharedModelIDs(GeometrySharedPtr p,std::vector<MDLID> & )=0;
	virtual void getMaterilIDs( GeometrySharedPtr pGeom,std::vector<MATID> &)=0;
	/** @} */ 

	/** @name 保护成员函数说明  
	*  @{
	*/
protected:      

	/** 查询指定包围盒内的所有不在缓存里的对象 
	* @param  [in] bound 要查询的包围盒
	* @param  [in] fcid 要添加的FeatureClass共享指针对象
	* @param  [out] objs 查询到的所有要素对象
	* @param  [in] readProps 是否读取属性
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool queryByBound(const Envelope3dRef bound, 	
		const std::vector<FCLSID> & fcid,
		std::vector<FeatureSharedPtr> & objs,const bool readProps = true)=0;

	virtual double  getDiagonal(double d[]);
	/** @} */ 
};
/** @} */  


/** @addtogroup proxy QResult-查询结果,该类仅在Proxy内部进行创建，外部不能创建,只能使用其指针
*  @{
*/
class GV3DCORE_API QResult{	
	/** @name 私有成员变量说明  
	*  @{
	*/
	/** 类Proxy */
	Proxy * _proxy;
    /** @} */ 

protected:
	/** @name 保护成员变量说明  
	*  @{
	*/
	/** 由于缓存大小限制，剩余的查询的要素对象ID集合 */
	std::vector<pair<FID,FCLSID> > _remainder;
	/** 当前索引值 */
	size_t                         _cur_index;
	/** 返回的要素对象列表 */
	std::vector<FeatureSharedPtr>  _result;
	/** @} */ 
public:
	/** @name 构造与析构函数 
	*  @{
	*/
	QResult(Proxy * p){
		_proxy = p;	
		_cur_index=-1;
	}
	~QResult(){
		_proxy = NULL;
		_cur_index=-1;
	}
	/** @} */

	/** @name 结果类中的所有函数  
	*  @{
	*/	  
	/** 逐个获取待获取的数据
	* @param  无
	* @return 获取的要素对象
	*/
	FeatureSharedPtr next(){
		size_t s1=_result.size();
		size_t s2 = _remainder.size();
		FeatureSharedPtr feaSptr;
		if(s1==0 && s2 ==0) 
			return feaSptr;
		_cur_index++;

		if(_cur_index<s1){//在内存中
			return _result[_cur_index];
		}
		else if(_cur_index>=s1 && _cur_index<s1+s2){
			std::vector<pair<FID,FCLSID> >::iterator  _itrCur = _remainder.begin()+(_cur_index-s1);
			//输入参数，调度指定的数据
			_proxy->queryByFeatureID(_itrCur->first, _itrCur->second, feaSptr);
			return 			feaSptr;
		}
		else{
			return feaSptr;
		}
	}

	/** 获取待获取数据的数据大小
	* @param  无
	* @return 获取的要素对象数据的大小
	*/
	inline size_t size(){return _result.size()+_remainder.size();}

	/** 清空结果类
	* @param  无
	* @return 无
	*/
	inline void clear(){
		_remainder.clear();
		_result.clear();
		_cur_index=-1;
	}

	/** 设置已经获取的数据，内部调用
	* @param  [in] ResultIn 查询到的要素对象集合
	* @return 无
	*/
	void setResult(std::vector<FeatureSharedPtr> & ResultIn){ 
		//先清空以前的数据  [sunka, 2009-9-22]
		_result.clear();
		_result.resize(ResultIn.size());
		//设置新数据
		std::copy(ResultIn.begin(),ResultIn.end(),_result.begin());
	}

	
	/** 添加待获取的数据，内部调用  
	* @param  [in] fid 待获取要素对象ID
	* @param  [in] fclsid 待获取要素类ID
	* @return 无
	*/
	void addRemainder(FID fid,FCLSID fclsid){
		_remainder.push_back(pair<FID,FCLSID>(fid,fclsid));
	};
	
public:
	/** 友元类proxy */
	friend class Proxy;
	/** @} */ 
};
typedef std::shared_ptr<QResult> QResultSharedPtr;
/** @} */ 
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
